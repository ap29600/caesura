#include "runtime.h"

IR_Node func_dyad_add_f64_f64 (IR_Node *left_node, IR_Node *right_node) {
	assert(left_node->type == IR_Type_Array);
	assert(right_node->type == IR_Type_Array);
	Array *left  = borrow_array(left_node->as.array);
	Array *right = borrow_array(right_node->as.array);

	assert(left->shape == right->shape);
	u64 shape = left->shape;
	Array *result = make_array(NULL, shape, Type_Float);

	for (u64 i = 0; i < shape; ++i) {
		((f64*)result->data)[i] = ((f64*)left->data)[i] + ((f64*)right->data)[i];
	}

	release_array(left);
	release_array(right);
	return (IR_Node) {.type = IR_Type_Array, .as.array = result};
}

IR_Node func_dyad_add_f64_i64 (IR_Node *left_node, IR_Node *right_node) {
	assert(left_node->type == IR_Type_Array);
	assert(right_node->type == IR_Type_Array);
	Array *left  = borrow_array(left_node->as.array);
	Array *right = borrow_array(right_node->as.array);

	assert(left->shape == right->shape);
	u64 shape = left->shape;
	Array *result = make_array(NULL, shape, Type_Float);

	for (u64 i = 0; i < shape; ++i) {
		((f64*)result->data)[i] = ((f64*)left->data)[i] + ((i64*)right->data)[i];
	}

	release_array(left);
	release_array(right);
	return (IR_Node) {.type = IR_Type_Array, .as.array = result};
}

IR_Node func_dyad_add_f64_char (IR_Node *left_node, IR_Node *right_node) {
	assert(left_node->type == IR_Type_Array);
	assert(right_node->type == IR_Type_Array);
	Array *left  = borrow_array(left_node->as.array);
	Array *right = borrow_array(right_node->as.array);

	assert(left->shape == right->shape);
	u64 shape = left->shape;
	Array *result = make_array(NULL, shape, Type_Float);

	for (u64 i = 0; i < shape; ++i) {
		((f64*)result->data)[i] = ((f64*)left->data)[i] + ((char*)right->data)[i];
	}

	release_array(left);
	release_array(right);
	return (IR_Node) {.type = IR_Type_Array, .as.array = result};
}

IR_Node func_dyad_add_f64_bool (IR_Node *left_node, IR_Node *right_node) {
	assert(left_node->type == IR_Type_Array);
	assert(right_node->type == IR_Type_Array);
	Array *left  = borrow_array(left_node->as.array);
	Array *right = borrow_array(right_node->as.array);

	assert(left->shape == right->shape);
	u64 shape = left->shape;
	Array *result = make_array(NULL, shape, Type_Float);

	for (u64 i = 0; i < shape; ++i) {
		((f64*)result->data)[i] = ((f64*)left->data)[i] + ((bool*)right->data)[i];
	}

	release_array(left);
	release_array(right);
	return (IR_Node) {.type = IR_Type_Array, .as.array = result};
}

IR_Node func_dyad_add_i64_f64 (IR_Node *left_node, IR_Node *right_node) {
	assert(left_node->type == IR_Type_Array);
	assert(right_node->type == IR_Type_Array);
	Array *left  = borrow_array(left_node->as.array);
	Array *right = borrow_array(right_node->as.array);

	assert(left->shape == right->shape);
	u64 shape = left->shape;
	Array *result = make_array(NULL, shape, Type_Float);

	for (u64 i = 0; i < shape; ++i) {
		((f64*)result->data)[i] = ((i64*)left->data)[i] + ((f64*)right->data)[i];
	}

	release_array(left);
	release_array(right);
	return (IR_Node) {.type = IR_Type_Array, .as.array = result};
}

IR_Node func_dyad_add_i64_i64 (IR_Node *left_node, IR_Node *right_node) {
	assert(left_node->type == IR_Type_Array);
	assert(right_node->type == IR_Type_Array);
	Array *left  = borrow_array(left_node->as.array);
	Array *right = borrow_array(right_node->as.array);

	assert(left->shape == right->shape);
	u64 shape = left->shape;
	Array *result = make_array(NULL, shape, Type_Int);

	for (u64 i = 0; i < shape; ++i) {
		((i64*)result->data)[i] = ((i64*)left->data)[i] + ((i64*)right->data)[i];
	}

	release_array(left);
	release_array(right);
	return (IR_Node) {.type = IR_Type_Array, .as.array = result};
}

IR_Node func_dyad_add_i64_char (IR_Node *left_node, IR_Node *right_node) {
	assert(left_node->type == IR_Type_Array);
	assert(right_node->type == IR_Type_Array);
	Array *left  = borrow_array(left_node->as.array);
	Array *right = borrow_array(right_node->as.array);

	assert(left->shape == right->shape);
	u64 shape = left->shape;
	Array *result = make_array(NULL, shape, Type_Int);

	for (u64 i = 0; i < shape; ++i) {
		((i64*)result->data)[i] = ((i64*)left->data)[i] + ((char*)right->data)[i];
	}

	release_array(left);
	release_array(right);
	return (IR_Node) {.type = IR_Type_Array, .as.array = result};
}

IR_Node func_dyad_add_i64_bool (IR_Node *left_node, IR_Node *right_node) {
	assert(left_node->type == IR_Type_Array);
	assert(right_node->type == IR_Type_Array);
	Array *left  = borrow_array(left_node->as.array);
	Array *right = borrow_array(right_node->as.array);

	assert(left->shape == right->shape);
	u64 shape = left->shape;
	Array *result = make_array(NULL, shape, Type_Int);

	for (u64 i = 0; i < shape; ++i) {
		((i64*)result->data)[i] = ((i64*)left->data)[i] + ((bool*)right->data)[i];
	}

	release_array(left);
	release_array(right);
	return (IR_Node) {.type = IR_Type_Array, .as.array = result};
}

IR_Node func_dyad_add_char_f64 (IR_Node *left_node, IR_Node *right_node) {
	assert(left_node->type == IR_Type_Array);
	assert(right_node->type == IR_Type_Array);
	Array *left  = borrow_array(left_node->as.array);
	Array *right = borrow_array(right_node->as.array);

	assert(left->shape == right->shape);
	u64 shape = left->shape;
	Array *result = make_array(NULL, shape, Type_Float);

	for (u64 i = 0; i < shape; ++i) {
		((f64*)result->data)[i] = ((char*)left->data)[i] + ((f64*)right->data)[i];
	}

	release_array(left);
	release_array(right);
	return (IR_Node) {.type = IR_Type_Array, .as.array = result};
}

IR_Node func_dyad_add_char_i64 (IR_Node *left_node, IR_Node *right_node) {
	assert(left_node->type == IR_Type_Array);
	assert(right_node->type == IR_Type_Array);
	Array *left  = borrow_array(left_node->as.array);
	Array *right = borrow_array(right_node->as.array);

	assert(left->shape == right->shape);
	u64 shape = left->shape;
	Array *result = make_array(NULL, shape, Type_Int);

	for (u64 i = 0; i < shape; ++i) {
		((i64*)result->data)[i] = ((char*)left->data)[i] + ((i64*)right->data)[i];
	}

	release_array(left);
	release_array(right);
	return (IR_Node) {.type = IR_Type_Array, .as.array = result};
}

IR_Node func_dyad_add_char_char (IR_Node *left_node, IR_Node *right_node) {
	assert(left_node->type == IR_Type_Array);
	assert(right_node->type == IR_Type_Array);
	Array *left  = borrow_array(left_node->as.array);
	Array *right = borrow_array(right_node->as.array);

	assert(left->shape == right->shape);
	u64 shape = left->shape;
	Array *result = make_array(NULL, shape, Type_Char);

	for (u64 i = 0; i < shape; ++i) {
		((char*)result->data)[i] = ((char*)left->data)[i] + ((char*)right->data)[i];
	}

	release_array(left);
	release_array(right);
	return (IR_Node) {.type = IR_Type_Array, .as.array = result};
}

IR_Node func_dyad_add_char_bool (IR_Node *left_node, IR_Node *right_node) {
	assert(left_node->type == IR_Type_Array);
	assert(right_node->type == IR_Type_Array);
	Array *left  = borrow_array(left_node->as.array);
	Array *right = borrow_array(right_node->as.array);

	assert(left->shape == right->shape);
	u64 shape = left->shape;
	Array *result = make_array(NULL, shape, Type_Char);

	for (u64 i = 0; i < shape; ++i) {
		((char*)result->data)[i] = ((char*)left->data)[i] + ((bool*)right->data)[i];
	}

	release_array(left);
	release_array(right);
	return (IR_Node) {.type = IR_Type_Array, .as.array = result};
}

