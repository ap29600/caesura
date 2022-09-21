#ifndef FZR_FORMAT
#define FZR_FORMAT

#include <stdbool.h>
#include <stdio.h>

#include "types.h"

#include "../scanner/types.h"
#include "../string/types.h"

extern char       default_format_buffer_data[4096];
extern Byte_Slice default_format_buffer;

bool            register_format_directive(Fmt_Directive directive);
fmt_procedure_t lookup_format_directive(const char begin[FORMAT_DIRECTIVE_CHARS]);

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

void set_format_user_ptr(const void* user_ptr);

i64 fmt_i64      [[nodiscard]] (Byte_Slice data, i64      src, Fmt_Info info);
i64 fmt_u64      [[nodiscard]] (Byte_Slice data, u64      src, Fmt_Info info);
i64 fmt_f64      [[nodiscard]] (Byte_Slice data, f64      src, Fmt_Info info);
i64 fmt_str      [[nodiscard]] (Byte_Slice data, String   src, Fmt_Info info);
i64 fmt_cstr     [[nodiscard]] (Byte_Slice data, cstring  src, Fmt_Info info);
i64 fmt_rune     [[nodiscard]] (Byte_Slice data, rune     src, Fmt_Info info);
i64 fmt_error    [[nodiscard]] (Byte_Slice dest, Error    src, Fmt_Info info);
i64 fmt_location [[nodiscard]] (Byte_Slice dest, Location src, Fmt_Info info);
i64 fmt_ptr      [[nodiscard]] (Byte_Slice dest, uintptr_t src, Fmt_Info info);

#endif
