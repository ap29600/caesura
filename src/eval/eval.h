#ifndef EVAL_H
#define EVAL_H

#include "lib/builtin.h"
#include "lib/format/module.h"

#include "src/array/array.h"
#include "src/eval/common.h"


typedef struct {
	Short_String name;
	func_t func;
	Element_Type left;
	Element_Type right;
	Element_Type result;
} Lookup_Entry;

struct Lookup_Scope {
	Lookup_Entry *entries;
	u64           count;
	u64           cap;
};

i32  entry_cmp (const void* a, const void*b);
Lookup_Entry *scope_lookup(Lookup_Scope *scope, Short_String name);
void scope_insert(Lookup_Scope *scope, Lookup_Entry entry);



/// compiles into ctx as an expression tree the AST contained in base, starting from expression base[expr]
/// returns the index of the parent expression;
Node_Handle apply(struct Eval_Context *ctx, const struct Ast_Node *base, Node_Handle expr);

/// evaluates in place the expression tree contained in ctx, starting from expression expr.
/// returns the index of the result expression;

typedef enum Value_Type {
	Value_Type_Array,
	Value_Type_Function,
	Value_Type_Closure,
} Value_Type;

typedef struct {
	Value_Type type;
	union {
		Array *array;
		func_t function;
	} as;
} Value;
Error flat_eval_into(struct Eval_Context *ctx);

#endif // EVAL_H
