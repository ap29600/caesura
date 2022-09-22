#include "funcs.h"

const static u64 sizes[] = {
    [Type_Float] = sizeof(f64),
    [Type_Char]  = sizeof(char),
    [Type_Int]   = sizeof(i64),
    [Type_Bool]  = sizeof(bool),
};

Eval_Node func_filter     (Eval_Context *ctx, Node_Handle left, Node_Handle right) {
    assert(ctx->nodes[left ].type == Node_Array);
    assert(ctx->nodes[right].type == Node_Array);
    Array *left_  = borrow_array(ctx->nodes[left ].as.array);
    Array *right_ = borrow_array(ctx->nodes[right].as.array);
    assert(left_->shape == right_->shape);
    left_ = array_cast(left_, Type_Bool);

    u64 i = 0;
    for (u64 j = 0; j < left_->shape; ++j) if (((bool*)left_->data)[j]) ++i;
    Array *result = make_array(NULL, i, right_->type);

    u64 size = sizes[right_->type];
    for(u64 j = 0, i = 0; j < right_->shape; ++j) {
        if (((bool*)left_->data)[j]) {
            memcpy(((u8*)result->data) + i * size, ((u8*)right_->data) + j * size, size);
            ++i;
        }
    }

    release_array(left_);
    release_array(right_);

    return (Eval_Node){ .type = Node_Array, .as.array = result};
}

Eval_Node func_reshape (Eval_Context *ctx, Node_Handle left, Node_Handle right) {
    assert(ctx->nodes[left].type == Node_Array);
    assert(ctx->nodes[right].type == Node_Array);
    Array *left_  = borrow_array(ctx->nodes[left ].as.array);
    Array *right_ = borrow_array(ctx->nodes[right].as.array);
    assert(left_->shape == 1);
    left_ = array_cast(left_, Type_Int);
    Array *result = make_array(NULL, ((i64*)left_->data)[0], right_->type);
    u64 count = 0;
    u64 size = sizes[right_->type];

    while (count < result->shape) {
        u64 to_copy = min(result->shape - count, right_->shape);
        memcpy(((u8*)result->data) + count * size, right_->data, to_copy * size);
        count += to_copy;
    }
    
    release_array(right_);
    release_array(left_);
    return (Eval_Node){ .type = Node_Array, .as.array = result};
}

Eval_Node func_shape (Eval_Context *ctx, Node_Handle left, Node_Handle right) {
    assert(ctx->nodes[right].type == Node_Array);
    Array *right_ = borrow_array(ctx->nodes[right].as.array);
    Array *result = make_array(NULL, 1, Type_Int);
    ((i64*)result->data)[0] = right_->shape;
    release_array(right_);
    return (Eval_Node){ .type = Node_Array, .as.array = result};
}
