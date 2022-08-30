#ifndef FZR_FORMAT
#define FZR_FORMAT

#include <stdbool.h>
#include <stdarg.h>

#include "builtin.h"
#include "parsing.h"
#include "string.h"
#include "error.h"

typedef struct {
    u8 *begin;
    u8 *end;
} Byte_Slice;

typedef struct {
    char pad_with;
} Fmt_Info;

typedef struct {
    char directive[16];
    i64 (*fmt)(Byte_Slice destination, va_list va, Fmt_Info info);
} Fmt_Directive;

extern u8            default_format_buffer_data[4096];
extern Byte_Slice    default_format_buffer;
extern Fmt_Directive directives[256];
extern u64           directives_count;

bool register_format_directive(Fmt_Directive directive);

String format_to(Byte_Slice data, const char *fmt, ...);
String format_to_va(Byte_Slice dest, cstring fmt, va_list params);

static inline void print(String s) {
    fwrite(s.begin, 1, s.end - s.begin, stdout);
}

static inline void println(String s) {
    print(s);
    fputc('\n', stdout);
}


static inline String format(cstring fmt, ...) {
    va_list params;
    va_start(params, fmt);
    String result = format_to_va(default_format_buffer, fmt, params);
    va_end(params);
    return result;
}

static inline void format_print(cstring fmt, ...) {
    va_list params;
    va_start(params, fmt);
    String result = format_to_va(default_format_buffer, fmt, params);
    va_end(params);
    print(result);
}

static inline void format_println(cstring fmt, ...) {
    va_list params;
    va_start(params, fmt);
    String result = format_to_va(default_format_buffer, fmt, params);
    va_end(params);
    println(result);
}

i64 fmt_i64_va      (Byte_Slice data, va_list va, Fmt_Info info);
i64 fmt_u64_va      (Byte_Slice data, va_list va, Fmt_Info info);
i64 fmt_i32_va      (Byte_Slice data, va_list va, Fmt_Info info);
i64 fmt_u32_va      (Byte_Slice data, va_list va, Fmt_Info info);
i64 fmt_str_va      (Byte_Slice data, va_list va, Fmt_Info info);
i64 fmt_cstr_va     (Byte_Slice data, va_list va, Fmt_Info info);
i64 fmt_rune_va     (Byte_Slice data, va_list va, Fmt_Info info);
i64 fmt_error_va    (Byte_Slice dest, va_list va, Fmt_Info info);
i64 fmt_location_va (Byte_Slice dest, va_list va, Fmt_Info info);

i64 fmt_i64      (Byte_Slice data, i64      src, Fmt_Info info);
i64 fmt_u64      (Byte_Slice data, u64      src, Fmt_Info info);
i64 fmt_str      (Byte_Slice data, String   src, Fmt_Info info);
i64 fmt_cstr     (Byte_Slice data, cstring  src, Fmt_Info info);
i64 fmt_rune     (Byte_Slice data, rune     src, Fmt_Info info);
i64 fmt_error    (Byte_Slice dest, Error    src, Fmt_Info info);
i64 fmt_location (Byte_Slice dest, Location src, Fmt_Info info);

#endif