IR_Node func_dyad_add_bool_f64 (IR_Node *left_node, IR_Node *right_node) {
	assert(left_node->type == IR_Type_Array);
	assert(right_node->type == IR_Type_Array);
	Array *left  = borrow_array(left_node->as.array);
	Array *right = borrow_array(right_node->as.array);

	assert(left->shape == right->shape);
	u64 shape = left->shape;
	Array *result = make_array(NULL, shape, Type_Float);

	for (u64 i = 0; i < shape; ++i) {
		((f64*)result->data)[i] = ((bool*)left->data)[i] + ((f64*)right->data)[i];
	}

	release_array(left);
	release_array(right);
	return (IR_Node) {.type = IR_Type_Array, .as.array = result};
}

IR_Node func_dyad_add_bool_i64 (IR_Node *left_node, IR_Node *right_node) {
	assert(left_node->type == IR_Type_Array);
	assert(right_node->type == IR_Type_Array);
	Array *left  = borrow_array(left_node->as.array);
	Array *right = borrow_array(right_node->as.array);

	assert(left->shape == right->shape);
	u64 shape = left->shape;
	Array *result = make_array(NULL, shape, Type_Int);

	for (u64 i = 0; i < shape; ++i) {
		((i64*)result->data)[i] = ((bool*)left->data)[i] + ((i64*)right->data)[i];
	}

	release_array(left);
	release_array(right);
	return (IR_Node) {.type = IR_Type_Array, .as.array = result};
}

IR_Node func_dyad_add_bool_char (IR_Node *left_node, IR_Node *right_node) {
	assert(left_node->type == IR_Type_Array);
	assert(right_node->type == IR_Type_Array);
	Array *left  = borrow_array(left_node->as.array);
	Array *right = borrow_array(right_node->as.array);

	assert(left->shape == right->shape);
	u64 shape = left->shape;
	Array *result = make_array(NULL, shape, Type_Char);

	for (u64 i = 0; i < shape; ++i) {
		((char*)result->data)[i] = ((bool*)left->data)[i] + ((char*)right->data)[i];
	}

	release_array(left);
	release_array(right);
	return (IR_Node) {.type = IR_Type_Array, .as.array = result};
}

IR_Node func_dyad_add_bool_bool (IR_Node *left_node, IR_Node *right_node) {
	assert(left_node->type == IR_Type_Array);
	assert(right_node->type == IR_Type_Array);
	Array *left  = borrow_array(left_node->as.array);
	Array *right = borrow_array(right_node->as.array);

	assert(left->shape == right->shape);
	u64 shape = left->shape;
	Array *result = make_array(NULL, shape, Type_Int);

	for (u64 i = 0; i < shape; ++i) {
		((i64*)result->data)[i] = ((bool*)left->data)[i] + ((bool*)right->data)[i];
	}

	release_array(left);
	release_array(right);
	return (IR_Node) {.type = IR_Type_Array, .as.array = result};
}

IR_Node func_dyad_sub_f64_f64 (IR_Node *left_node, IR_Node *right_node) {
	assert(left_node->type == IR_Type_Array);
	assert(right_node->type == IR_Type_Array);
	Array *left  = borrow_array(left_node->as.array);
	Array *right = borrow_array(right_node->as.array);

	assert(left->shape == right->shape);
	u64 shape = left->shape;
	Array *result = make_array(NULL, shape, Type_Float);

	for (u64 i = 0; i < shape; ++i) {
		((f64*)result->data)[i] = ((f64*)left->data)[i] - ((f64*)right->data)[i];
	}

	release_array(left);
	release_array(right);
	return (IR_Node) {.type = IR_Type_Array, .as.array = result};
}

IR_Node func_dyad_sub_f64_i64 (IR_Node *left_node, IR_Node *right_node) {
	assert(left_node->type == IR_Type_Array);
	assert(right_node->type == IR_Type_Array);
	Array *left  = borrow_array(left_node->as.array);
	Array *right = borrow_array(right_node->as.array);

	assert(left->shape == right->shape);
	u64 shape = left->shape;
	Array *result = make_array(NULL, shape, Type_Float);

	for (u64 i = 0; i < shape; ++i) {
		((f64*)result->data)[i] = ((f64*)left->data)[i] - ((i64*)right->data)[i];
	}

	release_array(left);
	release_array(right);
	return (IR_Node) {.type = IR_Type_Array, .as.array = result};
}

IR_Node func_dyad_sub_f64_char (IR_Node *left_node, IR_Node *right_node) {
	assert(left_node->type == IR_Type_Array);
	assert(right_node->type == IR_Type_Array);
	Array *left  = borrow_array(left_node->as.array);
	Array *right = borrow_array(right_node->as.array);

	assert(left->shape == right->shape);
	u64 shape = left->shape;
	Array *result = make_array(NULL, shape, Type_Float);

	for (u64 i = 0; i < shape; ++i) {
		((f64*)result->data)[i] = ((f64*)left->data)[i] - ((char*)right->data)[i];
	}

	release_array(left);
	release_array(right);
	return (IR_Node) {.type = IR_Type_Array, .as.array = result};
}

IR_Node func_dyad_sub_f64_bool (IR_Node *left_node, IR_Node *right_node) {
	assert(left_node->type == IR_Type_Array);
	assert(right_node->type == IR_Type_Array);
	Array *left  = borrow_array(left_node->as.array);
	Array *right = borrow_array(right_node->as.array);

	assert(left->shape == right->shape);
	u64 shape = left->shape;
	Array *result = make_array(NULL, shape, Type_Float);

	for (u64 i = 0; i < shape; ++i) {
		((f64*)result->data)[i] = ((f64*)left->data)[i] - ((bool*)right->data)[i];
	}

	release_array(left);
	release_array(right);
	return (IR_Node) {.type = IR_Type_Array, .as.array = result};
}

IR_Node func_dyad_sub_i64_f64 (IR_Node *left_node, IR_Node *right_node) {
	assert(left_node->type == IR_Type_Array);
	assert(right_node->type == IR_Type_Array);
	Array *left  = borrow_array(left_node->as.array);
	Array *right = borrow_array(right_node->as.array);

	assert(left->shape == right->shape);
	u64 shape = left->shape;
	Array *result = make_array(NULL, shape, Type_Float);

	for (u64 i = 0; i < shape; ++i) {
		((f64*)result->data)[i] = ((i64*)left->data)[i] - ((f64*)right->data)[i];
	}

	release_array(left);
	release_array(right);
	return (IR_Node) {.type = IR_Type_Array, .as.array = result};
}

IR_Node func_dyad_sub_i64_i64 (IR_Node *left_node, IR_Node *right_node) {
	assert(left_node->type == IR_Type_Array);
	assert(right_node->type == IR_Type_Array);
	Array *left  = borrow_array(left_node->as.array);
	Array *right = borrow_array(right_node->as.array);

	assert(left->shape == right->shape);
	u64 shape = left->shape;
	Array *result = make_array(NULL, shape, Type_Int);

	for (u64 i = 0; i < shape; ++i) {
		((i64*)result->data)[i] = ((i64*)left->data)[i] - ((i64*)right->data)[i];
	}

	release_array(left);
	release_array(right);
	return (IR_Node) {.type = IR_Type_Array, .as.array = result};
}

IR_Node func_dyad_sub_i64_char (IR_Node *left_node, IR_Node *right_node) {
	assert(left_node->type == IR_Type_Array);
	assert(right_node->type == IR_Type_Array);
	Array *left  = borrow_array(left_node->as.array);
	Array *right = borrow_array(right_node->as.array);

	assert(left->shape == right->shape);
	u64 shape = left->shape;
	Array *result = make_array(NULL, shape, Type_Int);

	for (u64 i = 0; i < shape; ++i) {
		((i64*)result->data)[i] = ((i64*)left->data)[i] - ((char*)right->data)[i];
	}

	release_array(left);
	release_array(right);
	return (IR_Node) {.type = IR_Type_Array, .as.array = result};
}

IR_Node func_dyad_sub_i64_bool (IR_Node *left_node, IR_Node *right_node) {
	assert(left_node->type == IR_Type_Array);
	assert(right_node->type == IR_Type_Array);
	Array *left  = borrow_array(left_node->as.array);
	Array *right = borrow_array(right_node->as.array);

	assert(left->shape == right->shape);
	u64 shape = left->shape;
	Array *result = make_array(NULL, shape, Type_Int);

	for (u64 i = 0; i < shape; ++i) {
		((i64*)result->data)[i] = ((i64*)left->data)[i] - ((bool*)right->data)[i];
	}

	release_array(left);
	release_array(right);
	return (IR_Node) {.type = IR_Type_Array, .as.array = result};
}

