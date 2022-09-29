#include <stdlib.h>
#include <stdbool.h>
#include "lib/scanner/module.h"
#include "lib/vector/vector.h"

#include "parser.h"

#include "src/formats/formats.h"
#include "src/token/token.h"

typedef struct {
	Vector active_nodes;
	Vector parens;
	Vector dyad_operators;
	Vector monad_operators;
} Expression_Parser_State;

static bool has_active_locals(Expression_Parser_State *state, u64 count) {
	if (state->active_nodes.count < count) { return false; }
	if (state->parens.count == 0) { return true; }
	Node_Handle const local = vec_array(&state->active_nodes, Node_Handle)[state->active_nodes.count - count];
	Node_Handle const last_paren = vec_back(&state->parens, Node_Handle);
	return  local >= last_paren;
}

static Node_Handle last_active_local(Expression_Parser_State *state) {
	if (has_active_locals(state, 1)) {
		return vec_back(&state->active_nodes, Node_Handle);
	}
	return -1;
}

static Node_Handle pop_active_local(Expression_Parser_State *state) {
	if (has_active_locals(state, 1)) {
		return vec_pop(&state->active_nodes, Node_Handle);
	}
	return -1;
}

static void desugar_dyad_operator(Expression_Parser_State *state, Ast *result) {
	Vector *const nodes = &state->active_nodes;
	Vector *const dyads = &state->dyad_operators;
	Node_Handle guard = vec_back_or_default(&state->parens, 0, Node_Handle);

	u64 count = 0;
	for (;vec_back_or_default(dyads, -1, Node_Handle) >= guard ; ++count) {
		vec_pop(dyads, Node_Handle);
	}

	Node_Handle *scoped_dyads = vec_end(dyads, Node_Handle);
	u64 right_arg_pos = 0;
	for(u64 i = 0; i < count; ++i) {
		Node_Handle const dyad = scoped_dyads[i];

		for(; vec_array(nodes, Node_Handle)[right_arg_pos] < dyad; ++right_arg_pos) {
			if (right_arg_pos+1 >= state->active_nodes.count) { assert(false&&"lone `:`"); }
		}

		// only accept shapes like `.. a:b ..`
		assert(right_arg_pos > 0 && vec_array(nodes, Node_Handle)[right_arg_pos - 1] >= guard);
		Node_Handle const left_arg  = vec_array(nodes, Node_Handle)[right_arg_pos - 1];
		Node_Handle const right_arg = vec_array(nodes, Node_Handle)[right_arg_pos];

		// expect two dummy nodes to overwrite
		assert(vec_array(&result->nodes, Ast_Node)[dyad].type == Ast_Type_None);
		assert(vec_array(&result->nodes, Ast_Node)[dyad + 1].type == Ast_Type_None);

		vec_array(&result->nodes, Ast_Node)[dyad] = (Ast_Node){ 
			.type= Ast_Type_Identifier, 
			.as_Identifier = {"->"}
		};
		vec_array(&result->nodes, Ast_Node)[dyad + 1] = (Ast_Node){ 
			.type = Ast_Type_Dyadic, .as_Dyadic = {
				.left = left_arg,
				.func = right_arg,
				.right = dyad,
			}
		};

		vec_array(nodes, Node_Handle)[right_arg_pos - 1] = dyad + 1;
		vec_remove(nodes, right_arg_pos, Node_Handle);
	}
}

