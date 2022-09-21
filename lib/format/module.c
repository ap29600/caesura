#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <assert.h>

#include "module.h"

#include "../string/module.h"
#include "../error/module.h"

char default_format_buffer_data[4096];
Byte_Slice default_format_buffer = {
    default_format_buffer_data,
    default_format_buffer_data + sizeof(default_format_buffer_data)
};


static const void *global_format_user_ptr;

void set_format_user_ptr(const void* user_ptr) {
    global_format_user_ptr = user_ptr;
}


String format_to(Byte_Slice dest, cstring fmt, ...) {
    va_list params;
    va_start(params, fmt);
    String result = format_to_va(dest, fmt, params);
    va_end(params);
    return result;
}


String format_to_va(Byte_Slice dest, cstring fmt, va_list params) {
    char *cursor = dest.begin;

    for(u64 i = 0; cursor < dest.end; ++i) {
        switch(fmt[i]) {
            case '\0': return (String){dest.begin, cursor};
            case '{':
                ++i;
                if (fmt[i] == '{') {
                    *cursor++ = fmt[i];
                } else {
                    char directive[FORMAT_DIRECTIVE_CHARS] = {0};

                    for (; fmt[i] == ' ' || fmt[i] == '\t'; ++i);

                    for (int j = 0; fmt[i] != '\0'; ++i, ++j) {
                        if (fmt[i] == ' ' || fmt[i] == '\t' || fmt[i] == '}') { break; }
                        if (j >= FORMAT_DIRECTIVE_CHARS) {
                            fputs("[ERROR]: format directive longer than 16 bytes in format string:\n`", stderr);
                            fputs(fmt, stderr);
                            fputs("`\n", stderr);
                            fflush(stderr);
                            exit(1);
                        }
                        directive[j] = fmt[i];
                    }

                    // for now, ignore up to '}'
                    // TODO: parse optional format options
                    for (; fmt[i] != '}' && fmt[i] != '\0'; ++i);

                    if (fmt[i] == '\0') {
                        fputs("[ERROR]: unmatched format delimiter `{` in format string:\n`", stderr);
                        fputs(fmt, stderr);
                        fputs("`\n", stderr);
                        fflush(stderr);
                        exit(1);
                    }

                    // TODO: extract into a function?
                    {
                        // count the trailing '}': they must be an odd number in order for one of them to be matched 
                        // to the opening '{'.
                        int j = 0;
                        for(; fmt[i+j] == '}'; j++);
                        if (j % 2 == 0) {
                            fputs("[ERROR]: escape sequence `}}` illegal in format directive:\n`", stderr);
                            fputs(fmt, stderr);
                            fputs("`\n", stderr);
                            fflush(stderr);
                            exit(1);
                        }
                    }

                    fmt_procedure_t fmt_proc = lookup_format_directive(directive);
                    if (fmt_proc == NULL) {
                        fputs("[ERROR]: unknown format directive `", stderr);
                        fputs(directive, stderr);
                        fputs("` in format string.\n", stderr);
                        fflush(stderr);
                        exit(1);
                    }

                    Fmt_Info info = {.user_ptr = global_format_user_ptr};
                    cursor += fmt_proc( (Byte_Slice){cursor, dest.end}, params, info);
                }
                break;
            case '}':
                if (fmt[i + 1] == '}') {
                    *cursor++ = fmt[i++];
                } else {
                    fputs("[ERROR]: unmatched format delimiter `}` in format string:\n`", stderr);
                    fputs(fmt, stderr);
                    fputs("`\n", stderr);
                    fflush(stderr);
                    exit(1);
                }
                break;
            default:
                *cursor++ = fmt[i];
                break;
        }
    }

    fputs("[ERROR]: format buffer is full, output may be truncated\n", stderr);
    fflush(stderr);
    exit(1);
}

i64 fmt_f64(Byte_Slice dest, f64 val, Fmt_Info info) {
    const char *begin = dest.begin;
    if (val < 0) {
        dest.begin += fmt_rune(dest,  '-', info);
        val = -val;
    }

    u64 integral = (u64)val;
    dest.begin += fmt_u64(dest, integral, info);
    u64 decimal = (val - integral) * 1000.0 + 0.5;

    if (decimal) {
        dest.begin += fmt_rune(dest, '.', info);
        if (decimal < 100) dest.begin += fmt_rune(dest,  '0', info);
        if (decimal < 10) dest.begin += fmt_rune(dest,  '0', info);
        dest.begin += fmt_u64(dest, decimal, info);
    }

    return dest.begin - begin;
}

i64 fmt_f64_va(Byte_Slice data, va_list va, Fmt_Info info) {
    f64 val = va_arg(va, f64);
    return fmt_f64(data, val, info);
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


i64 fmt_ptr(Byte_Slice destination, uintptr_t val, Fmt_Info info) {
    char map[16] = "0123456789ABCDEF";
    i64 i = 0;
    for(; i < 16 && destination.begin < destination.end; i++, destination.begin++) {
        *destination.begin = map[val >> 60];
        val <<= 4;
    }
    return i;
}

i64 fmt_ptr_va (Byte_Slice destination, va_list va, Fmt_Info info) {
    uintptr_t src = va_arg(va, uintptr_t);
    return fmt_ptr(destination, src, info);
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
    char *const begin = dest.begin;
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
