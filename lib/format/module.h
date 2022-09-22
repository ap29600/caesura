#ifndef FZR_FORMAT
#define FZR_FORMAT

#include <stdbool.h>
#include <stdio.h>

#include "types.h"

#include "../scanner/types.h"
#include "../string/types.h"

extern char       default_format_buffer_data[4096];
extern Byte_Slice default_format_buffer;

bool register_format_directive(Short_String name, fmt_procedure_t format);
fmt_procedure_t lookup_format_directive(Short_String name);

String format(cstring fmt, ...);
String format_to(Byte_Slice data, const char *fmt, ...);

void   print(String s);
void   println(String s);

void   format_print(cstring fmt, ...);
void   format_println(cstring fmt, ...);

void set_format_user_ptr(const void* user_ptr);

/// predefined format functions, useful for composing new ones

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
