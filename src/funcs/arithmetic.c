#include <math.h>
#include "funcs.h"

Element_Type supertype(Element_Type a, Element_Type b) {
    const static Element_Type supertypes[][Types_Count] = {
        //              Type_Bool   Type_Char   Type_Int    Type_Float
        [Type_Bool ] = {Type_Bool,  Type_Char,  Type_Int,   Type_Float},
        [Type_Char ] = {Type_Char,  Type_Char,  Type_Int,   Type_Float},
        [Type_Int  ] = {Type_Int,   Type_Int,   Type_Int,   Type_Float},
        [Type_Float] = {Type_Float, Type_Float, Type_Float, Type_Float},
    };

    return supertypes[a][b];
}

Eval_Node func_plus(Eval_Context *ctx, Node_Handle left, Node_Handle right) {
    assert(ctx->nodes[left ].type == Node_Array);
    assert(ctx->nodes[right].type == Node_Array);
    Array *left_  = borrow_array(ctx->nodes[left ].as.array);
    Array *right_ = borrow_array(ctx->nodes[right].as.array);
    assert(left_->shape == right_->shape);

    Element_Type natural_type = supertype(left_->type, right_->type);
    if (natural_type == Type_Bool) natural_type = Type_Int;

    left_  = array_cast(left_,  natural_type);
    right_ = array_cast(right_, natural_type);
    if (left_ == ctx->nodes[left].as.array) left_ = clone_array(left_);

    u64 n = left_->shape;
    switch (natural_type) {
        case Type_Char:
            for(i64 i = 0; i < n; i++) ((char*)left_->data)[i] += ((char*)right_->data)[i];
            break;
        case Type_Int:
            for(i64 i = 0; i < n; i++) ((i64*)left_->data)[i] += ((i64*)right_->data)[i];
            break;
        case Type_Float:
            for(i64 i = 0; i < n; i++) ((f64*)left_->data)[i] += ((f64*)right_->data)[i];
            break;
        case Type_Bool:   assert(false);
        case Types_Count: assert(false);
    }

    release_array(right_);
    return (Eval_Node){.type = Node_Array, .as.array = left_};
}

Eval_Node func_minus(Eval_Context *ctx, Node_Handle left, Node_Handle right) {
    assert(ctx->nodes[left ].type == Node_Array);
    assert(ctx->nodes[right].type == Node_Array);
    Array *left_  = borrow_array(ctx->nodes[left ].as.array);
    Array *right_ = borrow_array(ctx->nodes[right].as.array);
    assert(left_->shape == right_->shape);

    Element_Type natural_type = supertype(left_->type, right_->type);
    if (natural_type == Type_Bool) natural_type = Type_Int;

    left_  = array_cast(left_,  natural_type);
    right_ = array_cast(right_, natural_type);
    if (left_ == ctx->nodes[left].as.array) left_ = clone_array(left_);

    u64 n = left_->shape;
    switch (natural_type) {
        case Type_Char:
            for(i64 i = 0; i < n; i++) ((char*)left_->data)[i] -= ((char*)right_->data)[i];
            break;
        case Type_Int:
            for(i64 i = 0; i < n; i++) ((i64*)left_->data)[i] -= ((i64*)right_->data)[i];
            break;
        case Type_Float:
            for(i64 i = 0; i < n; i++) ((f64*)left_->data)[i] -= ((f64*)right_->data)[i];
            break;
        case Type_Bool:   assert(false);
        case Types_Count: assert(false);
    }

    release_array(right_);
    return (Eval_Node){.type = Node_Array, .as.array = left_};
}


