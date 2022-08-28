#ifndef FZR_STRING
#define FZR_STRING

#include <stdbool.h>
#include <stdio.h>
#include <assert.h>

typedef char rune;
typedef const char *cstring;

typedef struct {
    cstring begin;
    cstring end;
} String;

void          delete_str          (String *s);
bool          empty_string        (String s);
bool          empty_cstring       (cstring s);
String        slice               (String s, long begin, long end);
String        string_from_cstring (cstring s);
String        string_from_stream  (FILE *is);
String        string_split_rune   (String *it, const char delim);
String        string_split_any    (String *it, const rune *delims);
unsigned long string_index_rune   (String s, rune c);
unsigned long string_index_any    (String s, const rune *c);
unsigned long strlen_             (cstring s);

static inline unsigned long string_len (String s);
static inline unsigned long string_len(String s) {
    return s.end - s.begin;
}

#define empty(s)                 \
    _Generic((s),                \
    String      : empty_string,  \
    cstring     : empty_cstring, \
    char*       : empty_cstring  \
    )(s)

#define string_from(s)                 \
    _Generic((s),                      \
    FILE *      : string_from_stream,  \
    char*       : string_from_cstring, \
    cstring     : string_from_cstring  \
    )(s)

#define string_split(s, d)           \
    _Generic((d),                    \
    rune        : string_split_rune, \
    rune*       : string_split_any,  \
    const rune* : string_split_any   \
    )((s), (d))

#define string_index(s, d)           \
    _Generic((d),                    \
    rune        : string_index_rune, \
    rune*       : string_index_any,  \
    const rune* : string_index_any   \
    )((s), (d))

#endif
