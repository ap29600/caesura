#include <stdlib.h>
#include <stdbool.h>
#include "lib/scanner/module.h"
#include "lib/vector/vector.h"

#include "parser.h"

#include "src/formats/formats.h"
#include "src/token/token.h"

typedef struct {
	Node_Handle *active_nodes;
	Node_Handle *parens;
	Node_Handle *dyad_operators;
	Node_Handle *monad_operators;
} Expression_Parser_State;

static bool has_active_locals(Expression_Parser_State *state, u64 count) {
	if (Vec_size(state->active_nodes) < count) { return false; }
	if (Vec_size(state->parens) == 0) { return true; }
	Node_Handle const local = state->active_nodes[Vec_size(state->active_nodes) - count];
	Node_Handle const last_paren = Vec_back(state->parens);
	return  local >= last_paren;
}

static Node_Handle last_active_local(Expression_Parser_State *state) {
	if (has_active_locals(state, 1)) {
		return Vec_back(state->active_nodes);
	}
	return -1;
}

static Node_Handle pop_active_local(Expression_Parser_State *state) {
	if (has_active_locals(state, 1)) {
		return Vec_pop_back(&state->active_nodes);
	}
	return -1;
}

static void desugar_dyad_operator(Expression_Parser_State *state, Ast *result) {
	// Node_Handle *const nodes = state->active_nodes;
	// Node_Handle *const dyads = state->dyad_operators;
	Node_Handle const guard = Vec_back_or(state->parens, 0);

	u64 count = 0;
	for (; Vec_back_or(state->dyad_operators, -1) >= guard ; ++count) {
		Vec_pop_back(&state->dyad_operators);
	}

	Node_Handle *scoped_dyads = Vec_end(state->dyad_operators);
	u64 right_arg_pos = 0;
	for(u64 i = 0; i < count; ++i) {
		Node_Handle const dyad = scoped_dyads[i];

		for(; state->active_nodes[right_arg_pos] < dyad; ++right_arg_pos) {
			if (right_arg_pos + 1 >= Vec_size(state->active_nodes)) { assert(false&&"lone `:`"); }
		}

		// only accept shapes like `.. a:b ..`
		assert(right_arg_pos > 0 && state->active_nodes[right_arg_pos - 1] >= guard);
		Node_Handle const left_arg  = state->active_nodes[right_arg_pos - 1];
		Node_Handle const right_arg = state->active_nodes[right_arg_pos];

		// expect two dummy nodes to overwrite
		assert(result->nodes[dyad].type == Ast_Type_None);
		assert(result->nodes[dyad + 1].type == Ast_Type_None);

		result->nodes[dyad] = (Ast_Node){
			.type= Ast_Type_Identifier,
			.as_Identifier = {"->"}
		};
		result->nodes[dyad + 1] = (Ast_Node){
			.type = Ast_Type_Dyadic, .as_Dyadic = {
				.left = left_arg,
				.func = right_arg,
				.right = dyad,
			}
		};

		state->active_nodes[right_arg_pos - 1] = dyad + 1;
		Vec_remove(&state->active_nodes, right_arg_pos);
	}
}

static void desugar_monad_operator(Expression_Parser_State *state, Ast *result) {
	Node_Handle const guard = Vec_back_or(state->parens, 0);

	u64 count = 0;
	for (; Vec_back_or(state->monad_operators, -1) >= guard; ++count) {
		Vec_pop_back(&state->monad_operators);
	}

	Node_Handle *const scoped_monads = Vec_end(state->monad_operators);
	u64 right_arg_pos = 0;
	for(u64 i = 0; i < count; ++i) {
		Node_Handle const monad = scoped_monads[i];

		for(; state->active_nodes[right_arg_pos] < monad; ++right_arg_pos) {
			if (right_arg_pos + 1 >= Vec_size(state->active_nodes)) { assert(false&&"lone `:`"); }
		}

		// only accept shapes like `.. a.b ..`
		assert (right_arg_pos > 0 &&  state->active_nodes[right_arg_pos - 1] >= guard);
		Node_Handle const left_arg  = state->active_nodes[right_arg_pos - 1];
		Node_Handle const right_arg = state->active_nodes[right_arg_pos];

		// expect one dummy node to overwrite
		// assert(result->nodes[monad].type == Ast_Type_None);
		assert(result->nodes[monad].type == Ast_Type_None);
		result->nodes[monad] = (Ast_Node){ .type = Ast_Type_Monadic, .as_Monadic = {
			.func = left_arg,
			.right = right_arg,
		}};

		state->active_nodes[right_arg_pos - 1] = monad;
		Vec_remove(&state->active_nodes, right_arg_pos);
	}
}

