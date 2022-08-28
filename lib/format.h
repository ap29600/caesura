#ifndef FZR_FORMAT
#define FZR_FORMAT

#include <stdbool.h>
#include <stdarg.h>

#include "parsing.h"
#include "string.h"
#include "error.h"

typedef struct {
    unsigned char *begin;
    unsigned char *end;
} Byte_Slice;

typedef struct {
    char pad_with;
} Fmt_Info;

typedef struct {
    char directive[16];
    int (*fmt)(Byte_Slice destination, va_list va, Fmt_Info info);
} Fmt_Directive;

bool register_format_directive(Fmt_Directive directive);

String format_to(Byte_Slice data, const char *fmt, ...);

int fmt_i64_va      (Byte_Slice data, va_list va, Fmt_Info info);
int fmt_u64_va      (Byte_Slice data, va_list va, Fmt_Info info);
int fmt_i32_va      (Byte_Slice data, va_list va, Fmt_Info info);
int fmt_u32_va      (Byte_Slice data, va_list va, Fmt_Info info);
int fmt_str_va      (Byte_Slice data, va_list va, Fmt_Info info);
int fmt_cstr_va     (Byte_Slice data, va_list va, Fmt_Info info);
int fmt_char_va     (Byte_Slice data, va_list va, Fmt_Info info);
int fmt_error_va    (Byte_Slice dest, va_list va, Fmt_Info info);
int fmt_location_va (Byte_Slice dest, va_list va, Fmt_Info info);

int fmt_i64     (Byte_Slice data, long          src, Fmt_Info info);
int fmt_u64     (Byte_Slice data, long unsigned src, Fmt_Info info);
int fmt_str     (Byte_Slice data, String        src, Fmt_Info info);
int fmt_cstr    (Byte_Slice data, const char*   src, Fmt_Info info);
int fmt_char    (Byte_Slice data, char          src, Fmt_Info info);
int fmt_error   (Byte_Slice dest, Error         src, Fmt_Info info);
int fmt_location(Byte_Slice dest, Location      src, Fmt_Info info);

static inline void print(String s) {
    fwrite(s.begin, 1, s.end - s.begin, stdout);
}

static inline void println(String s) {
    print(s);
    fputc('\n', stdout);
}

extern unsigned char default_format_buffer_data[4096];
extern Byte_Slice default_format_buffer;
extern Fmt_Directive directives[256];
extern unsigned int directives_count;


#define format(...)         format_to(default_format_buffer, __VA_ARGS__)
#define format_print(...)   print(format_to(default_format_buffer, __VA_ARGS__))
#define format_println(...) println(format_to(default_format_buffer, __VA_ARGS__))

#endif
