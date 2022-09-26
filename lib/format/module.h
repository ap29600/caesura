#ifndef FZR_FORMAT
#define FZR_FORMAT

#include <stdbool.h>
#include <stdio.h>

#include "types.h"

#include "../scanner/types.h"
#include "../string/types.h"

extern char       default_format_buffer_data[4096];
extern Byte_Slice default_format_buffer;

typedef enum {
	Term_Col_Reset             = 0,

	Term_Col_FG_Black          = 30,
	Term_Col_FG_Red            = 31,
	Term_Col_FG_Green          = 32,
	Term_Col_FG_Yellow         = 33,
	Term_Col_FG_Blue           = 34,
	Term_Col_FG_Magenta        = 35,
	Term_Col_FG_Cyan           = 36,
	Term_Col_FG_White          = 37,

	Term_Col_BG_Black          = 40,
	Term_Col_BG_Red            = 41,
	Term_Col_BG_Green          = 42,
	Term_Col_BG_Yellow         = 43,
	Term_Col_BG_Blue           = 44,
	Term_Col_BG_Magenta        = 45,
	Term_Col_BG_Cyan           = 46,
	Term_Col_BG_White          = 47,

	Term_Col_FG_Bright_Black   = 90,
	Term_Col_FG_Bright_Red     = 91,
	Term_Col_FG_Bright_Green   = 92,
	Term_Col_FG_Bright_Yellow  = 93,
	Term_Col_FG_Bright_Blue    = 94,
	Term_Col_FG_Bright_Magenta = 95,
	Term_Col_FG_Bright_Cyan    = 96,
	Term_Col_FG_Bright_White   = 97,

	Term_Col_BG_Bright_Black   = 100,
	Term_Col_BG_Bright_Red     = 101,
	Term_Col_BG_Bright_Green   = 102,
	Term_Col_BG_Bright_Yellow  = 103,
	Term_Col_BG_Bright_Blue    = 104,
	Term_Col_BG_Bright_Magenta = 105,
	Term_Col_BG_Bright_Cyan    = 106,
	Term_Col_BG_Bright_White   = 107,
} Terminal_Color;

bool register_format_directive(Short_String name, fmt_procedure_t format);
fmt_procedure_t lookup_format_directive(Short_String name);

String format(cstring fmt, ...);
String format_to(Byte_Slice data, const char *fmt, ...);

void print(String s);
void println(String s);

void format_print(cstring fmt, ...);
void format_println(cstring fmt, ...);

void format_fprint(FILE *stream, cstring fmt, ...);
void format_fprintln(FILE *stream, cstring fmt, ...);

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
i64 fmt_ptr      [[nodiscard]] (Byte_Slice dest, usize    src, Fmt_Info info);
i64 fmt_color    [[nodiscard]] (Byte_Slice dest, Terminal_Color src, Fmt_Info info);

#endif
