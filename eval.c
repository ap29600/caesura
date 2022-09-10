#include "lib/bit_set.h"

#include "eval.h"
#include "funcs.h"

#include <stdlib.h>

i64 fmt_expression(Byte_Slice dest, Ast_Node src, Fmt_Info info) {
    const Ast_Node *ctx = info.user_ptr;
    const char *begin = dest.begin;
    switch (src.type) {
        case Node_None:
            dest.begin += fmt_cstr(dest, "<No_Expression>", info);
            break;

        case Node_Monad:
            dest.begin += fmt_rune(dest, '(', info);
            dest.begin += fmt_expression(dest, ctx[src.as.args.callee], info);
            dest.begin += fmt_rune(dest, ' ', info);
            dest.begin += fmt_expression(dest, ctx[src.as.args.right], info);
            dest.begin += fmt_rune(dest, ')', info);
            break;

        case Node_Dyad:
            dest.begin += fmt_rune(dest, '(', info);
            dest.begin += fmt_expression(dest, ctx[src.as.args.left], info);
            dest.begin += fmt_rune(dest, ' ', info);
            dest.begin += fmt_expression(dest, ctx[src.as.args.callee], info);
            dest.begin += fmt_rune(dest, ' ', info);
            dest.begin += fmt_expression(dest, ctx[src.as.args.right], info);
            dest.begin += fmt_rune(dest, ')', info);
            break;

        case Node_Function:
            assert(false);

        case Node_Identifier:
            dest.begin += fmt_cstr(dest, src.as.identifier, info);
            break;

        case Node_Array:
            for (u64 i = 0; i < src.as.array->shape; ++i) {
                if (i > 0) {
                    dest.begin += fmt_rune(dest, ',', info);
                }
                dest.begin += fmt_i64(dest, (i64)src.as.array->data[i], info);
            }
            break;
    }

    return dest.begin - begin;
}

i64 fmt_expression_va(Byte_Slice dest, va_list va, Fmt_Info info) {
    Ast_Node src = va_arg(va, Ast_Node);
    return fmt_expression(dest, src, info);
}

static func_t lookup_function(const char name[16], const char (*names)[16], func_t *functions, u64 count) {
    for(u64 i = 0; i < count; ++i) {
        if (strncmp(name, names[i], 16) == 0) {
            return functions[i];
        }
    }

    return NULL;
}

static Node_Handle append_node(Eval_Context *ctx, Eval_Node node) {
    if (ctx->count >= ctx->cap) {
        ctx->cap = ctx->cap == 0 ? 8 : (ctx->cap * 2);
        ctx->nodes = realloc(ctx->nodes, ctx->cap * sizeof(Eval_Node));
    }

    ctx->nodes[ctx->count] = node;
    return ctx->count++;
}

void release_node(Eval_Context *ctx, Node_Handle expr) {
    Eval_Node*node = &ctx->nodes[expr];
    node->ref_count--;
    if (node->ref_count == 0) {
        if (node->type == Node_Array) release_array(node->as.array);
        *node = (Eval_Node){.type = Node_None};
    }
}

Node_Handle apply_with(Eval_Context *ctx, const Ast_Node *base, Node_Handle expr, Node_Handle left, Node_Handle right) {
    // outer loop for TCO
    // TODO: use a stack for recursion
    while (true) {
        switch (base[expr].type) {
            case Node_None:
                assert(false);

            case Node_Array:
                return append_node(ctx, (Eval_Node){ .type = Node_Array, .as.array = borrow_array(base[expr].as.array) });

            case Node_Function:
                assert(false);

            case Node_Identifier: {

                if (left >= 0 && right >= 0) {
                    // TODO: make this lookup in a scope
                    func_t func = lookup_function(base[expr].as.identifier,
                                                  dyadic_function_names,
                                                  dyadic_functions,
                                                  dyadic_function_count);

                    if (func) {
                        Node_Handle func_node = append_node(ctx, (Eval_Node){.type = Node_Function, .as.function = func});
                        ctx->nodes[left     ].ref_count += 1;
                        ctx->nodes[func_node].ref_count += 1;
                        ctx->nodes[right    ].ref_count += 1;
                        return append_node(ctx, (Eval_Node){.type = Node_Dyad, .as.args = {.left = left, .callee = func_node, .right = right}});
                    }
                }

                if (right >= 0) {
                    // TODO: make this lookup in a scope
                    func_t func = lookup_function(base[expr].as.identifier,
                                                  monadic_function_names,
                                                  monadic_functions,
                                                  monadic_function_count);

                    if (func) {
                        Node_Handle func_node = append_node(ctx, (Eval_Node){.type = Node_Function, .as.function = func});
                        ctx->nodes[func_node].ref_count += 1;
                        ctx->nodes[right    ].ref_count += 1;
                        return append_node(ctx, (Eval_Node){.type = Node_Monad, .as.args = {.callee = func_node, .right = right}});
                    }
                }

                assert(false && "Unresolved symbol");
            } break;

            case Node_Monad: {
                right = apply_with(ctx, base, base[expr].as.args.right, left, right);
                left = -1;
                expr = base[expr].as.args.callee;
                continue;
            }

            case Node_Dyad: {
                Node_Handle new_left  = apply_with(ctx, base, base[expr].as.args.left,  left, right);
                Node_Handle new_right = apply_with(ctx, base, base[expr].as.args.right, left, right);
                left  = new_left;
                right = new_right;
                expr  = base[expr].as.args.callee;
                continue;
            }
        }
    }
}

Node_Handle apply(Eval_Context *ctx, const Ast_Node *base, Node_Handle expr) {
    Node_Handle result = apply_with(ctx, base, expr, -1, -1);
    ctx->nodes[result].ref_count += 1;
    return result;
}


Node_Handle eval(Eval_Context *ctx, Node_Handle expr) {
    switch(ctx->nodes[expr].type) {
        case Node_None:

        case Node_Array:
            return expr;

        case Node_Function:
            return expr;

        case Node_Identifier:
            assert(false && "Unhandled identifier in eval");

        case Node_Monad: {
            Node_Handle func_handle  = eval(ctx, ctx->nodes[expr].as.args.callee);
            assert(ctx->nodes[func_handle].type == Node_Function);
            Node_Handle right_handle = eval(ctx, ctx->nodes[expr].as.args.right);
            u64 old_ref_count = ctx->nodes[expr].ref_count;
            ctx->nodes[expr] = ctx->nodes[func_handle].as.function(ctx, -1, right_handle);
            ctx->nodes[expr].ref_count = old_ref_count;
            release_node(ctx, func_handle);
            release_node(ctx, right_handle);
            return expr;
        }

        case Node_Dyad: {
            Node_Handle func_handle = eval(ctx, ctx->nodes[expr].as.args.callee);
            assert(ctx->nodes[func_handle].type == Node_Function);
            Node_Handle left_handle  = eval(ctx, ctx->nodes[expr].as.args.left);
            Node_Handle right_handle = eval(ctx, ctx->nodes[expr].as.args.right);
            u64 old_ref_count = ctx->nodes[expr].ref_count;
            ctx->nodes[expr] = ctx->nodes[func_handle].as.function(ctx, left_handle, right_handle);
            ctx->nodes[expr].ref_count = old_ref_count;
            release_node(ctx, left_handle);
            release_node(ctx, func_handle);
            release_node(ctx, right_handle);
            return expr;
        }
    }
}
