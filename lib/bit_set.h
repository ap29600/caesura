#ifndef FZR_BIT_SET
#define FZR_BIT_SET
#include <stdbool.h>
#include "string.h"

typedef struct {
    u64* data;
} Bit_Set;

inline static Bit_Set bit_set_from_runes (u64 *data, const rune *bits);
inline static bool    get_bit            (Bit_Set set, u32 bit);
inline static void    set_bit            (Bit_Set set, u32 bit, bool state);

#define BITS (8*sizeof(u64))

inline static bool get_bit(Bit_Set set, u32 bit) {
    return (set.data[bit / BITS] >> (bit % BITS)) & 1;
}

inline static void set_bit(Bit_Set set, u32 bit, bool state) {
    u64 word = 1UL << (bit % BITS);
    if (state) {
        set.data[bit / BITS] |= word;
    } else {
        set.data[bit / BITS] &= ~word;
    }
}

inline static Bit_Set bit_set_from_runes(u64 *data, const rune *bits) {
    Bit_Set result = {.data = data };
    for(; *bits; bits++) {
        set_bit(result, *bits, true);
    }
    return result;
}

#endif
