#ifndef ARRAY_H
#define ARRAY_H

#include "lib/builtin.h"

typedef enum {
    Type_Bool,
    Type_Char,
    Type_Int,
    Type_Float,
    Types_Count,
} Element_Type;

typedef struct Array {
    void *data;
    u64  shape;
    Element_Type type;
    u64  ref_count;
} Array;

Array*  array_cast(Array *array, Element_Type type);
Array*  make_array(const void* data, u64 shape, Element_Type type);
Array*  clone_array(Array *array);
Array*  borrow_array(Array *array);
void    release_array(Array *array);
Array*  array_append_elem(const void *elem, Array *array, Element_Type type);

#endif // ARRAY_H
