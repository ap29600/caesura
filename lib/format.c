#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include "format.h"
#include "error.h"

u8 default_format_buffer_data[4096];
Byte_Slice default_format_buffer = {
    default_format_buffer_data,
    default_format_buffer_data + sizeof(default_format_buffer_data)
};

u64 directives_count = 9;
Fmt_Directive directives[256] = {
    {"i64",   fmt_i64_va },
    {"u64",   fmt_u64_va },
    {"i32",   fmt_i32_va },
    {"u32",   fmt_u32_va },
    {"str",   fmt_str_va },
    {"cstr",  fmt_cstr_va},
    {"rune",  fmt_rune_va},
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

String format_to(Byte_Slice dest, cstring fmt, ...) {
    va_list params;
    va_start(params, fmt);
    String result = format_to_va(dest, fmt, params);
    va_end(params);
    return result;
}

String format_to_va(Byte_Slice dest, cstring fmt, va_list params) {
    char *begin = (char*)dest.begin;

    while(*fmt != '\0' && dest.begin < dest.end) {
        if (fmt[0] == '{') {
            fmt++;

            const char *keyword = fmt;
            for(; *fmt != '\0' && *fmt != '}'; fmt++);
            if (*fmt == '\0') { exit(69); }
            const i32 keyword_len = fmt - keyword;
            fmt++;
            i32 idx = 0;
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

    char *end = (char*)dest.begin;
    return (String){begin, end};
}

i64 fmt_i64(Byte_Slice destination, i64 val, Fmt_Info info) {
    char bytes[32];
    u64  bytes_count = 0;
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

    i64 len = destination.end - destination.begin;
    for (i64 i = 0; i < bytes_count && i < len; i++)
        destination.begin[i] = bytes[bytes_count - i - 1];
    return bytes_count;
}


i64 fmt_u64(Byte_Slice destination, u64 val, Fmt_Info info) {
    char bytes[32];
    u64  bytes_count = 0;

    if (val == 0) {
        bytes[bytes_count++] = '0';
    } else {
        for(; val > 0; val /= 10)
            bytes[bytes_count++] = (val % 10) + '0';
    }

    i64 len = destination.end - destination.begin;
    for (i64 i = 0; i < bytes_count && i < len; i++)
        destination.begin[i] = bytes[bytes_count - i - 1];

    return bytes_count;
}

i64 fmt_u64_va(Byte_Slice destination, va_list va, Fmt_Info info) {
    u64 val = va_arg(va, u64);
    return fmt_u64(destination, val, info);
}

i64 fmt_u32_va(Byte_Slice destination, va_list va, Fmt_Info info) {
    u32 val = va_arg(va, u32);
    return fmt_u64(destination, val, info);
}

i64 fmt_i64_va(Byte_Slice destination, va_list va, Fmt_Info info) {
    i64 val = va_arg(va, i64);
    return fmt_i64(destination, val, info);
}

i64 fmt_i32_va(Byte_Slice destination, va_list va, Fmt_Info info) {
    i32 val = va_arg(va, i32);
    return fmt_i64(destination, val, info);
}

static inline i64 min(i64 a, i64 b) { return a < b ? a : b; }

i64 fmt_str(Byte_Slice destination, String src, Fmt_Info info) {
    i64 bytes = min(src.end - src.begin, destination.end - destination.begin);
    for (i64 i = 0; i < bytes; i++) {
        destination.begin[i] = src.begin[i];
    }
    return bytes;
}

i64 fmt_str_va(Byte_Slice destination, va_list va, Fmt_Info info) {
    String src = va_arg(va, String);
    return fmt_str(destination, src, info);
}

i64 fmt_cstr(Byte_Slice destination, const char *src, Fmt_Info info) {
    if (!src) return 0;
    i64 max_len = destination.end - destination.begin;
    i64 bytes = 0;
    for (; bytes < max_len && src[bytes] != '\0'; bytes++) {
        destination.begin[bytes] = src[bytes];
    }
    return bytes;
}

i64 fmt_cstr_va(Byte_Slice destination, va_list va, Fmt_Info info) {
    cstring src = va_arg(va, const char*);
    return fmt_cstr(destination, src, info);
}

i64 fmt_rune(Byte_Slice destination, rune src, Fmt_Info info) {
    /// TODO: handle utf-8 encoding
    assert(rune_width(src) == 1);
    if (destination.end - destination.begin > 0) {
        destination.begin[0] = src;
        return 1;
    } else {
        return 0;
    }
}

i64 fmt_rune_va(Byte_Slice destination, va_list va, Fmt_Info info) {
    rune src = va_arg(va, i32);
    return fmt_rune(destination, src, info);
}

i64 fmt_error_va(Byte_Slice dest, va_list va, Fmt_Info info) {
    Error src = va_arg(va, Error);
    return fmt_cstr(dest, runtime_error_strings[src], info);
}

i64 fmt_location(Byte_Slice dest, Location loc, Fmt_Info info) {
    info = (Fmt_Info){0};
    u8 *const begin = dest.begin;
    dest.begin += fmt_cstr (dest, loc.fname,   info);
    dest.begin += fmt_rune (dest, ':',         info);
    dest.begin += fmt_u64  (dest, loc.row + 1, info);
    dest.begin += fmt_rune (dest, ':',         info);
    dest.begin += fmt_u64  (dest, loc.col + 1, info);
    return dest.begin - begin;
}

i64 fmt_location_va(Byte_Slice dest, va_list va, Fmt_Info info) {
    Location src = va_arg(va, Location);
    return fmt_location(dest, src, info);
}
