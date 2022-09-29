#ifndef FZR_VECTOR
#define FZR_VECTOR

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <alloca.h>

#include "../builtin.h"
#include "../format/module.h"

typedef struct {
	usize size;
	usize capacity;
} Vector_Metadata;

#define Vec_metadata(v)     ((Vector_Metadata*)(v) - 1)
#define Vec_delete(v)       (!*(v) || (free(Vec_metadata(*(v))), *(v) = NULL))
#define Vec_flush(v)        (!*(v) || (Vec_metadata(*(v))->size = 0))
#define Vec_size(v)         ((v) ? Vec_metadata(v)->size : 0ul)
#define Vec_capacity(v)     ((v) ? Vec_metadata(v)->capacity : 0ul)
#define Vec_end(v)          ((v) + Vec_size(v))
#define Vec_back(v)         ((v)[Vec_size(v) - 1])
#define Vec_back_or(v, d)   (Vec_size(v) > 0 ? (v)[Vec_size(v) - 1] : (d))
#define Vec_reserve(v, c)   (Vec_reserve_((void**)(v), sizeof(**(v)), c), Vec_capacity(*(v)) < (c) && (Vec_metadata(*(v))->capacity = (c)))
#define Vec_ensure_fits(v)  (Vec_size(*(v)) >= Vec_capacity(*(v)) && (Vec_grow_((void**)(v), sizeof(**(v))), 1))
#define Vec_push_back(v, i) (Vec_ensure_fits(v), (*(v))[Vec_metadata(*(v))->size] = (i), Vec_metadata(*(v))->size++)
#define Vec_pop_back(v)     ((*(v))[--Vec_metadata(*(v))->size])

// TODO: use rotate
#define Vec_insert(v, i, p) (Vec_ensure_fits(v), memmove(*(v) + (p) + 1, *(v) + (p), sizeof(**(v)) * ((p) - Vec_size(*(v)))), \
	(*(v))[p] = (i), ++Vec_metadata(*(v)).size)
#define Vec_remove(v, p)  (memmove(*(v) + (p), *(v) + (p) + 1, sizeof(**(v)) * (Vec_size(*(v)) - (p) - 1)), --Vec_metadata(*(v))->size)

static inline void *Vec_new_(usize elem_size, usize capacity) {
	u8* const allocation = calloc(elem_size * capacity + sizeof(Vector_Metadata), 1);
	u8* const data = allocation + sizeof(Vector_Metadata);
	Vector_Metadata *const meta = (Vector_Metadata*)allocation;
	meta->capacity  = capacity;
	meta->size      = 0;
	return data;
}

static inline void Vec_reserve_(void **vec, usize elem_size, usize capacity) {
	if (*vec == NULL) {
		*vec = Vec_new_(elem_size, capacity);
	} else {
		Vector_Metadata *const meta = Vec_metadata(*vec);
		if(meta->capacity < capacity) {
			u64 const alloc_size = capacity * elem_size + sizeof(Vector_Metadata);
			meta->capacity = capacity;
			*vec = (Vector_Metadata*)realloc(meta, alloc_size) + 1;
		}
	}
}

static inline void Vec_grow_(void **vec, usize elem_size) {
	if (*vec == NULL) {
		Vec_reserve_(vec, elem_size, 8);
	} else {
		Vector_Metadata *const meta = Vec_metadata(*vec);
		Vec_reserve_(vec, elem_size, meta->capacity > 0 ? meta->capacity * 2 : 8);
	}
}

#endif // FZR_VECTOR
