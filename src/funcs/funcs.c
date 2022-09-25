#include "funcs.h"

#include <stdlib.h>
#include <math.h>


Eval_Node func_right_identity(Eval_Node *, Eval_Node *right) {
	assert(right->type == Node_Array);
	return (Eval_Node) {.type = Node_Array, .as.array = borrow_array(right->as.array)};
}

Eval_Node func_left_identity(Eval_Node *left, Eval_Node *) {
	assert(left->type == Node_Array);
	return (Eval_Node) {.type = Node_Array, .as.array = borrow_array(left->as.array)};
}

void init_default_scope(void) {
	scope_insert(&default_scope, (Lookup_Entry){ .name = {"->"}, .func = func_right_identity, .left = Type_None,      .right = Type_Generic_R, .result = Type_Generic_R });
	scope_insert(&default_scope, (Lookup_Entry){ .name = {"<-"}, .func = func_left_identity,  .left = Type_Generic_L, .right = Type_Generic_R, .result = Type_Generic_L });
	scope_insert(&default_scope, (Lookup_Entry){ .name = {"$"},  .func = func_shape,          .left = Type_None,      .right = Type_Generic_R, .result = Type_Int       });
	scope_insert(&default_scope, (Lookup_Entry){ .name = {"$"},  .func = func_reshape,        .left = Type_Int,       .right = Type_Generic_R, .result = Type_Generic_R });
	scope_insert(&default_scope, (Lookup_Entry){ .name = {"?"},  .func = func_filter,         .left = Type_Bool,      .right = Type_Generic_R, .result = Type_Generic_R });
	bind_generated_funcs();
}

Lookup_Scope default_scope = {0};
