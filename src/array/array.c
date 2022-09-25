#include <stdlib.h>
#include <string.h>
#include "lib/format/module.h"

#include "array.h"

const char *runtime_type_strings[] = {
	[Type_None]      = "Type_None",
	[Type_Bool]      = "Type_Bool",
	[Type_Char]      = "Type_Char",
	[Type_Int]       = "Type_Int",
	[Type_Float]     = "Type_Float",
	[Types_Count]    = "Types_Count",
	[Type_Generic_L] = "Type_Generic_L",
	[Type_Generic_R] = "Type_Generic_R",
};


static const u64 sizes[Types_Count] = {
	[Type_None]  = 0,
	[Type_Bool]  = sizeof(bool),
	[Type_Char]  = sizeof(char),
	[Type_Int]   = sizeof(i64),
	[Type_Float] = sizeof(f64),
};

Array* make_array(const void* data, u64 shape, Element_Type type) {
	Array *result = malloc(sizeof(Array) + sizes[type] * shape);

	*result = (Array){
		.data = ((u8*)result) + sizeof(Array),
		.ref_count = 1,
		.type = type,
		.shape = shape,
	};

	if (data) { memcpy(result->data, data, shape * sizes[type]); }
	else      { memset(result->data, 0,    shape * sizes[type]); }

	return result;
}

Array *borrow_array(Array *array) {
	assert(array);
	array->ref_count += 1;
	return array;
}

void release_array(Array *array) {
	assert(array);
	if (array->ref_count == 1) {
		free(array);
	} else {
		array->ref_count -= 1;
	}
}

Array *clone_array(Array *array) {
	assert(array);
	Array *result = make_array(array->data, array->shape, array->type);
	release_array(array);
	return result;
}

Array* array_append_elem(const void *elem, Array *array, Element_Type type) {
	assert(array);
	assert(array->type == type && "type mismatch");
	assert(array->ref_count == 1 && "permission to modify");
	assert((u8*)array->data == ((u8*)array + sizeof(Array)) && "single allocation");

	array = realloc(array, sizeof(Array) + (array->shape+1) * sizes[type]);
	array->data = ((u8*)array) + sizeof(Array);
	memcpy(((u8*)array->data) + array->shape * sizes[type], elem, sizes[type]);
	array->shape += 1;
	return array;
}

Array*  array_cast(Array *array, Element_Type type) {
	if (array->type == type) {
		return array;
	} else {
		Array *result = make_array(NULL, array->shape, type);

		#define CAST_ALL(dest, dest_t, arr, arr_t)                 \
			for(u64 i = 0; i < dest->shape; ++i) {                 \
				((dest_t*)dest->data)[i] = ((arr_t*)arr->data)[i]; \
			}

		switch (array->type) {
			break;case Type_None: assert(false);
			break;case Type_Generic_L: assert(false);
			break;case Type_Generic_R: assert(false);
			break;case Type_Bool:
				switch(type) {
					break;case Type_None:   assert(false);
					break;case Type_Generic_L: assert(false);
					break;case Type_Generic_R: assert(false);
					break;case Type_Bool:   assert(false);
					break;case Type_Char:   CAST_ALL(result, char, array, bool);
					break;case Type_Int:    CAST_ALL(result, i64,  array, bool);
					break;case Type_Float:  CAST_ALL(result, f64,  array, bool);
					break;case Types_Count: assert(false);
				}
			break;case Type_Char:
				switch(type) {
					break;case Type_None:   assert(false);
					break;case Type_Generic_L: assert(false);
					break;case Type_Generic_R: assert(false);
					break;case Type_Bool:   CAST_ALL(result, bool, array, char);
					break;case Type_Char:   assert(false);
					break;case Type_Int:    CAST_ALL(result, i64,  array, char);
					break;case Type_Float:  CAST_ALL(result, f64,  array, char);
					break;case Types_Count: assert(false);
				}
			break;case Type_Int:
				switch(type) {
					break;case Type_None:   assert(false);
					break;case Type_Generic_L: assert(false);
					break;case Type_Generic_R: assert(false);
					break;case Type_Bool:   CAST_ALL(result, bool, array, i64);
					break;case Type_Char:   CAST_ALL(result, char, array, i64);
					break;case Type_Int:    assert(false);
					break;case Type_Float:  CAST_ALL(result, f64,  array, i64);
					break;case Types_Count: assert(false);
				}
			break;case Type_Float:
				switch(type) {
					break;case Type_None:   assert(false);
					break;case Type_Generic_L: assert(false);
					break;case Type_Generic_R: assert(false);
					break;case Type_Bool:   CAST_ALL(result, bool, array, f64);
					break;case Type_Char:   CAST_ALL(result, char, array, f64);
					break;case Type_Int:    CAST_ALL(result, i64,  array, f64);
					break;case Type_Float:  assert(false);
					break;case Types_Count: assert(false);
				}
			break;case Types_Count: assert(false);
		}

		#undef CAST_ALL

		release_array(array);
		return result;
	}
}