static void desugar_monad_operator(Expression_Parser_State *state, Ast *result) {
	Vector *const nodes = &state->active_nodes;
	Vector *const monads = &state->monad_operators;
	Node_Handle const guard = vec_back_or_default(&state->parens, 0, Node_Handle);

	u64 count = 0;
	for (; vec_back_or_default(monads, -1, Node_Handle) >= guard; ++count) {
		vec_pop(monads, Node_Handle);
	}

	Node_Handle *const scoped_monads = vec_end(monads, Node_Handle);
	u64 right_arg_pos = 0;
	for(u64 i = 0; i < count; ++i) {
		Node_Handle const monad = scoped_monads[i];

		for(; vec_array(nodes, Node_Handle)[right_arg_pos] < monad; ++right_arg_pos) {
			if (right_arg_pos + 1 >= state->active_nodes.count) { assert(false&&"lone `:`"); }
		}

		// only accept shapes like `.. a.b ..`
		assert (right_arg_pos > 0 && vec_array(nodes, Node_Handle)[right_arg_pos - 1] >= guard);
		Node_Handle const left_arg  = vec_array(nodes, Node_Handle)[right_arg_pos - 1];
		Node_Handle const right_arg = vec_array(nodes, Node_Handle)[right_arg_pos];

		// expect one dummy node to overwrite
		// assert(result->nodes[monad].type == Ast_Type_None);
		assert(vec_array(&result->nodes, Ast_Node)[monad].type == Ast_Type_None);
		vec_array(&result->nodes, Ast_Node)[monad] = (Ast_Node){ .type = Ast_Type_Monadic, .as_Monadic = {
			.func = left_arg,
			.right = right_arg,
		}};

		vec_array(nodes, Node_Handle)[right_arg_pos - 1] = monad;
		vec_remove(nodes, right_arg_pos, Node_Handle);
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
		Node_Handle const handle = vec_push(&result->nodes, node, Ast_Node);
		vec_push(&state->active_nodes, handle, Node_Handle);
	}

	for (; has_active_locals(state, 2);) {
		Node_Handle const right = pop_active_local(state);
		Node_Handle const func  = pop_active_local(state);
		Ast_Node const node = {
			.type = Ast_Type_Monadic,
			.as_Monadic = { .func = func, .right = right, },
		};
		Node_Handle const handle = vec_push(&result->nodes, node, Ast_Node);
		vec_push(&state->active_nodes, handle, Node_Handle);
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
			Ast_Node *const node = &vec_array(&result.nodes, Ast_Node)[last_active_local(&expr_state)];
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
				Node_Handle const handle = vec_push(&result.nodes, node, Ast_Node);
				vec_push(&expr_state.active_nodes, handle, Node_Handle);
			}

			break;case Token_Type_Float: {
				Ast_Node const node = {
					.type = Ast_Type_Array_Ptr,
					.as_Array_Ptr = make_array(&tok.f64value, 1, Type_Float),
				};
				Node_Handle const handle = vec_push(&result.nodes, node, Ast_Node);
				vec_push(&expr_state.active_nodes, handle, Node_Handle);
			}

			break;case Token_Type_Identifier: {
				Ast_Node node = { .type = Ast_Type_Identifier };
				assert(string_len(tok.text) <= 15);
				strncpy(node.as_Identifier.begin, tok.text.begin, string_len(tok.text));
				Node_Handle const handle = vec_push(&result.nodes, node, Ast_Node);
				vec_push(&expr_state.active_nodes, handle, Node_Handle);
			}

			break;case Token_Type_Operator: {
				switch (tok.op) {
					break;case List: {
						enlisting = true;
					}

					break;case LParen: {
						vec_push(&expr_state.parens, result.nodes.count, Node_Handle);
					}

					break;case RParen: {
						assert(expr_state.parens.count > 0);
						apply_functions(&expr_state, &result);
						vec_pop(&expr_state.parens, Node_Handle);
					}

					break;case Assign: {
						assert(expr_state.parens.count == 0); // must be at toplevel
						assert(expr_state.active_nodes.count == 1); // must have something to bind to
						binding_name = vec_pop(&expr_state.active_nodes, Node_Handle);
						assert(vec_array(&result.nodes, Ast_Node)[binding_name].type == Ast_Type_Identifier);
					}

					break;case Dyad: {
						// dummy nodes for '->' and 'dyad'
						vec_push(&result.nodes, (Ast_Node){0}, Ast_Node);
						vec_push(&result.nodes, (Ast_Node){0}, Ast_Node);
						vec_push(&expr_state.dyad_operators, result.nodes.count - 2, Node_Handle);
					}

					break;case Monad: {
						// dummy node for 'monad'
						vec_push(&result.nodes, (Ast_Node){0}, Ast_Node);
						vec_push(&expr_state.monad_operators, result.nodes.count - 1, Node_Handle);
					}

					break;default: {
						assert(false && "unhandled operator");
					}
				}
			}
		}
	}

	assert(expr_state.parens.count == 0);
	apply_functions(&expr_state, &result);

	if (expr_state.active_nodes.count != 0) {
		result.parent = vec_array(&expr_state.active_nodes, Node_Handle)[0];

		if (binding_name >= 0) {
			Ast_Node const node = {
				.type = Ast_Type_Assignment,
				.as_Assignment = {
					.left = binding_name,
					.right = vec_array(&expr_state.active_nodes, Node_Handle)[0],
				},
			};
			result.parent = vec_push(&result.nodes, node, Ast_Node);
		}
	} else {
		result.parent = -1;
	}

	vec_delete(&expr_state.active_nodes);
	vec_delete(&expr_state.parens);
	vec_delete(&expr_state.dyad_operators);
	vec_delete(&expr_state.monad_operators);

	return result;
}
