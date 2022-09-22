#ifndef FUNCS_H
#define FUNCS_H

#include "src/eval/eval.h"

void init_default_scope();
extern Lookup_Scope default_scope;

Eval_Node func_plus       (Eval_Context *ctx, Node_Handle left, Node_Handle right);
Eval_Node func_minus      (Eval_Context *ctx, Node_Handle left, Node_Handle right);
Eval_Node func_multiply   (Eval_Context *ctx, Node_Handle left, Node_Handle right);
Eval_Node func_divide     (Eval_Context *ctx, Node_Handle left, Node_Handle right);
Eval_Node func_mismatch   (Eval_Context *ctx, Node_Handle left, Node_Handle right);
Eval_Node func_equal      (Eval_Context *ctx, Node_Handle left, Node_Handle right);
Eval_Node func_negate     (Eval_Context *ctx, Node_Handle left, Node_Handle right);
Eval_Node func_complement (Eval_Context *ctx, Node_Handle left, Node_Handle right);
Eval_Node func_square_root(Eval_Context *ctx, Node_Handle left, Node_Handle right);

Eval_Node func_filter     (Eval_Context *ctx, Node_Handle left, Node_Handle right);
Eval_Node func_shape      (Eval_Context *ctx, Node_Handle left, Node_Handle right);
Eval_Node func_reshape    (Eval_Context *ctx, Node_Handle left, Node_Handle right);

#endif // FUNCS_H
