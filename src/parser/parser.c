#include <stdlib.h>
#include <stdbool.h>
#include "lib/scanner/module.h"

#include "parser.h"

#include "src/formats/formats.h"
#include "src/token/token.h"


static Node_Handle append_ast_node(Ast *ast, Ast_Node node) {
	if (ast->count >= ast->cap) {
		ast->cap = ast->cap == 0 ? 8 : (ast->cap * 2);
		ast->nodes = realloc(ast->nodes, ast->cap * sizeof(Ast_Node));
	}

	ast->nodes[ast->count] = node;
	return ast->count++;
}

typedef struct {
	Node_Handle *handles;
	u64 count;
	u64 cap;
} Handle_Vector;

static void append_handle(Handle_Vector *vec, Node_Handle handle) {
	if (vec->count >= vec->cap) {
		vec->cap = vec->cap == 0 ? 8 : (vec->cap * 2);
		vec->handles = realloc(vec->handles, vec->cap * sizeof(Node_Handle));
	}

	vec->handles[vec->count] = handle;
	++vec->count;
}

static void insert_handle_at(Handle_Vector *vec, Node_Handle handle, u64 at) {
	assert(at <= vec->count);

	if (vec->count >= vec->cap) {
		vec->cap = vec->cap == 0 ? 8 : (vec->cap * 2);
		vec->handles = realloc(vec->handles, vec->cap * sizeof(Node_Handle));
	}

	++vec->count;
	for(u64 i = vec->count-1; i > at; --i) {
		vec->handles[i] = vec->handles[i-1];
	}

	vec->handles[at] = handle;
}

static void remove_handle_at(Handle_Vector *vec, u64 at) {
	assert(at < vec->count);

	--vec->count;
	for(u64 i = at; i < vec->count; ++i) {
		vec->handles[i] = vec->handles[i+1];
	}
}

static Node_Handle pop_handle(Handle_Vector *vec) {
	return vec->count > 0 ? vec->handles[--vec->count] : -1;
}

static Node_Handle peek_handle(Handle_Vector *vec) {
	return vec->count > 0 ? vec->handles[vec->count-1] : -1;
}

typedef struct {
	Handle_Vector active_nodes;
	Handle_Vector parens;
	Handle_Vector dyad_operators;
	Handle_Vector monad_operators;
} Expression_Parser_State;


static bool has_active_locals(Expression_Parser_State *state, u64 count) {
	if (state->active_nodes.count < count) { return false; }
	if (state->parens.count == 0) { return true; }
	return state->active_nodes.handles[state->active_nodes.count - count] >= state->parens.handles[state->parens.count - 1];
}

static Node_Handle last_active_local(Expression_Parser_State *state) {
	if (has_active_locals(state, 1)) {
		return state->active_nodes.handles[state->active_nodes.count - 1];
	}
	return -1;
}

static Node_Handle pop_active_local(Expression_Parser_State *state) {
	if (has_active_locals(state, 1)) {
		return state->active_nodes.handles[-- state->active_nodes.count];
	}
	return -1;
}

static void desugar_dyad_operator(Expression_Parser_State *state, Ast *result) {
	Node_Handle guard = peek_handle(&state->parens);
	if (guard == -1) { guard = 0; } // pretend expr starts with '('

	u64 count = 0;
	while (peek_handle(&state->dyad_operators) >= guard) {
		++count;
		pop_handle(&state->dyad_operators);
	}

	Node_Handle *scoped_dyads = state->dyad_operators.handles + state->dyad_operators.count;
	u64 right_arg_pos = 0;
	for(u64 i = 0; i < count; ++i) {
		Node_Handle dyad = scoped_dyads[i];

		for(; state->active_nodes.handles[right_arg_pos] < dyad; ++right_arg_pos) {
			if (right_arg_pos+1 >= state->active_nodes.count) { assert(false&&"lone `:`"); }
		}

		// only accept shapes like `.. a:b ..`
		assert (right_arg_pos>0 && state->active_nodes.handles[right_arg_pos-1]>=guard);
		Node_Handle left_arg  = state->active_nodes.handles[right_arg_pos - 1];
		Node_Handle right_arg = state->active_nodes.handles[right_arg_pos];

		// expect two dummy nodes to overwrite
		assert(result->nodes[dyad].type == Node_None);
		assert(result->nodes[dyad+1].type == Node_None);

		result->nodes[dyad] = (Ast_Node){ .type= Node_Identifier, .as.identifier = "->" };
		result->nodes[dyad+1] = (Ast_Node){ .type = Node_Dyad, .as.args = {
			.left = left_arg, .callee = right_arg, .right = dyad,
		}};

		state->active_nodes.handles[right_arg_pos-1] = dyad+1;
		remove_handle_at(&state->active_nodes, right_arg_pos);
	}
}

