#ifndef FZR_STRING
#define FZR_STRING

#include <stdio.h>
#include "types.h"

void   delete_str          (String *s);
String string_from_stream  (FILE *in);
String string_split_rune   (String *it, const rune delim);
String string_split_any    (String *it, const rune *delims);
u64    string_index_rune   (String s, rune c);
u64    string_index_any    (String s, const rune *c);
bool   string_eq_string(String a, String b);
bool   string_eq_cstring(String a, cstring b);

#define string_eq(a, b)                     \
	_Generic((b),                           \
				 String: string_eq_string,  \
				 char *: string_eq_cstring, \
				 cstring: string_eq_cstring \
			 )(a, b)

#endif
