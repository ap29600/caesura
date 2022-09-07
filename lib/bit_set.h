#ifndef FZR_BIT_SET
#define FZR_BIT_SET

#include <stdbool.h>
#include "builtin.h"


typedef struct {
    u64* data;
} Bit_Set;

inline static Bit_Set bit_set_from_runes (u64 *data, const rune *bits);
inline static bool    get_bit_word       (u64 set, u32 bit);
inline static u64     set_bit_word       (u64 set, u32 bit, bool state);
inline static bool    get_bit_set        (Bit_Set set, u32 bit);
inline static void    set_bit_set        (Bit_Set set, u32 bit, bool state);

#define get_bit(set, bit) _Generic((set), u64: get_bit_word, Bit_Set: get_bit_set)(set, bit)
#define set_bit(set, bit, state) _Generic((set), u64: set_bit_word, Bit_Set: set_bit_set)(set, bit, state)

#define BITS (8*sizeof(u64))

inline static bool get_bit_word(u64 set, u32 bit) {
    return set >> bit & 1;
}

inline static u64 set_bit_word(u64 set, u32 bit, bool state) {
    u64 word = 1UL << bit;
    if (state) {
        set |= word;
    } else {
        set &= ~word;
    }
    return set;
}



inline static bool get_bit_set(Bit_Set set, u32 bit) {
    return (set.data[bit / BITS] >> (bit % BITS)) & 1;
}

inline static void set_bit_set(Bit_Set set, u32 bit, bool state) {
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
