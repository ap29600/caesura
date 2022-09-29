#ifndef RUNTIME_H
#define RUNTIME_H

#include <stdlib.h>

#include <math.h>
#include "runtime.h"

IR_Node func_right_identity(IR_Node *, IR_Node *right) {
	assert(right->type == IR_Type_Array);
	return (IR_Node) {.type = IR_Type_Array, .as.array = borrow_array(right->as.array)};
}

IR_Node func_left_identity(IR_Node *left, IR_Node *) {
	assert(left->type == IR_Type_Array);
	return (IR_Node) {.type = IR_Type_Array, .as.array = borrow_array(left->as.array)};
}

IR_Node func_cast_bool(IR_Node *, IR_Node *right) {
	assert(right->type == IR_Type_Array);
	Array *const right_arr = array_cast(borrow_array(right->as.array), Type_Bool);
	return (IR_Node) { .type = IR_Type_Array, .as.array = right_arr };
}

IR_Node func_cast_char(IR_Node *, IR_Node *right) {
	assert(right->type == IR_Type_Array);
	Array *const right_arr = array_cast(borrow_array(right->as.array), Type_Char);
	return (IR_Node) { .type = IR_Type_Array, .as.array = right_arr };
}

IR_Node func_cast_i64(IR_Node *, IR_Node *right) {
	assert(right->type == IR_Type_Array);
	Array *const right_arr = array_cast(borrow_array(right->as.array), Type_Int);
	return (IR_Node) { .type = IR_Type_Array, .as.array = right_arr };
}

IR_Node func_cast_f64(IR_Node *, IR_Node *right) {
	assert(right->type == IR_Type_Array);
	Array *const right_arr = array_cast(borrow_array(right->as.array), Type_Float);
	return (IR_Node) { .type = IR_Type_Array, .as.array = right_arr };
}

void init_default_scope(void) {
	scope_insert(&default_scope, (Lookup_Entry){ .name = {"->"},   .func = func_right_identity, .left = Type_None,      .right = Type_Generic_R, .result = Type_Generic_R });
	scope_insert(&default_scope, (Lookup_Entry){ .name = {"<-"},   .func = func_left_identity,  .left = Type_Generic_L, .right = Type_Generic_R, .result = Type_Generic_L });
	scope_insert(&default_scope, (Lookup_Entry){ .name = {"$"},    .func = func_shape,          .left = Type_None,      .right = Type_Generic_R, .result = Type_Int       });
	scope_insert(&default_scope, (Lookup_Entry){ .name = {"$"},    .func = func_reshape,        .left = Type_Int,       .right = Type_Generic_R, .result = Type_Generic_R });
	scope_insert(&default_scope, (Lookup_Entry){ .name = {"?"},    .func = func_filter,         .left = Type_Bool,      .right = Type_Generic_R, .result = Type_Generic_R });

	scope_insert(&default_scope, (Lookup_Entry){ .name = {"f64"},  .func = func_cast_f64,       .left = Type_None,      .right = Type_Generic_R, .result = Type_Float     });
	scope_insert(&default_scope, (Lookup_Entry){ .name = {"i64"},  .func = func_cast_i64,       .left = Type_None,      .right = Type_Generic_R, .result = Type_Int       });
	scope_insert(&default_scope, (Lookup_Entry){ .name = {"char"}, .func = func_cast_char,      .left = Type_None,      .right = Type_Generic_R, .result = Type_Char      });
	scope_insert(&default_scope, (Lookup_Entry){ .name = {"bool"}, .func = func_cast_bool,      .left = Type_None,      .right = Type_Generic_R, .result = Type_Bool      });

	bind_generated_funcs();
}

Lookup_Scope default_scope = {0};

#endif // RUNTIME_H
