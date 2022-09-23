#ifndef FUNCS_H
#define FUNCS_H

#include "src/eval/eval.h"

void init_default_scope();
extern Lookup_Scope default_scope;

Eval_Node func_plus          ( Eval_Node *left, Eval_Node *right ) ;
Eval_Node func_minus         ( Eval_Node *left, Eval_Node *right ) ;
Eval_Node func_multiply      ( Eval_Node *left, Eval_Node *right ) ;
Eval_Node func_divide        ( Eval_Node *left, Eval_Node *right ) ;
Eval_Node func_mismatch      ( Eval_Node *left, Eval_Node *right ) ;
Eval_Node func_equal         ( Eval_Node *left, Eval_Node *right ) ;
Eval_Node func_greater_equal ( Eval_Node *left, Eval_Node *right ) ;
Eval_Node func_greater       ( Eval_Node *left, Eval_Node *right ) ;
Eval_Node func_less_equal    ( Eval_Node *left, Eval_Node *right ) ;
Eval_Node func_less          ( Eval_Node *left, Eval_Node *right ) ;
Eval_Node func_negate        ( Eval_Node *left, Eval_Node *right ) ;
Eval_Node func_complement    ( Eval_Node *left, Eval_Node *right ) ;
Eval_Node func_square_root   ( Eval_Node *left, Eval_Node *right ) ;

Eval_Node func_filter        ( Eval_Node *left, Eval_Node *right ) ;
Eval_Node func_shape         ( Eval_Node *left, Eval_Node *right ) ;
Eval_Node func_reshape       ( Eval_Node *left, Eval_Node *right ) ;

#endif // FUNCS_H