Eval_Node func_multiply(Eval_Context *ctx, Node_Handle left, Node_Handle right) {
    assert(ctx->nodes[left ].type == Node_Array);
    assert(ctx->nodes[right].type == Node_Array);
    Array *left_  = borrow_array(ctx->nodes[left ].as.array);
    Array *right_ = borrow_array(ctx->nodes[right].as.array);
    assert(left_->shape == right_->shape);

    Element_Type natural_type = supertype(left_->type, right_->type);

    left_  = array_cast(left_,  natural_type);
    right_ = array_cast(right_, natural_type);
    if (left_ == ctx->nodes[left].as.array) left_ = clone_array(left_);

    u64 n = left_->shape;
    switch (natural_type) {
        case Type_Char:
            for(i64 i = 0; i < n; i++) ((char*)left_->data)[i] *= ((char*)right_->data)[i];
            break;
        case Type_Int:
            for(i64 i = 0; i < n; i++) ((i64*)left_->data)[i] *= ((i64*)right_->data)[i];
            break;
        case Type_Float:
            for(i64 i = 0; i < n; i++) ((f64*)left_->data)[i] *= ((f64*)right_->data)[i];
            break;
        case Type_Bool:
            for(i64 i = 0; i < n; i++) ((bool*)left_->data)[i] &= ((bool*)right_->data)[i];
            break;
        case Types_Count: assert(false);
    }

    release_array(right_);
    return (Eval_Node){.type = Node_Array, .as.array = left_};
}

Eval_Node func_divide(Eval_Context *ctx, Node_Handle left, Node_Handle right) {
    assert(ctx->nodes[left ].type == Node_Array);
    assert(ctx->nodes[right].type == Node_Array);
    Array *left_  = borrow_array(ctx->nodes[left ].as.array);
    Array *right_ = borrow_array(ctx->nodes[right].as.array);
    assert(left_->shape == right_->shape);

    Element_Type natural_type = supertype(left_->type, right_->type);

    left_  = array_cast(left_,  natural_type);
    right_ = array_cast(right_, natural_type);
    if (left_ == ctx->nodes[left].as.array) left_ = clone_array(left_);

    u64 n = left_->shape;
    switch (natural_type) {
        case Type_Char:
            for(i64 i = 0; i < n; i++) ((char*)left_->data)[i] /= ((char*)right_->data)[i];
            break;
        case Type_Int:
            for(i64 i = 0; i < n; i++) ((i64*)left_->data)[i] /= ((i64*)right_->data)[i];
            break;
        case Type_Float:
            for(i64 i = 0; i < n; i++) ((f64*)left_->data)[i] /= ((f64*)right_->data)[i];
            break;
        case Type_Bool: assert(false);
        case Types_Count: assert(false);
    }

    release_array(right_);
    return (Eval_Node){.type = Node_Array, .as.array = left_};
}

Eval_Node func_mismatch (Eval_Context *ctx, Node_Handle left, Node_Handle right) {
    assert(ctx->nodes[left ].type == Node_Array);
    assert(ctx->nodes[right].type == Node_Array);
    Array *left_  = borrow_array(ctx->nodes[left ].as.array);
    Array *right_ = borrow_array(ctx->nodes[right].as.array);
    assert(left_->shape == right_->shape);

    Element_Type natural_type = supertype(left_->type, right_->type);

    left_  = array_cast(left_,  natural_type);
    right_ = array_cast(right_, natural_type);

    Array *result = make_array(NULL, left_->shape, Type_Bool);

    u64 n = result->shape;
    switch (natural_type) {
        case Type_Char:
            for(i64 i = 0; i < n; i++) ((bool*)result->data)[i] = ((char*)left_->data)[i] != ((char*)right_->data)[i];
            break;
        case Type_Int:
            for(i64 i = 0; i < n; i++) ((bool*)result->data)[i] = ((i64*)left_->data)[i] != ((i64*)right_->data)[i];
            break;
        case Type_Float:
            for(i64 i = 0; i < n; i++) ((bool*)result->data)[i] = ((f64*)left_->data)[i] != ((f64*)right_->data)[i];
            break;
        case Type_Bool:
            for(i64 i = 0; i < n; i++) ((bool*)result->data)[i] = ((bool*)left_->data)[i] != ((bool*)right_->data)[i];
            break;
        case Types_Count: assert(false);
    }

    release_array(left_);
    release_array(right_);
    return (Eval_Node){.type = Node_Array, .as.array = result};
}


