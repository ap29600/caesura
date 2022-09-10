#ifndef ARRAY_H
#define ARRAY_H

#include "lib/builtin.h"

struct Array {
    double *data;
    u64     shape;
    u64     ref_count;
    struct Array *owner;
};
typedef struct Array Array;

Array*  make_array(const double* data, u64 shape);
Array*  clone_array(Array *array);
Array*  borrow_array(Array *array);
void    release_array(Array *array);
Array*  array_append_elem(Array *array, double elem);

#endif // ARRAY_H