IR_Node func_dyad_sub_char_f64 (IR_Node *left_node, IR_Node *right_node) {
	assert(left_node->type == IR_Type_Array);
	assert(right_node->type == IR_Type_Array);
	Array *left  = borrow_array(left_node->as.array);
	Array *right = borrow_array(right_node->as.array);

	assert(left->shape == right->shape);
	u64 shape = left->shape;
	Array *result = make_array(NULL, shape, Type_Float);

	for (u64 i = 0; i < shape; ++i) {
		((f64*)result->data)[i] = ((char*)left->data)[i] - ((f64*)right->data)[i];
	}

	release_array(left);
	release_array(right);
	return (IR_Node) {.type = IR_Type_Array, .as.array = result};
}

IR_Node func_dyad_sub_char_i64 (IR_Node *left_node, IR_Node *right_node) {
	assert(left_node->type == IR_Type_Array);
	assert(right_node->type == IR_Type_Array);
	Array *left  = borrow_array(left_node->as.array);
	Array *right = borrow_array(right_node->as.array);

	assert(left->shape == right->shape);
	u64 shape = left->shape;
	Array *result = make_array(NULL, shape, Type_Int);

	for (u64 i = 0; i < shape; ++i) {
		((i64*)result->data)[i] = ((char*)left->data)[i] - ((i64*)right->data)[i];
	}

	release_array(left);
	release_array(right);
	return (IR_Node) {.type = IR_Type_Array, .as.array = result};
}

IR_Node func_dyad_sub_char_char (IR_Node *left_node, IR_Node *right_node) {
	assert(left_node->type == IR_Type_Array);
	assert(right_node->type == IR_Type_Array);
	Array *left  = borrow_array(left_node->as.array);
	Array *right = borrow_array(right_node->as.array);

	assert(left->shape == right->shape);
	u64 shape = left->shape;
	Array *result = make_array(NULL, shape, Type_Char);

	for (u64 i = 0; i < shape; ++i) {
		((char*)result->data)[i] = ((char*)left->data)[i] - ((char*)right->data)[i];
	}

	release_array(left);
	release_array(right);
	return (IR_Node) {.type = IR_Type_Array, .as.array = result};
}

IR_Node func_dyad_sub_char_bool (IR_Node *left_node, IR_Node *right_node) {
	assert(left_node->type == IR_Type_Array);
	assert(right_node->type == IR_Type_Array);
	Array *left  = borrow_array(left_node->as.array);
	Array *right = borrow_array(right_node->as.array);

	assert(left->shape == right->shape);
	u64 shape = left->shape;
	Array *result = make_array(NULL, shape, Type_Char);

	for (u64 i = 0; i < shape; ++i) {
		((char*)result->data)[i] = ((char*)left->data)[i] - ((bool*)right->data)[i];
	}

	release_array(left);
	release_array(right);
	return (IR_Node) {.type = IR_Type_Array, .as.array = result};
}

IR_Node func_dyad_sub_bool_f64 (IR_Node *left_node, IR_Node *right_node) {
	assert(left_node->type == IR_Type_Array);
	assert(right_node->type == IR_Type_Array);
	Array *left  = borrow_array(left_node->as.array);
	Array *right = borrow_array(right_node->as.array);

	assert(left->shape == right->shape);
	u64 shape = left->shape;
	Array *result = make_array(NULL, shape, Type_Float);

	for (u64 i = 0; i < shape; ++i) {
		((f64*)result->data)[i] = ((bool*)left->data)[i] - ((f64*)right->data)[i];
	}

	release_array(left);
	release_array(right);
	return (IR_Node) {.type = IR_Type_Array, .as.array = result};
}

IR_Node func_dyad_sub_bool_i64 (IR_Node *left_node, IR_Node *right_node) {
	assert(left_node->type == IR_Type_Array);
	assert(right_node->type == IR_Type_Array);
	Array *left  = borrow_array(left_node->as.array);
	Array *right = borrow_array(right_node->as.array);

	assert(left->shape == right->shape);
	u64 shape = left->shape;
	Array *result = make_array(NULL, shape, Type_Int);

	for (u64 i = 0; i < shape; ++i) {
		((i64*)result->data)[i] = ((bool*)left->data)[i] - ((i64*)right->data)[i];
	}

	release_array(left);
	release_array(right);
	return (IR_Node) {.type = IR_Type_Array, .as.array = result};
}

IR_Node func_dyad_sub_bool_char (IR_Node *left_node, IR_Node *right_node) {
	assert(left_node->type == IR_Type_Array);
	assert(right_node->type == IR_Type_Array);
	Array *left  = borrow_array(left_node->as.array);
	Array *right = borrow_array(right_node->as.array);

	assert(left->shape == right->shape);
	u64 shape = left->shape;
	Array *result = make_array(NULL, shape, Type_Char);

	for (u64 i = 0; i < shape; ++i) {
		((char*)result->data)[i] = ((bool*)left->data)[i] - ((char*)right->data)[i];
	}

	release_array(left);
	release_array(right);
	return (IR_Node) {.type = IR_Type_Array, .as.array = result};
}

IR_Node func_dyad_sub_bool_bool (IR_Node *left_node, IR_Node *right_node) {
	assert(left_node->type == IR_Type_Array);
	assert(right_node->type == IR_Type_Array);
	Array *left  = borrow_array(left_node->as.array);
	Array *right = borrow_array(right_node->as.array);

	assert(left->shape == right->shape);
	u64 shape = left->shape;
	Array *result = make_array(NULL, shape, Type_Int);

	for (u64 i = 0; i < shape; ++i) {
		((i64*)result->data)[i] = ((bool*)left->data)[i] - ((bool*)right->data)[i];
	}

	release_array(left);
	release_array(right);
	return (IR_Node) {.type = IR_Type_Array, .as.array = result};
}

IR_Node func_dyad_mul_f64_f64 (IR_Node *left_node, IR_Node *right_node) {
	assert(left_node->type == IR_Type_Array);
	assert(right_node->type == IR_Type_Array);
	Array *left  = borrow_array(left_node->as.array);
	Array *right = borrow_array(right_node->as.array);

	assert(left->shape == right->shape);
	u64 shape = left->shape;
	Array *result = make_array(NULL, shape, Type_Float);

	for (u64 i = 0; i < shape; ++i) {
		((f64*)result->data)[i] = ((f64*)left->data)[i] * ((f64*)right->data)[i];
	}

	release_array(left);
	release_array(right);
	return (IR_Node) {.type = IR_Type_Array, .as.array = result};
}

IR_Node func_dyad_mul_f64_i64 (IR_Node *left_node, IR_Node *right_node) {
	assert(left_node->type == IR_Type_Array);
	assert(right_node->type == IR_Type_Array);
	Array *left  = borrow_array(left_node->as.array);
	Array *right = borrow_array(right_node->as.array);

	assert(left->shape == right->shape);
	u64 shape = left->shape;
	Array *result = make_array(NULL, shape, Type_Float);

	for (u64 i = 0; i < shape; ++i) {
		((f64*)result->data)[i] = ((f64*)left->data)[i] * ((i64*)right->data)[i];
	}

	release_array(left);
	release_array(right);
	return (IR_Node) {.type = IR_Type_Array, .as.array = result};
}

IR_Node func_dyad_mul_f64_char (IR_Node *left_node, IR_Node *right_node) {
	assert(left_node->type == IR_Type_Array);
	assert(right_node->type == IR_Type_Array);
	Array *left  = borrow_array(left_node->as.array);
	Array *right = borrow_array(right_node->as.array);

	assert(left->shape == right->shape);
	u64 shape = left->shape;
	Array *result = make_array(NULL, shape, Type_Float);

	for (u64 i = 0; i < shape; ++i) {
		((f64*)result->data)[i] = ((f64*)left->data)[i] * ((char*)right->data)[i];
	}

	release_array(left);
	release_array(right);
	return (IR_Node) {.type = IR_Type_Array, .as.array = result};
}

IR_Node func_dyad_mul_f64_bool (IR_Node *left_node, IR_Node *right_node) {
	assert(left_node->type == IR_Type_Array);
	assert(right_node->type == IR_Type_Array);
	Array *left  = borrow_array(left_node->as.array);
	Array *right = borrow_array(right_node->as.array);

	assert(left->shape == right->shape);
	u64 shape = left->shape;
	Array *result = make_array(NULL, shape, Type_Float);

	for (u64 i = 0; i < shape; ++i) {
		((f64*)result->data)[i] = ((f64*)left->data)[i] * ((bool*)right->data)[i];
	}

	release_array(left);
	release_array(right);
	return (IR_Node) {.type = IR_Type_Array, .as.array = result};
}

