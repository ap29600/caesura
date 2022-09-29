#ifndef FZR_VECTOR
#define FZR_VECTOR

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <alloca.h>

#include "../builtin.h"

struct Vector {
	u8 *data;
	usize count;
	usize capacity;
};

typedef struct Vector Vector;

static inline usize vec_push(Vector *vec, const void *elem, usize size) {
	if (vec->count >= vec->capacity) {
		vec->capacity = vec->capacity == 0 ? 8 : (vec->capacity * 2);
		vec->data = realloc(vec->data, vec->capacity * size);
	}

	memcpy(vec->data + size * vec->count, elem, size);
	return vec->count++;
}

static inline void* vec_pop(Vector *vec, usize size) {
	assert(vec->count > 0);
	--vec->count;
	return vec->data + size * vec->count;
}

static inline void vec_insert(Vector *vec, const void*elem, usize pos, usize size) {
	assert(pos <= vec->count);

	if (vec->count >= vec->capacity) {
		vec->capacity = vec->capacity == 0 ? 8 : (vec->capacity * 2);
		vec->data = realloc(vec->data, vec->capacity * size);
	}

	u8*const addr = (u8*) vec->data + size * pos;
	memmove(addr + size, addr, size * (vec->count - pos));
	memcpy(addr, elem, size);
	++vec->count;
}

static inline void vec_remove(Vector *vec, usize pos, usize size) {
	assert(pos < vec->count);
	u8* const addr = (u8*)vec->data + size * pos;
	memmove(addr, addr + size, size * (vec->count - pos));
	--vec->count;
}

static inline void vec_delete(Vector *vec) {
	free(vec->data);
	*vec = (Vector){0};
}

#define vec_insert(vec, elem, pos, type) ({          \
	type const __elem = (elem);                  \
	vec_remove(vec, &__elem, pos, sizeof(type)); \
})

#define vec_push(vec, elem, type) ({                 \
	type const __elem = (elem);                  \
	vec_push(vec, &__elem, sizeof(type));        \
})

#define vec_remove(vec, pos, type) vec_remove(vec, pos, sizeof(type))

/// returns an lvalue of the last element of the array and decreases length by 1
#define vec_pop(vec, type) (*(type*)vec_pop(vec, sizeof(type)))

/// returns an lvalue of the `pos`th element of the array
// #define vec_index(vec, pos, type) (((type*)(vec)->data)[pos])

/// returns an lvalue of the last element of the array
#define vec_back(vec, type) ({                                                   \
	Vector *const __vec = vec;                                               \
	(((type*)__vec->data)[__vec->count - 1]);                                \
})

/// returns an rvalue of the last element of the array, or the default element
#define vec_back_or_default(vec, default, type) ({                               \
	Vector *const __vec = vec;                                               \
	__vec->count > 0 ? (((type*)__vec->data)[__vec->count - 1]) : (default); \
})

/// returns a pointer to the position immediately following the last element of the array
#define vec_end(vec, type) ({                                                    \
	Vector *const __vec = vec;                                               \
	&((type*)__vec->data)[__vec->count];                                     \
})

#define vec_array(vec, type) ((type*)(vec)->data)


#endif // FZR_VECTOR
