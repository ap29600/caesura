#ifndef IR_H
#define IR_H

#include "common.h"

#include "src/array/array.h"

struct IR_Node;
struct Eval_Context;

typedef enum IR_Type {
	IR_Type_None = 0,
	IR_Type_Array,
	IR_Type_Monad,
	IR_Type_Dyad,
	IR_Type_Assign,
	IR_Type_Function,
} IR_Type;

struct IR_Node {
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
};

struct Eval_Context {
	IR_Node      *nodes;
	u64           count;
	u64           cap;
	Lookup_Scope *scope;
};

void release_node(IR_Node *node);
IR_Node flat_eval(Eval_Context *ctx);

#endif // IR_H
