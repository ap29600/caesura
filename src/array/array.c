#include <stdlib.h>
#include <string.h>
#include "lib/format/module.h"

#include "array.h"

const static u64 sizes[] = {
    [Type_Float] = sizeof(f64),
    [Type_Char]  = sizeof(char),
    [Type_Int]   = sizeof(i64),
    [Type_UInt]  = sizeof(u64),
    [Type_Bool]  = sizeof(bool),
};

Array* make_array(const void* data, u64 shape, Element_Type type) {
    Array *result = malloc(sizeof(Array) + sizes[type] * shape);

    *result = (Array){
        .data = ((u8*)result) + sizeof(Array),
        .ref_count = 1,
        .type = type,
        .shape = shape,
    };

    if (data) memcpy(result->data, data, shape * sizes[type]);
    else      memset(result->data, 0,    shape * sizes[type]);

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
    return make_array(array->data, array->shape, array->type);
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
