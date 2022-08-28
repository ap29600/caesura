#ifndef FZR_BIT_SET
#define FZR_BIT_SET
#include <stdbool.h>
#include "string.h"

typedef struct {
    unsigned long* data;
} Bit_Set;

inline static Bit_Set bit_set_from_cstring (unsigned long *data, cstring bits);
inline static bool    get_bit              (Bit_Set set, unsigned int bit);
inline static void    set_bit              (Bit_Set set, unsigned int bit, bool state);

#define BITS (8*sizeof(unsigned long))

inline static bool get_bit(Bit_Set set, unsigned int bit) {
    return (set.data[bit / BITS] >> (bit % BITS)) & 1;
}

inline static void set_bit(Bit_Set set, unsigned int bit, bool state) {
    unsigned long word = 1UL << (bit % BITS);
    if (state) {
        set.data[bit / BITS] |= word;
    } else {
        set.data[bit / BITS] &= ~word;
    }
}

inline static Bit_Set bit_set_from_cstring(unsigned long *data, const char *bits) {
    Bit_Set result = {.data = data };
    for(; *bits; bits++) {
        set_bit(result, *bits, true);
    }
    return result;
}

#endif