static void apply_functions(Expression_Parser_State *state, Ast *result) {

	desugar_dyad_operator(state, result);

	desugar_monad_operator(state, result);

	for (; has_active_locals(state, 3);) {
		Node_Handle const right = pop_active_local(state);
		Node_Handle const func  = pop_active_local(state);
		Node_Handle const left  = pop_active_local(state);
		Ast_Node const node = {
			.type = Ast_Type_Dyadic,
			.as_Dyadic = { .left = left, .func = func, .right = right, },
		};
		Node_Handle const handle = Vec_push_back(&result->nodes, node);
		Vec_push_back(&state->active_nodes, handle);
	}

	for (; has_active_locals(state, 2);) {
		Node_Handle const right = pop_active_local(state);
		Node_Handle const func  = pop_active_local(state);
		Ast_Node const node = {
			.type = Ast_Type_Monadic,
			.as_Monadic = { .func = func, .right = right, },
		};
		Node_Handle const handle = Vec_push_back(&result->nodes, node);
		Vec_push_back(&state->active_nodes, handle);
	}
}

Ast parse_expressions(Scanner *state) {
	Ast result = {0}; // = {.nodes = malloc(sizeof(Ast_Node) * 8), .cap = 8, .count = 0};
	Expression_Parser_State expr_state = {0};

	bool enlisting = false;
	Node_Handle binding_name = -1;

	for (; !scanner_is_empty(state); ) {
		Token const tok = next_token(state);

		if (tok.type == Token_Type_Empty) {
			continue;
		} else if (!tok.is_valid) {
			report_state(state, stdout);
			exit(1);
		}

		if (enlisting) {
			assert(has_active_locals(&expr_state, 1));
			Ast_Node *const node = &result.nodes[last_active_local(&expr_state)];
			assert(node->type == Ast_Type_Array_Ptr);
			switch(tok.type) {
				break;case Token_Type_Float: {
					node->as_Array_Ptr = array_append_elem(&tok.f64value, node->as_Array_Ptr, Type_Float);
				}

				break;case Token_Type_Int:   {
					node->as_Array_Ptr = array_append_elem(&tok.i64value, node->as_Array_Ptr, Type_Int);
				}

				break;default: {
					assert(false);
					unreachable();
				}
			}
			enlisting = false;
			continue;
		}

		switch(tok.type) {
			break;case Token_Type_Empty:

			break;case Token_Type_Int: {
				Ast_Node const node = {
					.type = Ast_Type_Array_Ptr,
					.as_Array_Ptr = make_array(&tok.i64value, 1, Type_Int),
				};
				Node_Handle const handle = Vec_push_back(&result.nodes, node);
				Vec_push_back(&expr_state.active_nodes, handle);
			}

			break;case Token_Type_Float: {
				Ast_Node const node = {
					.type = Ast_Type_Array_Ptr,
					.as_Array_Ptr = make_array(&tok.f64value, 1, Type_Float),
				};
				Node_Handle const handle = Vec_push_back(&result.nodes, node);
				Vec_push_back(&expr_state.active_nodes, handle);
			}

			break;case Token_Type_Identifier: {
				Ast_Node node = { .type = Ast_Type_Identifier };
				assert(string_len(tok.text) <= 15);
				strncpy(node.as_Identifier.begin, tok.text.begin, string_len(tok.text));
				Node_Handle const handle = Vec_push_back(&result.nodes, node);
				Vec_push_back(&expr_state.active_nodes, handle);
			}

			break;case Token_Type_Operator: {
				switch (tok.op) {
					break;case List: {
						enlisting = true;
					}

					break;case LParen: {
						Vec_push_back(&expr_state.parens, Vec_size(result.nodes));
					}

					break;case RParen: {
						assert(Vec_size(expr_state.parens) > 0);
						apply_functions(&expr_state, &result);
						Vec_pop_back(&expr_state.parens);
					}

					break;case Assign: {
						assert(Vec_size(expr_state.parens) == 0); // must be at toplevel
						assert(Vec_size(expr_state.active_nodes) == 1); // must have something to bind to
						binding_name = Vec_pop_back(&expr_state.active_nodes);
						assert(result.nodes[binding_name].type == Ast_Type_Identifier);
					}

					break;case Dyad: {
						// dummy nodes for '->' and 'dyad'
						Vec_push_back(&result.nodes, (Ast_Node){0});
						Vec_push_back(&result.nodes, (Ast_Node){0});
						Vec_push_back(&expr_state.dyad_operators, Vec_size(result.nodes) - 2);
					}

					break;case Monad: {
						// dummy node for 'monad'
						Vec_push_back(&result.nodes, (Ast_Node){0});
						Vec_push_back(&expr_state.monad_operators, Vec_size(result.nodes) - 1);
					}

					break;default: {
						assert(false && "unhandled operator");
					}
				}
			}
		}
	}

	assert(Vec_size(expr_state.parens) == 0);
	apply_functions(&expr_state, &result);

	if (Vec_size(expr_state.active_nodes) != 0) {
		result.parent = expr_state.active_nodes[0];

		if (binding_name >= 0) {
			Ast_Node const node = {
				.type = Ast_Type_Assignment,
				.as_Assignment = {
					.left = binding_name,
					.right = expr_state.active_nodes[0],
				},
			};
			result.parent = Vec_push_back(&result.nodes, node);
		}
	} else {
		result.parent = -1;
	}

	Vec_delete(&expr_state.active_nodes);
	Vec_delete(&expr_state.parens);
	Vec_delete(&expr_state.dyad_operators);
	Vec_delete(&expr_state.monad_operators);

	return result;
}
