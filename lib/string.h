#ifndef FZR_STRING
#define FZR_STRING

#include "string_types.h"
#include <stdio.h>

void   delete_str          (String *s);
String string_from_stream  (FILE *in);
String string_split_rune   (String *it, const rune delim);
String string_split_any    (String *it, const rune *delims);
u64    string_index_rune   (String s, rune c);
u64    string_index_any    (String s, const rune *c);
u64    strlen_             (cstring s);

#endif
