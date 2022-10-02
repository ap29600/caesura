#include <stdlib.h>
#include <assert.h>
#include "module.h"

void swap(void *restrict a, void *restrict b, usize size) {
	for(usize i = 0; i < size; ++i) {
		u8 c = ((u8*)a)[i];
		((u8*)a)[i] = ((u8*)b)[i];
		((u8*)b)[i] = c;
	}
}

static inline void *choose_pivot(void *begin, void *, usize, compar_t) {
	return begin;
}

void *partition_(void *begin, void*end, usize size, compar_t compar) {
	if ((usize)((u8*)end - (u8*)begin) <= size) { return begin; }
	void *pivot = choose_pivot(begin, end, size, compar);
	if (pivot != begin) {
		swap(begin, pivot, size);
		pivot = begin;
	}

	u8 *low  = (u8*)begin + size;
	u8 *high = (u8*)end   - size;
	for (; low < high; ) {
		for (; low < high && compar(pivot, high) <= 0; high -= size);
		for (; low < high && compar(low, pivot)  <= 0; low  += size);
		if (low < high) {
			swap(low, high, size);
		}
	}

	if (pivot != low && compar(low, pivot) < 0) {
		swap(low, pivot, size);
		pivot = low;
	}

	return pivot;
}

void quick_sort_(
	void *const begin,
	void *const end,
	usize const elem_size,
	compar_t const compar
) {
	void* stack[63][2];
	usize stack_size;

	stack[0][0] = begin;
	stack[0][1] = end;
	stack_size  = 1;

	for (; stack_size > 0;) {
		u8* begin_ = stack[stack_size - 1][0];
		u8* end_   = stack[stack_size - 1][1];
		--stack_size;

		for(; (usize)(end_ - begin_) > 16 * elem_size;) {
			u8 *const pivot = partition_(begin_, end_, elem_size, compar);
			if (end_ - pivot - elem_size < (usize)(pivot - begin_)) {
				++stack_size;
				stack[stack_size - 1][0] = begin_;
				stack[stack_size - 1][1] = pivot;
				begin_ = pivot + elem_size;
			} else {
				++stack_size;
				stack[stack_size - 1][0] = pivot + elem_size;
				stack[stack_size - 1][1] = end_;
				end_ = pivot;
			}
		}

		insertion_sort_(begin_, end_, elem_size, compar);
	}
}

void insertion_sort_(
	void *const begin,
	void *const end,
	usize const size,
	compar_t const compar
) {
	u8 *const begin_ = begin;
	u8 *const end_   = end;
	for (u8 *it = begin_ + size; it < end_; it += size) {
		for (u8* other = it; other > begin_ && compar(other, other - size) <= 0; other -= size) {
			swap(other, other - size, size);
		}
	}
}
