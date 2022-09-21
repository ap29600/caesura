#include <stdlib.h>

#include "module.h"

i64 fmt_i64_va      [[nodiscard]] (Byte_Slice data, va_list va, Fmt_Info info);
i64 fmt_u64_va      [[nodiscard]] (Byte_Slice data, va_list va, Fmt_Info info);
i64 fmt_f64_va      [[nodiscard]] (Byte_Slice data, va_list va, Fmt_Info info);
i64 fmt_i32_va      [[nodiscard]] (Byte_Slice data, va_list va, Fmt_Info info);
i64 fmt_u32_va      [[nodiscard]] (Byte_Slice data, va_list va, Fmt_Info info);
i64 fmt_str_va      [[nodiscard]] (Byte_Slice data, va_list va, Fmt_Info info);
i64 fmt_cstr_va     [[nodiscard]] (Byte_Slice data, va_list va, Fmt_Info info);
i64 fmt_rune_va     [[nodiscard]] (Byte_Slice data, va_list va, Fmt_Info info);
i64 fmt_error_va    [[nodiscard]] (Byte_Slice dest, va_list va, Fmt_Info info);
i64 fmt_location_va [[nodiscard]] (Byte_Slice dest, va_list va, Fmt_Info info);
i64 fmt_ptr_va      [[nodiscard]] (Byte_Slice dest, va_list va, Fmt_Info info);

#define MAX_DIRECTIVES 256
static Fmt_Directive directives[MAX_DIRECTIVES] = {
    { "ptr",   fmt_ptr_va      },
    { "i64",   fmt_i64_va      },
    { "u64",   fmt_u64_va      },
    { "f64",   fmt_f64_va      },
    { "f32",   fmt_f64_va      }, // auto-promotion makes it a f64
    { "i32",   fmt_i32_va      },
    { "u32",   fmt_u32_va      },
    { "str",   fmt_str_va      },
    { "cstr",  fmt_cstr_va     },
    { "rune",  fmt_rune_va     },
    { "error", fmt_error_va    },
    { "loc",   fmt_location_va },
};
static u64 directives_count = 0;

static i32 directive_cmp(const void *a, const void*b) {
    return strncmp(a, b, FORMAT_DIRECTIVE_CHARS);
}

static void init_directives(){
    // count the nonempty directives.
    for(i64 i = 0; i < MAX_DIRECTIVES; i++) {
        if (directives[i].directive[0] == '\0') {
            directives_count = i;
            break;
        }
    }

    qsort(directives, directives_count, sizeof(Fmt_Directive), directive_cmp);
}

bool register_format_directive(Fmt_Directive directive) {
    if (directives_count == 0) { init_directives(); }
    if (directives_count >= MAX_DIRECTIVES) { return false; }

    u64 pos = directives_count++;
    while (pos != 0 && directive_cmp(&directive, &directives[pos - 1]) < 0) {
        directives[pos] = directives[pos - 1];
        --pos;
    }
    directives[pos] = directive;
    return true;
}

fmt_procedure_t lookup_format_directive(const char name[FORMAT_DIRECTIVE_CHARS]) {
    if (directives_count == 0) { init_directives(); }
    Fmt_Directive *dir = bsearch(name, directives, directives_count, sizeof(Fmt_Directive), directive_cmp);
    return dir ? dir->fmt : NULL;
}