IR_Node func_dyad_mul_i64_f64 (IR_Node *left_node, IR_Node *right_node) {
	assert(left_node->type == IR_Type_Array);
	assert(right_node->type == IR_Type_Array);
	Array *left  = borrow_array(left_node->as.array);
	Array *right = borrow_array(right_node->as.array);

	assert(left->shape == right->shape);
	u64 shape = left->shape;
	Array *result = make_array(NULL, shape, Type_Float);

	for (u64 i = 0; i < shape; ++i) {
		((f64*)result->data)[i] = ((i64*)left->data)[i] * ((f64*)right->data)[i];
	}

	release_array(left);
	release_array(right);
	return (IR_Node) {.type = IR_Type_Array, .as.array = result};
}

IR_Node func_dyad_mul_i64_i64 (IR_Node *left_node, IR_Node *right_node) {
	assert(left_node->type == IR_Type_Array);
	assert(right_node->type == IR_Type_Array);
	Array *left  = borrow_array(left_node->as.array);
	Array *right = borrow_array(right_node->as.array);

	assert(left->shape == right->shape);
	u64 shape = left->shape;
	Array *result = make_array(NULL, shape, Type_Int);

	for (u64 i = 0; i < shape; ++i) {
		((i64*)result->data)[i] = ((i64*)left->data)[i] * ((i64*)right->data)[i];
	}

	release_array(left);
	release_array(right);
	return (IR_Node) {.type = IR_Type_Array, .as.array = result};
}

IR_Node func_dyad_mul_i64_char (IR_Node *left_node, IR_Node *right_node) {
	assert(left_node->type == IR_Type_Array);
	assert(right_node->type == IR_Type_Array);
	Array *left  = borrow_array(left_node->as.array);
	Array *right = borrow_array(right_node->as.array);

	assert(left->shape == right->shape);
	u64 shape = left->shape;
	Array *result = make_array(NULL, shape, Type_Int);

	for (u64 i = 0; i < shape; ++i) {
		((i64*)result->data)[i] = ((i64*)left->data)[i] * ((char*)right->data)[i];
	}

	release_array(left);
	release_array(right);
	return (IR_Node) {.type = IR_Type_Array, .as.array = result};
}

IR_Node func_dyad_mul_i64_bool (IR_Node *left_node, IR_Node *right_node) {
	assert(left_node->type == IR_Type_Array);
	assert(right_node->type == IR_Type_Array);
	Array *left  = borrow_array(left_node->as.array);
	Array *right = borrow_array(right_node->as.array);

	assert(left->shape == right->shape);
	u64 shape = left->shape;
	Array *result = make_array(NULL, shape, Type_Int);

	for (u64 i = 0; i < shape; ++i) {
		((i64*)result->data)[i] = ((i64*)left->data)[i] * ((bool*)right->data)[i];
	}

	release_array(left);
	release_array(right);
	return (IR_Node) {.type = IR_Type_Array, .as.array = result};
}

IR_Node func_dyad_mul_char_f64 (IR_Node *left_node, IR_Node *right_node) {
	assert(left_node->type == IR_Type_Array);
	assert(right_node->type == IR_Type_Array);
	Array *left  = borrow_array(left_node->as.array);
	Array *right = borrow_array(right_node->as.array);

	assert(left->shape == right->shape);
	u64 shape = left->shape;
	Array *result = make_array(NULL, shape, Type_Float);

	for (u64 i = 0; i < shape; ++i) {
		((f64*)result->data)[i] = ((char*)left->data)[i] * ((f64*)right->data)[i];
	}

	release_array(left);
	release_array(right);
	return (IR_Node) {.type = IR_Type_Array, .as.array = result};
}

IR_Node func_dyad_mul_char_i64 (IR_Node *left_node, IR_Node *right_node) {
	assert(left_node->type == IR_Type_Array);
	assert(right_node->type == IR_Type_Array);
	Array *left  = borrow_array(left_node->as.array);
	Array *right = borrow_array(right_node->as.array);

	assert(left->shape == right->shape);
	u64 shape = left->shape;
	Array *result = make_array(NULL, shape, Type_Int);

	for (u64 i = 0; i < shape; ++i) {
		((i64*)result->data)[i] = ((char*)left->data)[i] * ((i64*)right->data)[i];
	}

	release_array(left);
	release_array(right);
	return (IR_Node) {.type = IR_Type_Array, .as.array = result};
}

IR_Node func_dyad_mul_char_char (IR_Node *left_node, IR_Node *right_node) {
	assert(left_node->type == IR_Type_Array);
	assert(right_node->type == IR_Type_Array);
	Array *left  = borrow_array(left_node->as.array);
	Array *right = borrow_array(right_node->as.array);

	assert(left->shape == right->shape);
	u64 shape = left->shape;
	Array *result = make_array(NULL, shape, Type_Char);

	for (u64 i = 0; i < shape; ++i) {
		((char*)result->data)[i] = ((char*)left->data)[i] * ((char*)right->data)[i];
	}

	release_array(left);
	release_array(right);
	return (IR_Node) {.type = IR_Type_Array, .as.array = result};
}

IR_Node func_dyad_mul_char_bool (IR_Node *left_node, IR_Node *right_node) {
	assert(left_node->type == IR_Type_Array);
	assert(right_node->type == IR_Type_Array);
	Array *left  = borrow_array(left_node->as.array);
	Array *right = borrow_array(right_node->as.array);

	assert(left->shape == right->shape);
	u64 shape = left->shape;
	Array *result = make_array(NULL, shape, Type_Char);

	for (u64 i = 0; i < shape; ++i) {
		((char*)result->data)[i] = ((char*)left->data)[i] * ((bool*)right->data)[i];
	}

	release_array(left);
	release_array(right);
	return (IR_Node) {.type = IR_Type_Array, .as.array = result};
}

IR_Node func_dyad_mul_bool_f64 (IR_Node *left_node, IR_Node *right_node) {
	assert(left_node->type == IR_Type_Array);
	assert(right_node->type == IR_Type_Array);
	Array *left  = borrow_array(left_node->as.array);
	Array *right = borrow_array(right_node->as.array);

	assert(left->shape == right->shape);
	u64 shape = left->shape;
	Array *result = make_array(NULL, shape, Type_Float);

	for (u64 i = 0; i < shape; ++i) {
		((f64*)result->data)[i] = ((bool*)left->data)[i] * ((f64*)right->data)[i];
	}

	release_array(left);
	release_array(right);
	return (IR_Node) {.type = IR_Type_Array, .as.array = result};
}

IR_Node func_dyad_mul_bool_i64 (IR_Node *left_node, IR_Node *right_node) {
	assert(left_node->type == IR_Type_Array);
	assert(right_node->type == IR_Type_Array);
	Array *left  = borrow_array(left_node->as.array);
	Array *right = borrow_array(right_node->as.array);

	assert(left->shape == right->shape);
	u64 shape = left->shape;
	Array *result = make_array(NULL, shape, Type_Int);

	for (u64 i = 0; i < shape; ++i) {
		((i64*)result->data)[i] = ((bool*)left->data)[i] * ((i64*)right->data)[i];
	}

	release_array(left);
	release_array(right);
	return (IR_Node) {.type = IR_Type_Array, .as.array = result};
}

IR_Node func_dyad_mul_bool_char (IR_Node *left_node, IR_Node *right_node) {
	assert(left_node->type == IR_Type_Array);
	assert(right_node->type == IR_Type_Array);
	Array *left  = borrow_array(left_node->as.array);
	Array *right = borrow_array(right_node->as.array);

	assert(left->shape == right->shape);
	u64 shape = left->shape;
	Array *result = make_array(NULL, shape, Type_Char);

	for (u64 i = 0; i < shape; ++i) {
		((char*)result->data)[i] = ((bool*)left->data)[i] * ((char*)right->data)[i];
	}

	release_array(left);
	release_array(right);
	return (IR_Node) {.type = IR_Type_Array, .as.array = result};
}

IR_Node func_dyad_mul_bool_bool (IR_Node *left_node, IR_Node *right_node) {
	assert(left_node->type == IR_Type_Array);
	assert(right_node->type == IR_Type_Array);
	Array *left  = borrow_array(left_node->as.array);
	Array *right = borrow_array(right_node->as.array);

	assert(left->shape == right->shape);
	u64 shape = left->shape;
	Array *result = make_array(NULL, shape, Type_Bool);

	for (u64 i = 0; i < shape; ++i) {
		((bool*)result->data)[i] = ((bool*)left->data)[i] & ((bool*)right->data)[i];
	}

	release_array(left);
	release_array(right);
	return (IR_Node) {.type = IR_Type_Array, .as.array = result};
}

IR_Node func_dyad_div_f64_f64 (IR_Node *left_node, IR_Node *right_node) {
	assert(left_node->type == IR_Type_Array);
	assert(right_node->type == IR_Type_Array);
	Array *left  = borrow_array(left_node->as.array);
	Array *right = borrow_array(right_node->as.array);

	assert(left->shape == right->shape);
	u64 shape = left->shape;
	Array *result = make_array(NULL, shape, Type_Float);

	for (u64 i = 0; i < shape; ++i) {
		((f64*)result->data)[i] = ((f64*)left->data)[i] / ((f64*)right->data)[i];
	}

	release_array(left);
	release_array(right);
	return (IR_Node) {.type = IR_Type_Array, .as.array = result};
}

