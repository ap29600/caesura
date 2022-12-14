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

static const u64 ranges[Types_Count] = {
	[Type_None]  = 0,
	[Type_Bool]  = 1,
	[Type_Char]  = 8,
	[Type_Int]   = 64,
	[Type_Float] = 2048,
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
	++array->ref_count;
	return array;
}

void release_array(Array *array) {
	assert(array);
	if (array->ref_count == 1) { free(array); }
	else { --array->ref_count; }
}

Array *clone_array(Array *array) {
	assert(array);
	Array *result = make_array(array->data, array->shape, array->type);
	release_array(array);
	return result;
}

Array* array_append_elem(const void *elem, Array *array, Element_Type type) {
	assert(array);
	assert(array->ref_count == 1 && "permission to modify");
	assert((u8*)array->data == ((u8*)array + sizeof(Array)) && "single allocation");

	u8 buffer[sizeof(i64)] = {0};
	Element_Type supertype = ranges[array->type] >= ranges[type] ? array->type : type;
	array = array_cast(array, supertype);

	if (supertype != type) {
		switch(supertype) {
			break;case Type_Float:
				switch(type) {
					break;case Type_Int: *(f64*)buffer = *(i64*)elem;
					break;case Type_Char: *(f64*)buffer = *(char*)elem;
					break;case Type_Bool: *(f64*)buffer = *(bool*)elem;
					break;default:
						assert(false);
						unreachable();
				}
			break;case Type_Int:
				switch(type) {
					break;case Type_Char: *(i64*)buffer = *(char*)elem;
					break;case Type_Bool: *(i64*)buffer = *(bool*)elem;
					break;default:
						assert(false);
						unreachable();
				}
			break;case Type_Char:
				switch(type) {
					break;case Type_Bool: *(i64*)buffer = *(bool*)elem;
					break;default:
						assert(false);
						unreachable();
				}
			break;default:
				assert(false);
				unreachable();
		}
	} else {
		memcpy(buffer, elem, sizes[supertype]);
	}

	array = realloc(array, sizeof(Array) + (array->shape+1) * sizes[supertype]);
	array->data = ((u8*)array) + sizeof(Array);
	memcpy(((u8*)array->data) + array->shape * sizes[supertype], buffer, sizes[supertype]);
	array->shape += 1;
	return array;
}

Array*  array_cast(Array *array, Element_Type type) {
	if (array->type == type) {
		return array;
	} else {
		Array *result = make_array(NULL, array->shape, type);

		#define CAST_ALL(dest, dest_t, arr, arr_t)                         \
			for(u64 i = 0; i < dest->shape; ++i) {                     \
				((dest_t*)dest->data)[i] = ((arr_t*)arr->data)[i]; \
			}

		switch (array->type) {
			break;case Type_None: assert(false);
			break;case Type_Generic_L: assert(false);
			break;case Type_Generic_R: assert(false);
			break;case Type_Bool:
				switch(type) {
					break;case Type_None:      assert(false);
					break;case Type_Generic_L: assert(false);
					break;case Type_Generic_R: assert(false);
					break;case Type_Bool:      assert(false);
					break;case Type_Char:      CAST_ALL(result, char, array, bool);
					break;case Type_Int:       CAST_ALL(result, i64,  array, bool);
					break;case Type_Float:     CAST_ALL(result, f64,  array, bool);
					break;case Types_Count:    assert(false);
				}
			break;case Type_Char:
				switch(type) {
					break;case Type_None:      assert(false);
					break;case Type_Generic_L: assert(false);
					break;case Type_Generic_R: assert(false);
					break;case Type_Bool:      CAST_ALL(result, bool, array, char);
					break;case Type_Char:      assert(false);
					break;case Type_Int:       CAST_ALL(result, i64,  array, char);
					break;case Type_Float:     CAST_ALL(result, f64,  array, char);
					break;case Types_Count:    assert(false);
				}
			break;case Type_Int:
				switch(type) {
					break;case Type_None:      assert(false);
					break;case Type_Generic_L: assert(false);
					break;case Type_Generic_R: assert(false);
					break;case Type_Bool:      CAST_ALL(result, bool, array, i64);
					break;case Type_Char:      CAST_ALL(result, char, array, i64);
					break;case Type_Int:       assert(false);
					break;case Type_Float:     CAST_ALL(result, f64,  array, i64);
					break;case Types_Count:    assert(false);
				}
			break;case Type_Float:
				switch(type) {
					break;case Type_None:      assert(false);
					break;case Type_Generic_L: assert(false);
					break;case Type_Generic_R: assert(false);
					break;case Type_Bool:      CAST_ALL(result, bool, array, f64);
					break;case Type_Char:      CAST_ALL(result, char, array, f64);
					break;case Type_Int:       CAST_ALL(result, i64,  array, f64);
					break;case Type_Float:     assert(false);
					break;case Types_Count:    assert(false);
				}
			break;case Types_Count: assert(false);
		}

		#undef CAST_ALL

		release_array(array);
		return result;
	}
}
