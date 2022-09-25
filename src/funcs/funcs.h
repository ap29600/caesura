#ifndef FUNCS_H
#define FUNCS_H

#include "src/eval/eval.h"

void init_default_scope();
extern Lookup_Scope default_scope;

Eval_Node func_left_identity  (Eval_Node *left, Eval_Node *right);
Eval_Node func_right_identity (Eval_Node *left, Eval_Node *right);
Eval_Node func_filter         (Eval_Node *left, Eval_Node *right);
Eval_Node func_shape          (Eval_Node *left, Eval_Node *right);
Eval_Node func_reshape        (Eval_Node *left, Eval_Node *right);

void init_default_scope(void);
void bind_generated_funcs(void);
#endif // FUNCS_H