IR_Node func_dyad_div_f64_i64 (IR_Node *left_node, IR_Node *right_node) {
	assert(left_node->type == IR_Type_Array);
	assert(right_node->type == IR_Type_Array);
	Array *left  = borrow_array(left_node->as.array);
	Array *right = borrow_array(right_node->as.array);

	assert(left->shape == right->shape);
	u64 shape = left->shape;
	Array *result = make_array(NULL, shape, Type_Float);

	for (u64 i = 0; i < shape; ++i) {
		((f64*)result->data)[i] = ((f64*)left->data)[i] / ((i64*)right->data)[i];
	}

	release_array(left);
	release_array(right);
	return (IR_Node) {.type = IR_Type_Array, .as.array = result};
}

IR_Node func_dyad_div_f64_char (IR_Node *left_node, IR_Node *right_node) {
	assert(left_node->type == IR_Type_Array);
	assert(right_node->type == IR_Type_Array);
	Array *left  = borrow_array(left_node->as.array);
	Array *right = borrow_array(right_node->as.array);

	assert(left->shape == right->shape);
	u64 shape = left->shape;
	Array *result = make_array(NULL, shape, Type_Float);

	for (u64 i = 0; i < shape; ++i) {
		((f64*)result->data)[i] = ((f64*)left->data)[i] / ((char*)right->data)[i];
	}

	release_array(left);
	release_array(right);
	return (IR_Node) {.type = IR_Type_Array, .as.array = result};
}

// f64 / bool ==> deleted

IR_Node func_dyad_div_i64_f64 (IR_Node *left_node, IR_Node *right_node) {
	assert(left_node->type == IR_Type_Array);
	assert(right_node->type == IR_Type_Array);
	Array *left  = borrow_array(left_node->as.array);
	Array *right = borrow_array(right_node->as.array);

	assert(left->shape == right->shape);
	u64 shape = left->shape;
	Array *result = make_array(NULL, shape, Type_Float);

	for (u64 i = 0; i < shape; ++i) {
		((f64*)result->data)[i] = ((i64*)left->data)[i] / ((f64*)right->data)[i];
	}

	release_array(left);
	release_array(right);
	return (IR_Node) {.type = IR_Type_Array, .as.array = result};
}

IR_Node func_dyad_div_i64_i64 (IR_Node *left_node, IR_Node *right_node) {
	assert(left_node->type == IR_Type_Array);
	assert(right_node->type == IR_Type_Array);
	Array *left  = borrow_array(left_node->as.array);
	Array *right = borrow_array(right_node->as.array);

	assert(left->shape == right->shape);
	u64 shape = left->shape;
	Array *result = make_array(NULL, shape, Type_Int);

	for (u64 i = 0; i < shape; ++i) {
		((i64*)result->data)[i] = ((i64*)left->data)[i] / ((i64*)right->data)[i];
	}

	release_array(left);
	release_array(right);
	return (IR_Node) {.type = IR_Type_Array, .as.array = result};
}

IR_Node func_dyad_div_i64_char (IR_Node *left_node, IR_Node *right_node) {
	assert(left_node->type == IR_Type_Array);
	assert(right_node->type == IR_Type_Array);
	Array *left  = borrow_array(left_node->as.array);
	Array *right = borrow_array(right_node->as.array);

	assert(left->shape == right->shape);
	u64 shape = left->shape;
	Array *result = make_array(NULL, shape, Type_Int);

	for (u64 i = 0; i < shape; ++i) {
		((i64*)result->data)[i] = ((i64*)left->data)[i] / ((char*)right->data)[i];
	}

	release_array(left);
	release_array(right);
	return (IR_Node) {.type = IR_Type_Array, .as.array = result};
}

// i64 / bool ==> deleted

IR_Node func_dyad_div_char_f64 (IR_Node *left_node, IR_Node *right_node) {
	assert(left_node->type == IR_Type_Array);
	assert(right_node->type == IR_Type_Array);
	Array *left  = borrow_array(left_node->as.array);
	Array *right = borrow_array(right_node->as.array);

	assert(left->shape == right->shape);
	u64 shape = left->shape;
	Array *result = make_array(NULL, shape, Type_Float);

	for (u64 i = 0; i < shape; ++i) {
		((f64*)result->data)[i] = ((char*)left->data)[i] / ((f64*)right->data)[i];
	}

	release_array(left);
	release_array(right);
	return (IR_Node) {.type = IR_Type_Array, .as.array = result};
}

IR_Node func_dyad_div_char_i64 (IR_Node *left_node, IR_Node *right_node) {
	assert(left_node->type == IR_Type_Array);
	assert(right_node->type == IR_Type_Array);
	Array *left  = borrow_array(left_node->as.array);
	Array *right = borrow_array(right_node->as.array);

	assert(left->shape == right->shape);
	u64 shape = left->shape;
	Array *result = make_array(NULL, shape, Type_Int);

	for (u64 i = 0; i < shape; ++i) {
		((i64*)result->data)[i] = ((char*)left->data)[i] / ((i64*)right->data)[i];
	}

	release_array(left);
	release_array(right);
	return (IR_Node) {.type = IR_Type_Array, .as.array = result};
}

IR_Node func_dyad_div_char_char (IR_Node *left_node, IR_Node *right_node) {
	assert(left_node->type == IR_Type_Array);
	assert(right_node->type == IR_Type_Array);
	Array *left  = borrow_array(left_node->as.array);
	Array *right = borrow_array(right_node->as.array);

	assert(left->shape == right->shape);
	u64 shape = left->shape;
	Array *result = make_array(NULL, shape, Type_Char);

	for (u64 i = 0; i < shape; ++i) {
		((char*)result->data)[i] = ((char*)left->data)[i] / ((char*)right->data)[i];
	}

	release_array(left);
	release_array(right);
	return (IR_Node) {.type = IR_Type_Array, .as.array = result};
}

// char / bool ==> deleted

IR_Node func_dyad_div_bool_f64 (IR_Node *left_node, IR_Node *right_node) {
	assert(left_node->type == IR_Type_Array);
	assert(right_node->type == IR_Type_Array);
	Array *left  = borrow_array(left_node->as.array);
	Array *right = borrow_array(right_node->as.array);

	assert(left->shape == right->shape);
	u64 shape = left->shape;
	Array *result = make_array(NULL, shape, Type_Float);

	for (u64 i = 0; i < shape; ++i) {
		((f64*)result->data)[i] = ((bool*)left->data)[i] / ((f64*)right->data)[i];
	}

	release_array(left);
	release_array(right);
	return (IR_Node) {.type = IR_Type_Array, .as.array = result};
}

IR_Node func_dyad_div_bool_i64 (IR_Node *left_node, IR_Node *right_node) {
	assert(left_node->type == IR_Type_Array);
	assert(right_node->type == IR_Type_Array);
	Array *left  = borrow_array(left_node->as.array);
	Array *right = borrow_array(right_node->as.array);

	assert(left->shape == right->shape);
	u64 shape = left->shape;
	Array *result = make_array(NULL, shape, Type_Int);

	for (u64 i = 0; i < shape; ++i) {
		((i64*)result->data)[i] = ((bool*)left->data)[i] / ((i64*)right->data)[i];
	}

	release_array(left);
	release_array(right);
	return (IR_Node) {.type = IR_Type_Array, .as.array = result};
}

IR_Node func_dyad_div_bool_char (IR_Node *left_node, IR_Node *right_node) {
	assert(left_node->type == IR_Type_Array);
	assert(right_node->type == IR_Type_Array);
	Array *left  = borrow_array(left_node->as.array);
	Array *right = borrow_array(right_node->as.array);

	assert(left->shape == right->shape);
	u64 shape = left->shape;
	Array *result = make_array(NULL, shape, Type_Char);

	for (u64 i = 0; i < shape; ++i) {
		((char*)result->data)[i] = ((bool*)left->data)[i] / ((char*)right->data)[i];
	}

	release_array(left);
	release_array(right);
	return (IR_Node) {.type = IR_Type_Array, .as.array = result};
}

// bool / bool ==> deleted

