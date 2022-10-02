#ifndef FZR_ALGORITHM
#define FZR_ALGORITHM

#include "../builtin.h"

typedef i32 (*compar_t)(const void*, const void*);

void swap(void *restrict a, void *restrict b, usize size);
void rotate_(void *begin, void *middle, void *end, usize elem_size);
void quick_sort_(void *begin, void *end, usize elem_size, compar_t compar);
void insertion_sort_(void *begin, void *end, usize elem_size, compar_t compar);
void *partition_(void *begin, void*end, usize size, compar_t compar);

#define rotate(begin, middle, end)          rotate_(begin, middle, end, sizeof(*(begin)))
#define quick_sort(begin, end, compar)      quick_sort_(begin, end, sizeof(*(begin)), (compar_t)compar)
#define insertion_sort(begin, end, compar)  insertion_sort_(begin, end, sizeof(*(begin)), (compar_t)compar)
#define partition(begin, end, compar)       ((__typeof(begin))partition_(begin, end, sizeof(*(begin)), (compar_t)compar))

#endif // FZR_ALGORITHM
