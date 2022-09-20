#include <stdlib.h>
#include "lib/bit_set.h"

#include "eval.h"
#include "src/funcs/funcs.h"

i64 fmt_expression_va(Byte_Slice dest, va_list va, Fmt_Info info) {
    Ast_Node src = va_arg(va, Ast_Node);
    return fmt_expression(dest, src, info);
}

i32 entry_cmp (const void* a, const void*b) {
    const Lookup_Entry *aa = a;
    const Lookup_Entry *bb = b;
    return strncmp(aa->name, bb->name, 16);
}

void scope_insert(Lookup_Scope *scope, Lookup_Entry entry) {
    if (scope->count + 1 >= scope->cap) {
        scope->cap = scope->cap ? scope->cap * 2 : 8;
        scope->entries = realloc(scope->entries, scope->cap * sizeof(Lookup_Entry));
    }

    u64 i = scope->count++;
    for (; i > 0 && entry_cmp(&entry, &scope->entries[i-1]) < 0; i--) {
        scope->entries[i] = scope->entries[i-1];
    }
    scope->entries[i] = entry;
}


Lookup_Entry *scope_lookup(Lookup_Scope *scope, const char name[16]) {
    Lookup_Entry it = {0};
    strncpy(it.name, name, 16);
    return bsearch(&it, scope->entries, scope->count, sizeof(Lookup_Entry), entry_cmp);
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

                Lookup_Entry *entry = scope_lookup(ctx->scope, base[expr].as.identifier);
                if (!entry) {
                    format_println("Error: `{cstr}` not found", base[expr].as.identifier);
                    fflush(stdout);
                    assert(false && "lookup failed");
                }

                if (left >= 0 && right >= 0) {
                    func_t func = entry->as_dyadic;
                    if (func) {
                        Node_Handle func_node = append_node(ctx, (Eval_Node){.type = Node_Function, .as.function = func});
                        ctx->nodes[left     ].ref_count += 1;
                        ctx->nodes[func_node].ref_count += 1;
                        ctx->nodes[right    ].ref_count += 1;
                        return append_node(ctx, (Eval_Node){.type = Node_Dyad, .as.args = {.left = left, .callee = func_node, .right = right}});
                    }
                }

                if (right >= 0) {
                    func_t func = entry->as_monadic;
                    if (func) {
                        Node_Handle func_node = append_node(ctx, (Eval_Node){.type = Node_Function, .as.function = func});
                        ctx->nodes[func_node].ref_count += 1;
                        ctx->nodes[right    ].ref_count += 1;
                        return append_node(ctx, (Eval_Node){.type = Node_Monad, .as.args = {.callee = func_node, .right = right}});
                    }
                }

                Eval_Node value = entry->value;
                assert(value.type == Node_Array);
                entry->value.as.array->ref_count++;
                return append_node(ctx, value);
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

            case Node_Assign: {

                Node_Handle bind_name  = base[expr].as.args.left;
                Node_Handle bind_value = base[expr].as.args.right;
                format_println("register `{cstr}`", base[bind_name].as.identifier);

                Eval_Context new_ctx = {.scope = ctx->scope};

                Node_Handle formal_left_arg = append_node(&new_ctx, (Eval_Node){.type = Node_None});
                Node_Handle formal_right_arg = append_node(&new_ctx, (Eval_Node){.type = Node_None});

                bind_value = apply_with(&new_ctx, base, bind_value, formal_left_arg, formal_right_arg);

                Lookup_Entry entry = {0};
                strncpy(entry.name, base[bind_name].as.identifier, 15);

                if ( new_ctx.nodes[formal_left_arg].ref_count == 0
                  && new_ctx.nodes[formal_right_arg].ref_count == 0 ) {
                    bind_value = eval(&new_ctx, bind_value);

                    entry.value = new_ctx.nodes[bind_value];
                    entry.value.as.array->ref_count++;

                    scope_insert(ctx->scope, entry);

                    free(new_ctx.nodes);
                    return append_node(ctx, entry.value);

                } else {
                    assert(false);
                    entry.eval_tree.nodes = realloc(new_ctx.nodes, sizeof(Eval_Node) * new_ctx.count);
                    entry.eval_tree.count = new_ctx.count;

                    assert(false && "assigning functions is not implemented yet");
                }
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

        case Node_Assign: {
            assert(false && "assign unhandled in application");
        }
    }
}