IR_Node func_dyad_eq_f64_f64 (IR_Node *left_node, IR_Node *right_node) {
	assert(left_node->type == IR_Type_Array);
	assert(right_node->type == IR_Type_Array);
	Array *left  = borrow_array(left_node->as.array);
	Array *right = borrow_array(right_node->as.array);

	assert(left->shape == right->shape);
	u64 shape = left->shape;
	Array *result = make_array(NULL, shape, Type_Bool);

	for (u64 i = 0; i < shape; ++i) {
		((bool*)result->data)[i] = ((f64*)left->data)[i] == ((f64*)right->data)[i];
	}

	release_array(left);
	release_array(right);
	return (IR_Node) {.type = IR_Type_Array, .as.array = result};
}

IR_Node func_dyad_eq_f64_i64 (IR_Node *left_node, IR_Node *right_node) {
	assert(left_node->type == IR_Type_Array);
	assert(right_node->type == IR_Type_Array);
	Array *left  = borrow_array(left_node->as.array);
	Array *right = borrow_array(right_node->as.array);

	assert(left->shape == right->shape);
	u64 shape = left->shape;
	Array *result = make_array(NULL, shape, Type_Bool);

	for (u64 i = 0; i < shape; ++i) {
		((bool*)result->data)[i] = ((f64*)left->data)[i] == ((i64*)right->data)[i];
	}

	release_array(left);
	release_array(right);
	return (IR_Node) {.type = IR_Type_Array, .as.array = result};
}

IR_Node func_dyad_eq_f64_char (IR_Node *left_node, IR_Node *right_node) {
	assert(left_node->type == IR_Type_Array);
	assert(right_node->type == IR_Type_Array);
	Array *left  = borrow_array(left_node->as.array);
	Array *right = borrow_array(right_node->as.array);

	assert(left->shape == right->shape);
	u64 shape = left->shape;
	Array *result = make_array(NULL, shape, Type_Bool);

	for (u64 i = 0; i < shape; ++i) {
		((bool*)result->data)[i] = ((f64*)left->data)[i] == ((char*)right->data)[i];
	}

	release_array(left);
	release_array(right);
	return (IR_Node) {.type = IR_Type_Array, .as.array = result};
}

IR_Node func_dyad_eq_f64_bool (IR_Node *left_node, IR_Node *right_node) {
	assert(left_node->type == IR_Type_Array);
	assert(right_node->type == IR_Type_Array);
	Array *left  = borrow_array(left_node->as.array);
	Array *right = borrow_array(right_node->as.array);

	assert(left->shape == right->shape);
	u64 shape = left->shape;
	Array *result = make_array(NULL, shape, Type_Bool);

	for (u64 i = 0; i < shape; ++i) {
		((bool*)result->data)[i] = ((f64*)left->data)[i] == ((bool*)right->data)[i];
	}

	release_array(left);
	release_array(right);
	return (IR_Node) {.type = IR_Type_Array, .as.array = result};
}

IR_Node func_dyad_eq_i64_f64 (IR_Node *left_node, IR_Node *right_node) {
	assert(left_node->type == IR_Type_Array);
	assert(right_node->type == IR_Type_Array);
	Array *left  = borrow_array(left_node->as.array);
	Array *right = borrow_array(right_node->as.array);

	assert(left->shape == right->shape);
	u64 shape = left->shape;
	Array *result = make_array(NULL, shape, Type_Bool);

	for (u64 i = 0; i < shape; ++i) {
		((bool*)result->data)[i] = ((i64*)left->data)[i] == ((f64*)right->data)[i];
	}

	release_array(left);
	release_array(right);
	return (IR_Node) {.type = IR_Type_Array, .as.array = result};
}

IR_Node func_dyad_eq_i64_i64 (IR_Node *left_node, IR_Node *right_node) {
	assert(left_node->type == IR_Type_Array);
	assert(right_node->type == IR_Type_Array);
	Array *left  = borrow_array(left_node->as.array);
	Array *right = borrow_array(right_node->as.array);

	assert(left->shape == right->shape);
	u64 shape = left->shape;
	Array *result = make_array(NULL, shape, Type_Bool);

	for (u64 i = 0; i < shape; ++i) {
		((bool*)result->data)[i] = ((i64*)left->data)[i] == ((i64*)right->data)[i];
	}

	release_array(left);
	release_array(right);
	return (IR_Node) {.type = IR_Type_Array, .as.array = result};
}

IR_Node func_dyad_eq_i64_char (IR_Node *left_node, IR_Node *right_node) {
	assert(left_node->type == IR_Type_Array);
	assert(right_node->type == IR_Type_Array);
	Array *left  = borrow_array(left_node->as.array);
	Array *right = borrow_array(right_node->as.array);

	assert(left->shape == right->shape);
	u64 shape = left->shape;
	Array *result = make_array(NULL, shape, Type_Bool);

	for (u64 i = 0; i < shape; ++i) {
		((bool*)result->data)[i] = ((i64*)left->data)[i] == ((char*)right->data)[i];
	}

	release_array(left);
	release_array(right);
	return (IR_Node) {.type = IR_Type_Array, .as.array = result};
}

IR_Node func_dyad_eq_i64_bool (IR_Node *left_node, IR_Node *right_node) {
	assert(left_node->type == IR_Type_Array);
	assert(right_node->type == IR_Type_Array);
	Array *left  = borrow_array(left_node->as.array);
	Array *right = borrow_array(right_node->as.array);

	assert(left->shape == right->shape);
	u64 shape = left->shape;
	Array *result = make_array(NULL, shape, Type_Bool);

	for (u64 i = 0; i < shape; ++i) {
		((bool*)result->data)[i] = ((i64*)left->data)[i] == ((bool*)right->data)[i];
	}

	release_array(left);
	release_array(right);
	return (IR_Node) {.type = IR_Type_Array, .as.array = result};
}

IR_Node func_dyad_eq_char_f64 (IR_Node *left_node, IR_Node *right_node) {
	assert(left_node->type == IR_Type_Array);
	assert(right_node->type == IR_Type_Array);
	Array *left  = borrow_array(left_node->as.array);
	Array *right = borrow_array(right_node->as.array);

	assert(left->shape == right->shape);
	u64 shape = left->shape;
	Array *result = make_array(NULL, shape, Type_Bool);

	for (u64 i = 0; i < shape; ++i) {
		((bool*)result->data)[i] = ((char*)left->data)[i] == ((f64*)right->data)[i];
	}

	release_array(left);
	release_array(right);
	return (IR_Node) {.type = IR_Type_Array, .as.array = result};
}

IR_Node func_dyad_eq_char_i64 (IR_Node *left_node, IR_Node *right_node) {
	assert(left_node->type == IR_Type_Array);
	assert(right_node->type == IR_Type_Array);
	Array *left  = borrow_array(left_node->as.array);
	Array *right = borrow_array(right_node->as.array);

	assert(left->shape == right->shape);
	u64 shape = left->shape;
	Array *result = make_array(NULL, shape, Type_Bool);

	for (u64 i = 0; i < shape; ++i) {
		((bool*)result->data)[i] = ((char*)left->data)[i] == ((i64*)right->data)[i];
	}

	release_array(left);
	release_array(right);
	return (IR_Node) {.type = IR_Type_Array, .as.array = result};
}

IR_Node func_dyad_eq_char_char (IR_Node *left_node, IR_Node *right_node) {
	assert(left_node->type == IR_Type_Array);
	assert(right_node->type == IR_Type_Array);
	Array *left  = borrow_array(left_node->as.array);
	Array *right = borrow_array(right_node->as.array);

	assert(left->shape == right->shape);
	u64 shape = left->shape;
	Array *result = make_array(NULL, shape, Type_Bool);

	for (u64 i = 0; i < shape; ++i) {
		((bool*)result->data)[i] = ((char*)left->data)[i] == ((char*)right->data)[i];
	}

	release_array(left);
	release_array(right);
	return (IR_Node) {.type = IR_Type_Array, .as.array = result};
}

IR_Node func_dyad_eq_char_bool (IR_Node *left_node, IR_Node *right_node) {
	assert(left_node->type == IR_Type_Array);
	assert(right_node->type == IR_Type_Array);
	Array *left  = borrow_array(left_node->as.array);
	Array *right = borrow_array(right_node->as.array);

	assert(left->shape == right->shape);
	u64 shape = left->shape;
	Array *result = make_array(NULL, shape, Type_Bool);

	for (u64 i = 0; i < shape; ++i) {
		((bool*)result->data)[i] = ((char*)left->data)[i] == ((bool*)right->data)[i];
	}

	release_array(left);
	release_array(right);
	return (IR_Node) {.type = IR_Type_Array, .as.array = result};
}

IR_Node func_dyad_eq_bool_f64 (IR_Node *left_node, IR_Node *right_node) {
	assert(left_node->type == IR_Type_Array);
	assert(right_node->type == IR_Type_Array);
	Array *left  = borrow_array(left_node->as.array);
	Array *right = borrow_array(right_node->as.array);

	assert(left->shape == right->shape);
	u64 shape = left->shape;
	Array *result = make_array(NULL, shape, Type_Bool);

	for (u64 i = 0; i < shape; ++i) {
		((bool*)result->data)[i] = ((bool*)left->data)[i] == ((f64*)right->data)[i];
	}

	release_array(left);
	release_array(right);
	return (IR_Node) {.type = IR_Type_Array, .as.array = result};
}

