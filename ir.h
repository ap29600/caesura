#ifndef IR_H
#define IR_H

#include "src/eval/eval.h"

struct IR_Node;
struct Eval_Context;

typedef struct IR_Node (*func_t)(struct IR_Node *left, struct IR_Node *right);

typedef enum IR_Type {
	IR_Type_None = 0,
	IR_Type_Array,
	IR_Type_Monad,
	IR_Type_Dyad,
	IR_Type_Assign,
	IR_Type_Function,
} IR_Type;

typedef struct IR_Node {
	IR_Type type;
	Element_Type eval_type;
	u64 ref_count;
	union {
		Array *array;
		func_t  function;
		char    identifier[16];
		struct {
			Node_Handle left;
			Node_Handle right;
			Node_Handle callee;
		} args;
	} as;
} IR_Node;


#endif // IR_H