Eval_Node func_equal (Eval_Context *ctx, Node_Handle left, Node_Handle right) {
    assert(ctx->nodes[left ].type == Node_Array);
    assert(ctx->nodes[right].type == Node_Array);
    Array *left_  = borrow_array(ctx->nodes[left ].as.array);
    Array *right_ = borrow_array(ctx->nodes[right].as.array);
    assert(left_->shape == right_->shape);

    Element_Type natural_type = supertype(left_->type, right_->type);

    left_  = array_cast(left_,  natural_type);
    right_ = array_cast(right_, natural_type);

    Array *result = make_array(NULL, left_->shape, Type_Bool);

    u64 n = result->shape;
    switch (natural_type) {
        case Type_Char:
            for(i64 i = 0; i < n; i++) ((bool*)result->data)[i] = ((char*)left_->data)[i] == ((char*)right_->data)[i];
            break;
        case Type_Int:
            for(i64 i = 0; i < n; i++) ((bool*)result->data)[i] = ((i64*)left_->data)[i] == ((i64*)right_->data)[i];
            break;
        case Type_Float:
            for(i64 i = 0; i < n; i++) ((bool*)result->data)[i] = ((f64*)left_->data)[i] == ((f64*)right_->data)[i];
            break;
        case Type_Bool:
            for(i64 i = 0; i < n; i++) ((bool*)result->data)[i] = ((bool*)left_->data)[i] == ((bool*)right_->data)[i];
            break;
        case Types_Count: assert(false);
    }

    release_array(left_);
    release_array(right_);
    return (Eval_Node){.type = Node_Array, .as.array = result};
}

Eval_Node func_negate (Eval_Context *ctx, Node_Handle left, Node_Handle right) {
    assert(ctx->nodes[right].type == Node_Array);
    Array *right_ = borrow_array(ctx->nodes[right].as.array);

    Element_Type natural_type = right_->type == Type_Float ? Type_Float : Type_Int;
    right_ = array_cast(right_, natural_type);
    if (right_ == ctx->nodes[right].as.array) right_ = clone_array(right_);

    u64 n = right_->shape;
    switch (natural_type) {
        case Type_Int:
            for(i64 i = 0; i < n; i++) ((i64*)right_->data)[i] = -((i64*)right_->data)[i];
            break;
        case Type_Float:
            for(i64 i = 0; i < n; i++) ((f64*)right_->data)[i] = -((f64*)right_->data)[i];
            break;
        default: assert(false);
    }

    return (Eval_Node){.type = Node_Array, .as.array = right_};
}

Eval_Node func_complement (Eval_Context *ctx, Node_Handle left, Node_Handle right) {
    assert(ctx->nodes[right].type == Node_Array);
    Array *right_ = borrow_array(ctx->nodes[right].as.array);

    Element_Type natural_type = right_->type == Type_Float ? Type_Float : Type_Int;
    right_ = array_cast(right_, natural_type);
    if (right_ == ctx->nodes[right].as.array) right_ = clone_array(right_);

    u64 n = right_->shape;
    switch (natural_type) {
        case Type_Int:
            for(i64 i = 0; i < n; i++) ((i64*)right_->data)[i] = 1-((i64*)right_->data)[i];
            break;
        case Type_Float:
            for(i64 i = 0; i < n; i++) ((f64*)right_->data)[i] = 1-((f64*)right_->data)[i];
            break;
        default: assert(false);
    }

    return (Eval_Node){.type = Node_Array, .as.array = right_};
}

Eval_Node func_square_root(Eval_Context *ctx, Node_Handle left, Node_Handle right) {
    assert(ctx->nodes[right].type == Node_Array);
    Array *right_ = borrow_array(ctx->nodes[right].as.array);

    Element_Type natural_type = Type_Float;
    right_ = array_cast(right_, natural_type);
    if (right_ == ctx->nodes[right].as.array) right_ = clone_array(right_);

    u64 n = right_->shape;
    switch (natural_type) {
        case Type_Float:
            for(i64 i = 0; i < n; i++) ((f64*)right_->data)[i] = sqrt(((f64*)right_->data)[i]);
            break;
        default: assert(false);
    }

    return (Eval_Node){.type = Node_Array, .as.array = right_};
}