IR_Node func_dyad_eq_bool_i64 (IR_Node *left_node, IR_Node *right_node) {
	assert(left_node->type == IR_Type_Array);
	assert(right_node->type == IR_Type_Array);
	Array *left  = borrow_array(left_node->as.array);
	Array *right = borrow_array(right_node->as.array);

	assert(left->shape == right->shape);
	u64 shape = left->shape;
	Array *result = make_array(NULL, shape, Type_Bool);

	for (u64 i = 0; i < shape; ++i) {
		((bool*)result->data)[i] = ((bool*)left->data)[i] == ((i64*)right->data)[i];
	}

	release_array(left);
	release_array(right);
	return (IR_Node) {.type = IR_Type_Array, .as.array = result};
}

IR_Node func_dyad_eq_bool_char (IR_Node *left_node, IR_Node *right_node) {
	assert(left_node->type == IR_Type_Array);
	assert(right_node->type == IR_Type_Array);
	Array *left  = borrow_array(left_node->as.array);
	Array *right = borrow_array(right_node->as.array);

	assert(left->shape == right->shape);
	u64 shape = left->shape;
	Array *result = make_array(NULL, shape, Type_Bool);

	for (u64 i = 0; i < shape; ++i) {
		((bool*)result->data)[i] = ((bool*)left->data)[i] == ((char*)right->data)[i];
	}

	release_array(left);
	release_array(right);
	return (IR_Node) {.type = IR_Type_Array, .as.array = result};
}

IR_Node func_dyad_eq_bool_bool (IR_Node *left_node, IR_Node *right_node) {
	assert(left_node->type == IR_Type_Array);
	assert(right_node->type == IR_Type_Array);
	Array *left  = borrow_array(left_node->as.array);
	Array *right = borrow_array(right_node->as.array);

	assert(left->shape == right->shape);
	u64 shape = left->shape;
	Array *result = make_array(NULL, shape, Type_Bool);

	for (u64 i = 0; i < shape; ++i) {
		((bool*)result->data)[i] = ((bool*)left->data)[i] == ((bool*)right->data)[i];
	}

	release_array(left);
	release_array(right);
	return (IR_Node) {.type = IR_Type_Array, .as.array = result};
}

