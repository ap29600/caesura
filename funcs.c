#include "funcs.h"

#include <stdlib.h>

static Eval_Node func_right_identity(Eval_Context *ctx, Node_Handle left, Node_Handle right) {
    assert(ctx->nodes[right].type == Node_Array);
    return (Eval_Node) {.type = Node_Array, .as.array = borrow_array(ctx->nodes[right].as.array)};
}

static Eval_Node func_left_identity(Eval_Context *ctx, Node_Handle left, Node_Handle right) {
    assert(ctx->nodes[left].type == Node_Array);
    return (Eval_Node) {.type = Node_Array, .as.array = borrow_array(ctx->nodes[left].as.array)};
}

static Eval_Node func_plus(Eval_Context *ctx, Node_Handle left, Node_Handle right) {
    assert(ctx->nodes[left].type == Node_Array);
    assert(ctx->nodes[right].type == Node_Array);
    Array *left_arr = borrow_array(ctx->nodes[left].as.array);
    Array *right_arr = borrow_array(ctx->nodes[right].as.array);
    assert(left_arr->shape == right_arr->shape);

    Array *result = clone_array(left_arr);
    for(i64 i = 0; i < left_arr->shape; i++) {
        result->data[i] += right_arr->data[i];
    }

    release_array(left_arr);
    release_array(right_arr);

    return (Eval_Node){.type = Node_Array, .as.array = result};
}

static Eval_Node func_minus (Eval_Context *ctx, Node_Handle left, Node_Handle right) {
    assert(ctx->nodes[left].type == Node_Array);
    assert(ctx->nodes[right].type == Node_Array);
    Array *left_arr = borrow_array(ctx->nodes[left].as.array);
    Array *right_arr = borrow_array(ctx->nodes[right].as.array);
    assert(left_arr->shape == right_arr->shape);

    Array *result = clone_array(left_arr);
    for(i64 i = 0; i < left_arr->shape; i++) {
        result->data[i] -= right_arr->data[i];
    }

    release_array(left_arr);
    release_array(right_arr);

    return (Eval_Node){.type = Node_Array, .as.array = result};
}

static Eval_Node func_negate (Eval_Context *ctx, Node_Handle left, Node_Handle right) {
    assert(ctx->nodes[right].type == Node_Array);
    Array *result = clone_array(ctx->nodes[right].as.array);

    for(i64 i = 0; i < result->shape; i++) {
        result->data[i] *= -1;
    }

    return (Eval_Node){.type = Node_Array, .as.array = result};
}

void init_default_scope() {
     scope_insert(&default_scope, (Lookup_Entry){ .name = "->", .as_monadic = func_right_identity, .as_dyadic = NULL               });
     scope_insert(&default_scope, (Lookup_Entry){ .name = "<-", .as_monadic = NULL,                .as_dyadic = func_left_identity });
     scope_insert(&default_scope, (Lookup_Entry){ .name = "+",  .as_monadic = func_right_identity, .as_dyadic = func_plus          });
     scope_insert(&default_scope, (Lookup_Entry){ .name = "-",  .as_monadic = func_negate,         .as_dyadic = func_minus         });
}

Lookup_Scope default_scope = {0};
