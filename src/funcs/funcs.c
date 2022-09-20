#include "funcs.h"

#include <stdlib.h>
#include <math.h>

#define BINARY_OP_ONCE(a, b, i, op, ty) ((ty *)a)[i] = ((ty *)a)[i] op ((ty *)b)[i]

#define BINARY_OP(name, op)                                                                  \
  static Eval_Node name(Eval_Context *ctx, Node_Handle left, Node_Handle right) {            \
      assert(ctx->nodes[left].type == Node_Array);                                           \
      assert(ctx->nodes[right].type == Node_Array);                                          \
      Array *left_arr = borrow_array(ctx->nodes[left].as.array);                             \
      Array *right_arr = borrow_array(ctx->nodes[right].as.array);                           \
      assert(left_arr->shape == right_arr->shape);                                           \
      assert(left_arr->type == right_arr->type);                                             \
      Array *const result = clone_array(left_arr);                                           \
      const u64 n = result->shape;                                                           \
      void *const a = result->data;                                                          \
      const void *const b = right_arr->data;                                                 \
      switch (left_arr->type) {                                                              \
          case Type_Float: for(i64 i = 0; i < n; i++) BINARY_OP_ONCE(a, b, i, op, f64); break;  \
          case Type_Int:   for(i64 i = 0; i < n; i++) BINARY_OP_ONCE(a, b, i, op, i64); break;  \
          case Type_UInt:  for(i64 i = 0; i < n; i++) BINARY_OP_ONCE(a, b, i, op, u64); break;  \
          case Type_Char:  for(i64 i = 0; i < n; i++) BINARY_OP_ONCE(a, b, i, op, char); break; \
          case Type_Bool:  for(i64 i = 0; i < n; i++) BINARY_OP_ONCE(a, b, i, op, bool); break; \
          default: assert(false);                                                            \
      }                                                                                      \
      release_array(left_arr);                                                               \
      release_array(right_arr);                                                              \
      return (Eval_Node){.type = Node_Array, .as.array = result};                            \
  }

#define UNARY_OP_ONCE(a, i, op, ty) ((ty *)a)[i] = op (((ty *)a)[i])

#define UNARY_OP(name, op)                                                                   \
  static Eval_Node name (Eval_Context *ctx, Node_Handle left, Node_Handle right) {           \
      assert(ctx->nodes[right].type == Node_Array);                                          \
      Array *result = clone_array(ctx->nodes[right].as.array);                               \
      const u64 n = result->shape;                                                           \
      void *const a = result->data;                                                          \
      switch (result->type) {                                                                \
          case Type_Float: for(i64 i = 0; i < n; i++) UNARY_OP_ONCE(a, i, op, f64); break;      \
          case Type_Int:   for(i64 i = 0; i < n; i++) UNARY_OP_ONCE(a, i, op, i64); break;      \
          case Type_UInt:  for(i64 i = 0; i < n; i++) UNARY_OP_ONCE(a, i, op, u64); break;      \
          case Type_Char:  for(i64 i = 0; i < n; i++) UNARY_OP_ONCE(a, i, op, char); break;     \
          case Type_Bool:  for(i64 i = 0; i < n; i++) UNARY_OP_ONCE(a, i, op, bool); break;     \
          default: assert(false);                                                            \
      }                                                                                      \
      return (Eval_Node){.type = Node_Array, .as.array = result};                            \
  }                                                                                          \


static Eval_Node func_right_identity(Eval_Context *ctx, Node_Handle left, Node_Handle right) {
    assert(ctx->nodes[right].type == Node_Array);
    return (Eval_Node) {.type = Node_Array, .as.array = borrow_array(ctx->nodes[right].as.array)};
}

static Eval_Node func_left_identity(Eval_Context *ctx, Node_Handle left, Node_Handle right) {
    assert(ctx->nodes[left].type == Node_Array);
    return (Eval_Node) {.type = Node_Array, .as.array = borrow_array(ctx->nodes[left].as.array)};
}

UNARY_OP(func_complement,     1-  )
UNARY_OP(func_negate,         -   )
UNARY_OP(func_absolute_value, fabs)
UNARY_OP(func_square_root,    sqrt)

BINARY_OP(func_equal,    ==)
BINARY_OP(func_mismatch, !=)
BINARY_OP(func_minus,    - )
BINARY_OP(func_plus,     + )
BINARY_OP(func_multiply, * )
BINARY_OP(func_divide,   / )

void init_default_scope() {
     scope_insert(&default_scope, (Lookup_Entry){ .name = "->", .as_monadic = func_right_identity, .as_dyadic = NULL               });
     scope_insert(&default_scope, (Lookup_Entry){ .name = "<-", .as_monadic = NULL,                .as_dyadic = func_left_identity });
     scope_insert(&default_scope, (Lookup_Entry){ .name = "||", .as_monadic = func_absolute_value, .as_dyadic = NULL               });
     scope_insert(&default_scope, (Lookup_Entry){ .name = "+",  .as_monadic = func_right_identity, .as_dyadic = func_plus          });
     scope_insert(&default_scope, (Lookup_Entry){ .name = "-",  .as_monadic = func_negate,         .as_dyadic = func_minus         });
     scope_insert(&default_scope, (Lookup_Entry){ .name = "*",  .as_monadic = NULL,                .as_dyadic = func_multiply      });
     scope_insert(&default_scope, (Lookup_Entry){ .name = "/",  .as_monadic = func_square_root,    .as_dyadic = func_divide        });
     scope_insert(&default_scope, (Lookup_Entry){ .name = "~",  .as_monadic = func_complement,     .as_dyadic = func_mismatch      });
     scope_insert(&default_scope, (Lookup_Entry){ .name = "=",  .as_monadic = NULL,                .as_dyadic = func_equal         });
}

Lookup_Scope default_scope = {0};
