#include "funcs.h"

static Eval_Node func_right(Eval_Context *ctx, Node_Handle left, Node_Handle right) {
    assert(ctx->nodes[right].type == Node_Array);
    return (Eval_Node) {.type = Node_Array, .as.array = borrow_array(ctx->nodes[right].as.array)};
}

static Eval_Node func_left(Eval_Context *ctx, Node_Handle left, Node_Handle right) {
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

func_t dyadic_functions[] = {
    func_left,
    func_plus,
    func_minus,
};

u64 dyadic_function_count = 3;
const char dyadic_function_names[][16] = {
    "<-",
    "+",
    "-",
};

func_t monadic_functions[] = {
    func_right,
};

u64 monadic_function_count = 1;
const char monadic_function_names[][16] = {
    "->",
};
