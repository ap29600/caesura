#ifndef COMMON_H
#define COMMON_H

#include "lib/builtin.h"

typedef i32 Node_Handle;

typedef struct Ast_Node Ast_Node;
typedef struct IR_Node IR_Node;
typedef struct Eval_Context Eval_Context;
typedef struct Lookup_Scope Lookup_Scope;

typedef IR_Node (*func_t)(IR_Node *left, IR_Node *right);

#endif // COMMON_H
