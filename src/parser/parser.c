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
    Node_Handle *active_nodes;
    u64 active_nodes_count;

    Node_Handle *parens;
    u64 parens_count;
} Expression_Parser_State;


static bool has_active_locals(Expression_Parser_State *state, u64 count) {
    if (state->active_nodes_count < count) return false;
    if (state->parens_count == 0) return true;
    return state->active_nodes[state->active_nodes_count - count] >= state->parens[state->parens_count - 1];
}

static Node_Handle last_active_local(Expression_Parser_State *state) {
    if (has_active_locals(state, 1)) {
        return state->active_nodes[state->active_nodes_count - 1];
    }
    return -1;
}

static Node_Handle pop_active_local(Expression_Parser_State *state) {
    if (has_active_locals(state, 1)) {
        return state->active_nodes[-- state->active_nodes_count];
    }
    return -1;
}

static void apply_functions(Expression_Parser_State *state, Ast *result) {
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

        state->active_nodes[state->active_nodes_count ++] = val;
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

        state->active_nodes[state->active_nodes_count ++] = val;
    }
}

Ast parse_expressions(Scanner *state) {
    Ast result = {.nodes = malloc(sizeof(Ast_Node) * 8), .cap = 8, .count = 0};

    Expression_Parser_State expr_state = {
        .active_nodes = (Node_Handle[1000]){0},
        .parens = (Node_Handle[1000]){0},
    };

    bool enlisting = false;
    Node_Handle binding_name = -1;

    for (; !scanner_is_empty(state); ) {
        Token tok = next_token(state);

        if (!tok.is_valid) {
            report_state(state, stdout);
            exit(1);
        }

        if (enlisting) {
            assert(tok.type == Float);
            assert(has_active_locals(&expr_state, 1));
            Ast_Node *node = &result.nodes[last_active_local(&expr_state)];
            assert(node->type == Node_Array);
            node->as.array = array_append_elem(&tok.value, node->as.array, Type_Float);
            enlisting = false;
            continue;
        }

        switch(tok.type) {
            case Empty:
                break;

            case Float:
                expr_state.active_nodes[expr_state.active_nodes_count++] = append_ast_node(
                        &result, (Ast_Node){
                            .type = Node_Array,
                            .as.array = make_array(&tok.value, 1, Type_Float),
                        });
                break;

            case Identifier: {
                Ast_Node node = {.type = Node_Identifier, .as.identifier= {0}};
                assert(string_len(tok.text) <= 15);
                strncpy(node.as.identifier, tok.text.begin, string_len(tok.text));
                expr_state.active_nodes[expr_state.active_nodes_count++] = append_ast_node(&result, node);
            } break;

            case Operator:
                switch (tok.op) {
                    case List:
                        enlisting = true;
                        break;

                    case LParen:
                        expr_state.parens[expr_state.parens_count++] = result.count;
                        break;

                    case RParen: {
                        assert(expr_state.parens_count > 0);
                        apply_functions(&expr_state, &result);
                        expr_state.parens_count--;
                    } break;

                    case Assign: {
                        assert(expr_state.parens_count == 0); // must be at toplevel
                        assert(expr_state.active_nodes_count == 1); // must have something to bind to
                        expr_state.active_nodes_count--;
                        assert(result.count == 1); // bind to a single variable
                        assert(result.nodes[0].type == Node_Identifier);
                        binding_name = 0;
                    } break;

                    default: assert(false && "unhandled operator");
                } break;
        }
    }

    assert(expr_state.parens_count == 0);
    apply_functions(&expr_state, &result);
    result.parent = expr_state.active_nodes[0];

    if (binding_name >= 0) {
        result.parent = append_ast_node(
                &result, (Ast_Node){
                    .type = Node_Assign,
                    .as.args = {
                        .left = binding_name,
                        .right = expr_state.active_nodes[0],
                    }
                });
    }

    return result;
}
