#ifndef FZR_FORMAT_TYPES
#define FZR_FORMAT_TYPES

#include <stdarg.h>
#include "../builtin.h"

typedef struct {
	char *begin;
	char *end;
} Byte_Slice;

typedef struct {
	char pad_with;
	const void *user_ptr;
} Fmt_Info;

#define FORMAT_DIRECTIVE_CHARS 16
typedef i64(*fmt_procedure_t)(Byte_Slice, va_list, Fmt_Info);

#endif
