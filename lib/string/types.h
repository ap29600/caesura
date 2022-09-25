#ifndef FZR_STRING_TYPES
#define FZR_STRING_TYPES

#include "../builtin.h"
#include <assert.h>
#include <string.h>

typedef struct {
	cstring begin;
	cstring end;
} String;

typedef struct {
	char begin[16];
} Short_String;

static u64 string_len(String s) {
	return s.end - s.begin;
}

static u64 rune_width(rune r) {
	if (r < 0x000080) { return 1; }
	if (r < 0x000800) { return 2; }
	if (r < 0x010000) { return 3; }
	if (r < 0x110000) { return 4; }
	unreachable();
}

static String string_from_cstring(cstring s) {
	return (String){s, s + strlen(s)};
}

static String slice(String s, i64 begin, i64 end) {
	begin = begin >= 0 ? begin : 0;
	end   = end   >= 0 ? end   : s.end - s.begin;

	assert(begin <= s.end - s.begin);
	assert(end   <= s.end - s.begin);
	assert(begin <= end);

	return (String) {
		.begin = s.begin + begin,
		.end   = s.begin + end, 
	};
}

static bool empty_string(String s) {
	return s.end == s.begin;
}

static bool empty_cstring(cstring s) {
	return s == NULL || *s == '\0';
}

#endif

