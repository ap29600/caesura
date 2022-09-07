#include "funcs.h"

static Eval_Node func_right(Eval_Context *ctx, Node_Handle left, Node_Handle right) {
    assert(ctx->nodes[right].type == Node_Array);
    return (Eval_Node) {.type = Node_Array, .as.array = new_array(
            ctx->nodes[right].as.array->shape,
            ctx->nodes[right].as.array->data
        )};
}

static Eval_Node func_left(Eval_Context *ctx, Node_Handle left, Node_Handle right) {
    assert(ctx->nodes[left].type == Node_Array);
    return (Eval_Node) {.type = Node_Array, .as.array = new_array(
            ctx->nodes[left].as.array->shape,
            ctx->nodes[left].as.array->data
        )};
}

static Eval_Node func_plus(Eval_Context *ctx, Node_Handle left, Node_Handle right) {
    assert(ctx->nodes[left].type == Node_Array);
    assert(ctx->nodes[right].type == Node_Array);
    assert(ctx->nodes[right].as.array->shape == ctx->nodes[right].as.array->shape);

    double data[ctx->nodes[left].as.array->shape];

    for(i64 i = 0; i < ctx->nodes[left].as.array->shape; i++) {
        data[i] = ctx->nodes[left].as.array->data[i] + ctx->nodes[right].as.array->data[i];
    }

    Array_t result = new_array(ctx->nodes[left].as.array->shape, data);

    return (Eval_Node){.type = Node_Array, .as.array = result };
}

static Eval_Node func_minus (Eval_Context *ctx, Node_Handle left, Node_Handle right) {
    assert(ctx->nodes[left].type == Node_Array);
    assert(ctx->nodes[right].type == Node_Array);
    assert(ctx->nodes[right].as.array->shape == ctx->nodes[right].as.array->shape);

    double data[ctx->nodes[left].as.array->shape];

    for(i64 i = 0; i < ctx->nodes[left].as.array->shape; i++) {
        data[i] = ctx->nodes[left].as.array->data[i] - ctx->nodes[right].as.array->data[i];
    }

    Array_t result = new_array(ctx->nodes[left].as.array->shape, data);

    return (Eval_Node){.type = Node_Array, .as.array = result };
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