static void desugar_monad_operator(Expression_Parser_State *state, Ast *result) {
	Node_Handle guard = peek_handle(&state->parens);
	if (guard == -1) { guard = 0; } // pretend expr starts with '('

	u64 count = 0;
	while (peek_handle(&state->monad_operators) >= guard) {
		++count;
		pop_handle(&state->monad_operators);
	}

	Node_Handle *scoped_monads = state->monad_operators.handles + state->monad_operators.count;
	u64 right_arg_pos = 0;
	for(u64 i = 0; i < count; ++i) {
		Node_Handle monad = scoped_monads[i];

		for(; state->active_nodes.handles[right_arg_pos] < monad; ++right_arg_pos) {
			if (right_arg_pos+1 >= state->active_nodes.count) { assert(false&&"lone `:`"); }
		}

		// only accept shapes like `.. a.b ..`
		assert (right_arg_pos>0 && state->active_nodes.handles[right_arg_pos-1]>=guard);
		Node_Handle left_arg  = state->active_nodes.handles[right_arg_pos - 1];
		Node_Handle right_arg = state->active_nodes.handles[right_arg_pos];

		// expect one dummy node to overwrite
		assert(result->nodes[monad].type == Node_None);

		result->nodes[monad] = (Ast_Node){ .type = Node_Monad, .as.args = {
			.callee = left_arg, .right = right_arg,
		}};

		state->active_nodes.handles[right_arg_pos-1] = monad;
		remove_handle_at(&state->active_nodes, right_arg_pos);
	}
}

static void apply_functions(Expression_Parser_State *state, Ast *result) {

	desugar_dyad_operator(state, result);
	desugar_monad_operator(state, result);

	while (has_active_locals(state, 3)) {
		Node_Handle right  = pop_active_local(state);
		Node_Handle callee = pop_active_local(state);
		Node_Handle left   = pop_active_local(state);
		Node_Handle val = append_ast_node(
				result, (Ast_Node){
					.type = Node_Dyad,
					.as.args = {
						.left = left,
						.callee = callee,
						.right = right,
					}
				});

		append_handle(&state->active_nodes, val);
	}

	while (has_active_locals(state, 2)) {
		Node_Handle right  = pop_active_local(state);
		Node_Handle callee = pop_active_local(state);
		Node_Handle val = append_ast_node(
				result, (Ast_Node){
					.type = Node_Monad,
					.as.args = {
						.callee = callee,
						.right = right,
					}
				});

		append_handle(&state->active_nodes, val);
	}
}

Ast parse_expressions(Scanner *state) {
	Ast result = {.nodes = malloc(sizeof(Ast_Node) * 8), .cap = 8, .count = 0};

	Expression_Parser_State expr_state = {0};

	bool enlisting = false;
	Node_Handle binding_name = -1;

	for (; !scanner_is_empty(state); ) {
		Token tok = next_token(state);

		if (!tok.is_valid) {
			report_state(state, stdout);
			exit(1);
		}

		if (enlisting) {
			assert(has_active_locals(&expr_state, 1));
			Ast_Node *node = &result.nodes[last_active_local(&expr_state)];
			assert(node->type == Node_Array);
			switch(tok.type) {
				break;case Float: node->as.array = array_append_elem(&tok.f64value, node->as.array, Type_Float);
				break;case Int:   node->as.array = array_append_elem(&tok.i64value, node->as.array, Type_Int);
				break;default:
					assert(false);
					unreachable();
			}
			enlisting = false;
			continue;
		}

		switch(tok.type) {
			break;case Empty:

			break;case Int:
				append_handle(&expr_state.active_nodes,
					append_ast_node(&result, (Ast_Node){
						.type = Node_Array,
						.as.array = make_array(&tok.i64value, 1, Type_Int),
					})
				);
			break;case Float:
				append_handle(&expr_state.active_nodes,
					append_ast_node(&result, (Ast_Node){
						.type = Node_Array,
						.as.array = make_array(&tok.f64value, 1, Type_Float),
					})
				);

			break;case Identifier: {
				Ast_Node node = {.type = Node_Identifier, .as.identifier= {0}};
				assert(string_len(tok.text) <= 15);
				strncpy(node.as.identifier, tok.text.begin, string_len(tok.text));
				append_handle(&expr_state.active_nodes, append_ast_node(&result, node));
			}

			break;case Operator:
				switch (tok.op) {
					break;case List:
						enlisting = true;

					break;case LParen:
						append_handle(&expr_state.parens, result.count);

					break;case RParen: {
						assert(expr_state.parens.count > 0);
						apply_functions(&expr_state, &result);
						pop_handle(&expr_state.parens);
					}

					break;case Assign: {
						assert(expr_state.parens.count == 0); // must be at toplevel
						assert(expr_state.active_nodes.count == 1); // must have something to bind to
						assert(pop_handle(&expr_state.active_nodes) == 0);
						assert(result.nodes[0].type == Node_Identifier);
						binding_name = 0;
					}

					break;case Dyad: {
						append_ast_node(&result, (Ast_Node){0}); // dummy node for '->'
						append_ast_node(&result, (Ast_Node){0}); // dummy node for 'dyad'
						append_handle(&expr_state.dyad_operators, result.count-2);
					}

					break;case Monad: {
						append_ast_node(&result, (Ast_Node){0}); // dummy node for 'monad'
						append_handle(&expr_state.monad_operators, result.count-1);
					}

					break;default: assert(false && "unhandled operator");
				}
		}
	}

	assert(expr_state.parens.count == 0);
	apply_functions(&expr_state, &result);
	result.parent = expr_state.active_nodes.handles[0];

	if (binding_name >= 0) {
		result.parent = append_ast_node(
				&result, (Ast_Node){
					.type = Node_Assign,
					.as.args = {
						.left = binding_name,
						.right = expr_state.active_nodes.handles[0],
					}
				});
	}

	free(expr_state.active_nodes.handles);
	free(expr_state.parens.handles);
	free(expr_state.dyad_operators.handles);
	free(expr_state.monad_operators.handles);
	return result;
}
