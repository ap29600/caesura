#include "string.h"
#include <assert.h>
#include <stdlib.h>
#include "bit_set.h"

String slice(String s, long begin, long end) {
    return (String) {
        .begin = begin >= 0 ? (s.begin + begin) : s.begin,
        .end = end >= 0 ? (s.begin + end) : s.end,
    };
}

unsigned long strlen_(const char *s) {
    unsigned long result = 0;
    if (s != NULL) {
        while (*s) {
            s++;
            result++;
        }
    }
    return result;
}

unsigned long string_index_rune(String s, rune c) {
    unsigned long i = 0;
    for (; s.begin < s.end && *s.begin != c; s.begin++) { 
        i++;
    }
    return i;
}

unsigned long string_index_any(String s, const rune *c) {
    if (c) {
        #define SIZE ((256 - 1)/BITS + 1)
        Bit_Set fastmap = {(unsigned long[SIZE]){0}};
        #undef SIZE

        for (const rune *j = c; *j; j++) {
            set_bit(fastmap, *j, true);
        }

        unsigned long i = 0;
        for (; s.begin < s.end && !get_bit(fastmap, *s.begin); s.begin++) {
            i++;
        }
        return i;
    }
    return s.end - s.begin;
}

bool empty_string(String s) {
    return s.end == s.begin;
}

bool empty_cstring(const char *s) {
    return s == 0 || *s == 0;
}

String string_split_rune(String *it, const char delim) {
    String result = (String){
        .begin = it->begin,
        .end   = it->begin + string_index_rune(*it, delim),
    };
    it->begin = result.end + (result.end < it->end ? 1 : 0);
    return result;
}

String string_split_any(String *it, const rune *delims) {
    String result = (String){
        .begin = it->begin,
        .end   = it->begin + string_index_any(*it, delims),
    };
    it->begin = result.end + (result.end < it->end ? 1 : 0);
    return result;
}

void delete_str(String *s) {
    free((void *)s->begin);
    *s = (String){0};
}

String string_from_cstring(const char *s) {
    return (String){s, s + strlen_(s)};
}

String string_from_stream(FILE *is) {
    if (fseek(is, 0, SEEK_END) >= 0) {
        unsigned long cap  = ftell(is);
        fseek(is, 0, SEEK_SET);
        char *data = malloc(cap);
        unsigned long len = fread(data, 1, cap, is);
        assert(len == cap);
        return (String){data, data + len};
    } else {
        unsigned long cap = 8;
        unsigned long len = 0;
        unsigned long num;
        char *data = malloc(cap);

        while ((num = fread(data + len, 1, cap - len, is)) > 0) {
            len += num;
            if (len >= cap) {
                cap *= 2;
                data = realloc(data, cap);
            }
        }

        if (!feof(is)) {
            fprintf(stderr, "[ERROR] reading stream\n");
        }

        data = realloc(data, len);
        return (String){data, data + len};
    }
}

