#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <assert.h>

#include "module.h"

#include "../string/module.h"
#include "../error/module.h"

static const void *global_format_user_ptr;

String format_to_va(Byte_Slice dest, cstring fmt, va_list params) {
	char *cursor = dest.begin;

	for(u64 i = 0; cursor < dest.end; ++i) {
		switch(fmt[i]) {
			break;case '\0': return (String){dest.begin, cursor};
			break;case '{':
				++i;
				if (fmt[i] == '{') {
					*cursor++ = fmt[i];
				} else {
					Short_String directive = {0};

					for (; fmt[i] == ' ' || fmt[i] == '\t'; ++i);

					for (u64 j = 0; fmt[i] != '\0'; ++i, ++j) {
						if (fmt[i] == ' ' || fmt[i] == '\t' || fmt[i] == '}') { break; }
						if (j >= sizeof(Short_String)) {
							fputs("[ERROR]: format directive longer than 16 bytes in format string:\n`", stderr);
							fputs(fmt, stderr);
							fputs("`\n", stderr);
							fflush(stderr);
							exit(1);
						}
						directive.begin[j] = fmt[i];
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
						for(; fmt[i+j] == '}'; ++j);
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
						fputs(directive.begin, stderr);
						fputs("` in format string.\n", stderr);
						fflush(stderr);
						exit(1);
					}

					Fmt_Info info = {.user_ptr = global_format_user_ptr};
					cursor += fmt_proc( (Byte_Slice){cursor, dest.end}, params, info);
				}
			break;case '}':
				if (fmt[i + 1] == '}') {
					*cursor++ = fmt[i++];
				} else {
					fputs("[ERROR]: unmatched format delimiter `}` in format string:\n`", stderr);
					fputs(fmt, stderr);
					fputs("`\n", stderr);
					fflush(stderr);
					exit(1);
				}
			break;default:
				*cursor++ = fmt[i];
		}
	}

	fputs("[ERROR]: format buffer is full, output may be truncated\n", stderr);
	fflush(stderr);
	exit(1);
}

void print(String s) {
	fwrite(s.begin, 1, s.end - s.begin, stdout);
}

void println(String s) {
	print(s);
	fputc('\n', stdout);
}

// TODO: factor out common behaviour
String format(cstring fmt, ...) {
	va_list params;
	va_start(params, fmt);
	String result = format_to_va(default_format_buffer, fmt, params);
	va_end(params);
	return result;
}

void   format_fprint(FILE *stream, cstring fmt, ...) {
	va_list params;
	va_start(params, fmt);
	String result = format_to_va(default_format_buffer, fmt, params);
	va_end(params);
	fwrite(result.begin, 1, result.end - result.begin, stream);
}

void   format_fprintln(FILE *stream, cstring fmt, ...) {
	va_list params;
	va_start(params, fmt);
	String result = format_to_va(default_format_buffer, fmt, params);
	va_end(params);
	fwrite(result.begin, 1, result.end - result.begin, stream);
	fputc( '\n', stream);
}

void format_print(cstring fmt, ...) {
	va_list params;
	va_start(params, fmt);
	String result = format_to_va(default_format_buffer, fmt, params);
	va_end(params);
	print(result);
}

void format_println(cstring fmt, ...) {
	va_list params;
	va_start(params, fmt);
	String result = format_to_va(default_format_buffer, fmt, params);
	va_end(params);
	println(result);
}

char default_format_buffer_data[4096];
Byte_Slice default_format_buffer = {
	default_format_buffer_data,
	default_format_buffer_data + sizeof(default_format_buffer_data)
};

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
