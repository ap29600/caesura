#include <stdlib.h>
#include <string.h>
#include "lib/format/module.h"

#include "array.h"

Array*  make_array(const double* data, u64 shape) {
    Array *result = malloc(sizeof(Array) + sizeof(double) * shape);
    result->data = (double*)(((u8*)result) + sizeof(Array));
    result->owner = NULL;
    result->ref_count = 1;
    result->shape = shape;

    if (data) memcpy(result->data, data, shape * sizeof(double));
    else      memset(result->data, 0,    shape * sizeof(double));

    return result;
}

Array *borrow_array(Array *array) {
    array->ref_count += 1;
    return array;
}

void release_array(Array *array) {
    if (array->ref_count == 1) {
        if (array->owner) release_array(array->owner);
        free(array);
    } else {
        array->ref_count -= 1;
    }
}

Array *clone_array(Array *array) {
    return make_array(array->data, array->shape);
}

Array* array_append_elem(Array *array, double elem) {
    if (array->ref_count == 1) {
        array = realloc(array, sizeof(Array) + sizeof(double) * (array->shape + 1));
        array->data = (double*)((uintptr_t)array + sizeof(Array));
        array->data[array->shape++] = elem;
        return array;
    } else {
        Array *copy = make_array(NULL, array->shape + 1);
        memcpy(copy->data, array->data, array->shape * sizeof(double));
        copy->data[copy->shape-1] = elem;
        return copy;
    }
}
