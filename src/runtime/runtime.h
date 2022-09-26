#ifndef FUNCS_H
#define FUNCS_H

#include "src/eval/eval.h"
#include "src/eval/ir.h"

void init_default_scope();
extern Lookup_Scope default_scope;

IR_Node func_left_identity  (IR_Node *left, IR_Node *right);
IR_Node func_right_identity (IR_Node *left, IR_Node *right);
IR_Node func_filter         (IR_Node *left, IR_Node *right);
IR_Node func_shape          (IR_Node *left, IR_Node *right);
IR_Node func_reshape        (IR_Node *left, IR_Node *right);

void init_default_scope(void);
void bind_generated_funcs(void);
#endif // FUNCS_H
