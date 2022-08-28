#include "format.h"
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include "error.h"

unsigned char default_format_buffer_data[4096];
Byte_Slice default_format_buffer = {
    default_format_buffer_data,
    default_format_buffer_data + sizeof(default_format_buffer_data)
};

unsigned int directives_count = 9;
Fmt_Directive directives[256] = {
    {"i64",   fmt_i64_va },
    {"u64",   fmt_u64_va },
    {"i32",   fmt_i32_va },
    {"u32",   fmt_u32_va },
    {"str",   fmt_str_va },
    {"cstr",  fmt_cstr_va},
    {"char",  fmt_char_va},
    {"error", fmt_error_va},
    {"loc",   fmt_location_va},
};

bool register_format_directive(Fmt_Directive directive) {
    if (directives_count >= sizeof(directives) / sizeof(directives[0])) {
        return false;
    }
    directives[directives_count++] = directive;
    return true;
}

String format_to(Byte_Slice dest, const char *fmt, ...) {
    char *begin = (char*)dest.begin;
    va_list params;
    va_start(params, fmt);

    while(*fmt != '\0' && dest.begin < dest.end) {
        if (fmt[0] == '{') {
            fmt++;

            const char *keyword = fmt;
            for(; *fmt != '\0' && *fmt != '}'; fmt++);
            if (*fmt == '\0') { exit(69); }
            const int keyword_len = fmt - keyword;
            fmt++;
            int idx = 0;
            for(; idx < directives_count; idx++) {
                if (!strncmp(directives[idx].directive, keyword, keyword_len)) {
                    Fmt_Info info = {0};
                    dest.begin += directives[idx].fmt(dest, params, info);
                    break;
                }
            }
            if (idx == directives_count) {
                fputs("[ERROR]: unknown format directive `", stderr);
                fwrite(keyword, 1, keyword_len, stderr);
                fputs("`\n", stderr);
            }
        } else {
            *dest.begin++ = *fmt++;
        }
    }

    va_end(params);

    char *end = (char*)dest.begin;
    return (String){begin, end};
}

int fmt_i64(Byte_Slice destination, long int val, Fmt_Info info) {
    char     bytes[32];
    unsigned bytes_count = 0;
    bool is_negative = val < 0;

    if (val < 0) val = -val;

    if (val == 0) {
        bytes[bytes_count++] = '0';
    } else {
        for(; val > 0; val /= 10)
            bytes[bytes_count++] = (val % 10) + '0';
    }

    if (is_negative)
        bytes[bytes_count++] = '-';

    long len = destination.end - destination.begin;
    for (int i = 0; i < bytes_count && i < len; i++)
        destination.begin[i] = bytes[bytes_count - i - 1];
    return bytes_count;
}


int fmt_u64(Byte_Slice destination, unsigned long val, Fmt_Info info) {
    char     bytes[32];
    unsigned bytes_count = 0;

    if (val == 0) {
        bytes[bytes_count++] = '0';
    } else {
        for(; val > 0; val /= 10)
            bytes[bytes_count++] = (val % 10) + '0';
    }

    long len = destination.end - destination.begin;
    for (int i = 0; i < bytes_count && i < len; i++)
        destination.begin[i] = bytes[bytes_count - i - 1];

    return bytes_count;
}

int fmt_u64_va(Byte_Slice destination, va_list va, Fmt_Info info) {
    unsigned long val = va_arg(va, unsigned long);
    return fmt_u64(destination, val, info);
}

int fmt_u32_va(Byte_Slice destination, va_list va, Fmt_Info info) {
    unsigned val = va_arg(va, unsigned);
    return fmt_u64(destination, val, info);
}

int fmt_i64_va(Byte_Slice destination, va_list va, Fmt_Info info) {
    long int val = va_arg(va, long int);
    return fmt_i64(destination, val, info);
}

int fmt_i32_va(Byte_Slice destination, va_list va, Fmt_Info info) {
    int val = va_arg(va, int);
    return fmt_i64(destination, val, info);
}

static inline long min(long a, long b) { return a < b ? a : b; }

int fmt_str(Byte_Slice destination, String src, Fmt_Info info) {
    long bytes = min(src.end - src.begin, destination.end - destination.begin);
    for (long i = 0; i < bytes; i++) {
        destination.begin[i] = src.begin[i];
    }
    return bytes;
}

int fmt_str_va(Byte_Slice destination, va_list va, Fmt_Info info) {
    String src = va_arg(va, String);
    return fmt_str(destination, src, info);
}

int fmt_cstr(Byte_Slice destination, const char *src, Fmt_Info info) {
    if (!src) return 0;
    long max_len = destination.end - destination.begin;
    long bytes = 0;
    for (; bytes < max_len && src[bytes] != '\0'; bytes++) {
        destination.begin[bytes] = src[bytes];
    }
    return bytes;
}

int fmt_cstr_va(Byte_Slice destination, va_list va, Fmt_Info info) {
    const char *src = va_arg(va, const char*);
    return fmt_cstr(destination, src, info);
}

int fmt_char(Byte_Slice destination, char src, Fmt_Info info) {
    if (destination.end - destination.begin > 0) {
        destination.begin[0] = src;
        return 1;
    } else {
        return 0;
    }
}

int fmt_char_va(Byte_Slice destination, va_list va, Fmt_Info info) {
    char src = va_arg(va, int);
    return fmt_char(destination, src, info);
}

int fmt_error_va(Byte_Slice dest, va_list va, Fmt_Info info) {
    Error src = va_arg(va, Error);
    return fmt_cstr(dest, runtime_error_strings[src], info);
}

int fmt_location(Byte_Slice dest, Location loc, Fmt_Info info) {
    info = (Fmt_Info){0};
    unsigned char *const begin = dest.begin;
    dest.begin += fmt_cstr (dest, loc.fname,   info);
    dest.begin += fmt_char (dest, ':',         info);
    dest.begin += fmt_u64  (dest, loc.row + 1, info);
    dest.begin += fmt_char (dest, ':',         info);
    dest.begin += fmt_u64  (dest, loc.col + 1, info);
    return dest.begin - begin;
}

int fmt_location_va(Byte_Slice dest, va_list va, Fmt_Info info) {
    Location src = va_arg(va, Location);
    return fmt_location(dest, src, info);
}