void bind_generated_funcs (void) {
	scope_insert(&default_scope, (Lookup_Entry){ .name = {"+"}, .func = func_dyad_add_f64_f64, .left = Type_Float, .right = Type_Float, .result = Type_Float, });
	scope_insert(&default_scope, (Lookup_Entry){ .name = {"+"}, .func = func_dyad_add_f64_i64, .left = Type_Float, .right = Type_Int, .result = Type_Float, });
	scope_insert(&default_scope, (Lookup_Entry){ .name = {"+"}, .func = func_dyad_add_f64_char, .left = Type_Float, .right = Type_Char, .result = Type_Float, });
	scope_insert(&default_scope, (Lookup_Entry){ .name = {"+"}, .func = func_dyad_add_f64_bool, .left = Type_Float, .right = Type_Bool, .result = Type_Float, });
	scope_insert(&default_scope, (Lookup_Entry){ .name = {"+"}, .func = func_dyad_add_i64_f64, .left = Type_Int, .right = Type_Float, .result = Type_Float, });
	scope_insert(&default_scope, (Lookup_Entry){ .name = {"+"}, .func = func_dyad_add_i64_i64, .left = Type_Int, .right = Type_Int, .result = Type_Int, });
	scope_insert(&default_scope, (Lookup_Entry){ .name = {"+"}, .func = func_dyad_add_i64_char, .left = Type_Int, .right = Type_Char, .result = Type_Int, });
	scope_insert(&default_scope, (Lookup_Entry){ .name = {"+"}, .func = func_dyad_add_i64_bool, .left = Type_Int, .right = Type_Bool, .result = Type_Int, });
	scope_insert(&default_scope, (Lookup_Entry){ .name = {"+"}, .func = func_dyad_add_char_f64, .left = Type_Char, .right = Type_Float, .result = Type_Float, });
	scope_insert(&default_scope, (Lookup_Entry){ .name = {"+"}, .func = func_dyad_add_char_i64, .left = Type_Char, .right = Type_Int, .result = Type_Int, });
	scope_insert(&default_scope, (Lookup_Entry){ .name = {"+"}, .func = func_dyad_add_char_char, .left = Type_Char, .right = Type_Char, .result = Type_Char, });
	scope_insert(&default_scope, (Lookup_Entry){ .name = {"+"}, .func = func_dyad_add_char_bool, .left = Type_Char, .right = Type_Bool, .result = Type_Char, });
	scope_insert(&default_scope, (Lookup_Entry){ .name = {"+"}, .func = func_dyad_add_bool_f64, .left = Type_Bool, .right = Type_Float, .result = Type_Float, });
	scope_insert(&default_scope, (Lookup_Entry){ .name = {"+"}, .func = func_dyad_add_bool_i64, .left = Type_Bool, .right = Type_Int, .result = Type_Int, });
	scope_insert(&default_scope, (Lookup_Entry){ .name = {"+"}, .func = func_dyad_add_bool_char, .left = Type_Bool, .right = Type_Char, .result = Type_Char, });
	scope_insert(&default_scope, (Lookup_Entry){ .name = {"+"}, .func = func_dyad_add_bool_bool, .left = Type_Bool, .right = Type_Bool, .result = Type_Int, });
	scope_insert(&default_scope, (Lookup_Entry){ .name = {"-"}, .func = func_dyad_sub_f64_f64, .left = Type_Float, .right = Type_Float, .result = Type_Float, });
	scope_insert(&default_scope, (Lookup_Entry){ .name = {"-"}, .func = func_dyad_sub_f64_i64, .left = Type_Float, .right = Type_Int, .result = Type_Float, });
	scope_insert(&default_scope, (Lookup_Entry){ .name = {"-"}, .func = func_dyad_sub_f64_char, .left = Type_Float, .right = Type_Char, .result = Type_Float, });
	scope_insert(&default_scope, (Lookup_Entry){ .name = {"-"}, .func = func_dyad_sub_f64_bool, .left = Type_Float, .right = Type_Bool, .result = Type_Float, });
	scope_insert(&default_scope, (Lookup_Entry){ .name = {"-"}, .func = func_dyad_sub_i64_f64, .left = Type_Int, .right = Type_Float, .result = Type_Float, });
	scope_insert(&default_scope, (Lookup_Entry){ .name = {"-"}, .func = func_dyad_sub_i64_i64, .left = Type_Int, .right = Type_Int, .result = Type_Int, });
	scope_insert(&default_scope, (Lookup_Entry){ .name = {"-"}, .func = func_dyad_sub_i64_char, .left = Type_Int, .right = Type_Char, .result = Type_Int, });
	scope_insert(&default_scope, (Lookup_Entry){ .name = {"-"}, .func = func_dyad_sub_i64_bool, .left = Type_Int, .right = Type_Bool, .result = Type_Int, });
	scope_insert(&default_scope, (Lookup_Entry){ .name = {"-"}, .func = func_dyad_sub_char_f64, .left = Type_Char, .right = Type_Float, .result = Type_Float, });
	scope_insert(&default_scope, (Lookup_Entry){ .name = {"-"}, .func = func_dyad_sub_char_i64, .left = Type_Char, .right = Type_Int, .result = Type_Int, });
	scope_insert(&default_scope, (Lookup_Entry){ .name = {"-"}, .func = func_dyad_sub_char_char, .left = Type_Char, .right = Type_Char, .result = Type_Char, });
	scope_insert(&default_scope, (Lookup_Entry){ .name = {"-"}, .func = func_dyad_sub_char_bool, .left = Type_Char, .right = Type_Bool, .result = Type_Char, });
	scope_insert(&default_scope, (Lookup_Entry){ .name = {"-"}, .func = func_dyad_sub_bool_f64, .left = Type_Bool, .right = Type_Float, .result = Type_Float, });
	scope_insert(&default_scope, (Lookup_Entry){ .name = {"-"}, .func = func_dyad_sub_bool_i64, .left = Type_Bool, .right = Type_Int, .result = Type_Int, });
	scope_insert(&default_scope, (Lookup_Entry){ .name = {"-"}, .func = func_dyad_sub_bool_char, .left = Type_Bool, .right = Type_Char, .result = Type_Char, });
	scope_insert(&default_scope, (Lookup_Entry){ .name = {"-"}, .func = func_dyad_sub_bool_bool, .left = Type_Bool, .right = Type_Bool, .result = Type_Int, });
	scope_insert(&default_scope, (Lookup_Entry){ .name = {"*"}, .func = func_dyad_mul_f64_f64, .left = Type_Float, .right = Type_Float, .result = Type_Float, });
	scope_insert(&default_scope, (Lookup_Entry){ .name = {"*"}, .func = func_dyad_mul_f64_i64, .left = Type_Float, .right = Type_Int, .result = Type_Float, });
	scope_insert(&default_scope, (Lookup_Entry){ .name = {"*"}, .func = func_dyad_mul_f64_char, .left = Type_Float, .right = Type_Char, .result = Type_Float, });
	scope_insert(&default_scope, (Lookup_Entry){ .name = {"*"}, .func = func_dyad_mul_f64_bool, .left = Type_Float, .right = Type_Bool, .result = Type_Float, });
	scope_insert(&default_scope, (Lookup_Entry){ .name = {"*"}, .func = func_dyad_mul_i64_f64, .left = Type_Int, .right = Type_Float, .result = Type_Float, });
	scope_insert(&default_scope, (Lookup_Entry){ .name = {"*"}, .func = func_dyad_mul_i64_i64, .left = Type_Int, .right = Type_Int, .result = Type_Int, });
	scope_insert(&default_scope, (Lookup_Entry){ .name = {"*"}, .func = func_dyad_mul_i64_char, .left = Type_Int, .right = Type_Char, .result = Type_Int, });
	scope_insert(&default_scope, (Lookup_Entry){ .name = {"*"}, .func = func_dyad_mul_i64_bool, .left = Type_Int, .right = Type_Bool, .result = Type_Int, });
	scope_insert(&default_scope, (Lookup_Entry){ .name = {"*"}, .func = func_dyad_mul_char_f64, .left = Type_Char, .right = Type_Float, .result = Type_Float, });
	scope_insert(&default_scope, (Lookup_Entry){ .name = {"*"}, .func = func_dyad_mul_char_i64, .left = Type_Char, .right = Type_Int, .result = Type_Int, });
	scope_insert(&default_scope, (Lookup_Entry){ .name = {"*"}, .func = func_dyad_mul_char_char, .left = Type_Char, .right = Type_Char, .result = Type_Char, });
	scope_insert(&default_scope, (Lookup_Entry){ .name = {"*"}, .func = func_dyad_mul_char_bool, .left = Type_Char, .right = Type_Bool, .result = Type_Char, });
	scope_insert(&default_scope, (Lookup_Entry){ .name = {"*"}, .func = func_dyad_mul_bool_f64, .left = Type_Bool, .right = Type_Float, .result = Type_Float, });
	scope_insert(&default_scope, (Lookup_Entry){ .name = {"*"}, .func = func_dyad_mul_bool_i64, .left = Type_Bool, .right = Type_Int, .result = Type_Int, });
	scope_insert(&default_scope, (Lookup_Entry){ .name = {"*"}, .func = func_dyad_mul_bool_char, .left = Type_Bool, .right = Type_Char, .result = Type_Char, });
	scope_insert(&default_scope, (Lookup_Entry){ .name = {"*"}, .func = func_dyad_mul_bool_bool, .left = Type_Bool, .right = Type_Bool, .result = Type_Bool, });
	scope_insert(&default_scope, (Lookup_Entry){ .name = {"/"}, .func = func_dyad_div_f64_f64, .left = Type_Float, .right = Type_Float, .result = Type_Float, });
	scope_insert(&default_scope, (Lookup_Entry){ .name = {"/"}, .func = func_dyad_div_f64_i64, .left = Type_Float, .right = Type_Int, .result = Type_Float, });
	scope_insert(&default_scope, (Lookup_Entry){ .name = {"/"}, .func = func_dyad_div_f64_char, .left = Type_Float, .right = Type_Char, .result = Type_Float, });
	scope_insert(&default_scope, (Lookup_Entry){ .name = {"/"}, .func = func_dyad_div_i64_f64, .left = Type_Int, .right = Type_Float, .result = Type_Float, });
	scope_insert(&default_scope, (Lookup_Entry){ .name = {"/"}, .func = func_dyad_div_i64_i64, .left = Type_Int, .right = Type_Int, .result = Type_Int, });
	scope_insert(&default_scope, (Lookup_Entry){ .name = {"/"}, .func = func_dyad_div_i64_char, .left = Type_Int, .right = Type_Char, .result = Type_Int, });
	scope_insert(&default_scope, (Lookup_Entry){ .name = {"/"}, .func = func_dyad_div_char_f64, .left = Type_Char, .right = Type_Float, .result = Type_Float, });
	scope_insert(&default_scope, (Lookup_Entry){ .name = {"/"}, .func = func_dyad_div_char_i64, .left = Type_Char, .right = Type_Int, .result = Type_Int, });
	scope_insert(&default_scope, (Lookup_Entry){ .name = {"/"}, .func = func_dyad_div_char_char, .left = Type_Char, .right = Type_Char, .result = Type_Char, });
	scope_insert(&default_scope, (Lookup_Entry){ .name = {"/"}, .func = func_dyad_div_bool_f64, .left = Type_Bool, .right = Type_Float, .result = Type_Float, });
	scope_insert(&default_scope, (Lookup_Entry){ .name = {"/"}, .func = func_dyad_div_bool_i64, .left = Type_Bool, .right = Type_Int, .result = Type_Int, });
	scope_insert(&default_scope, (Lookup_Entry){ .name = {"/"}, .func = func_dyad_div_bool_char, .left = Type_Bool, .right = Type_Char, .result = Type_Char, });
	scope_insert(&default_scope, (Lookup_Entry){ .name = {"="}, .func = func_dyad_eq_f64_f64, .left = Type_Float, .right = Type_Float, .result = Type_Bool, });
	scope_insert(&default_scope, (Lookup_Entry){ .name = {"="}, .func = func_dyad_eq_f64_i64, .left = Type_Float, .right = Type_Int, .result = Type_Bool, });
	scope_insert(&default_scope, (Lookup_Entry){ .name = {"="}, .func = func_dyad_eq_f64_char, .left = Type_Float, .right = Type_Char, .result = Type_Bool, });
	scope_insert(&default_scope, (Lookup_Entry){ .name = {"="}, .func = func_dyad_eq_f64_bool, .left = Type_Float, .right = Type_Bool, .result = Type_Bool, });
	scope_insert(&default_scope, (Lookup_Entry){ .name = {"="}, .func = func_dyad_eq_i64_f64, .left = Type_Int, .right = Type_Float, .result = Type_Bool, });
	scope_insert(&default_scope, (Lookup_Entry){ .name = {"="}, .func = func_dyad_eq_i64_i64, .left = Type_Int, .right = Type_Int, .result = Type_Bool, });
	scope_insert(&default_scope, (Lookup_Entry){ .name = {"="}, .func = func_dyad_eq_i64_char, .left = Type_Int, .right = Type_Char, .result = Type_Bool, });
	scope_insert(&default_scope, (Lookup_Entry){ .name = {"="}, .func = func_dyad_eq_i64_bool, .left = Type_Int, .right = Type_Bool, .result = Type_Bool, });
	scope_insert(&default_scope, (Lookup_Entry){ .name = {"="}, .func = func_dyad_eq_char_f64, .left = Type_Char, .right = Type_Float, .result = Type_Bool, });
	scope_insert(&default_scope, (Lookup_Entry){ .name = {"="}, .func = func_dyad_eq_char_i64, .left = Type_Char, .right = Type_Int, .result = Type_Bool, });
	scope_insert(&default_scope, (Lookup_Entry){ .name = {"="}, .func = func_dyad_eq_char_char, .left = Type_Char, .right = Type_Char, .result = Type_Bool, });
	scope_insert(&default_scope, (Lookup_Entry){ .name = {"="}, .func = func_dyad_eq_char_bool, .left = Type_Char, .right = Type_Bool, .result = Type_Bool, });
	scope_insert(&default_scope, (Lookup_Entry){ .name = {"="}, .func = func_dyad_eq_bool_f64, .left = Type_Bool, .right = Type_Float, .result = Type_Bool, });
	scope_insert(&default_scope, (Lookup_Entry){ .name = {"="}, .func = func_dyad_eq_bool_i64, .left = Type_Bool, .right = Type_Int, .result = Type_Bool, });
	scope_insert(&default_scope, (Lookup_Entry){ .name = {"="}, .func = func_dyad_eq_bool_char, .left = Type_Bool, .right = Type_Char, .result = Type_Bool, });
	scope_insert(&default_scope, (Lookup_Entry){ .name = {"="}, .func = func_dyad_eq_bool_bool, .left = Type_Bool, .right = Type_Bool, .result = Type_Bool, });
}
