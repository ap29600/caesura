#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#include "module.h"

#include "../bit_set.h"

u64 string_index_rune(String s, rune c) {
	// TODO: handle utf-8 decoding
	assert(rune_width(c) == 1);

	// TODO: vectorize this loop when c is ascii
	for (u64 i = 0; i < (u64)(s.end - s.begin); ++i) {
		if ((rune)s.begin[i] == c) {
			return i;
		}
	}
	return s.end - s.begin;
}

u64 string_index_any(String s, const rune *c) {
	if (c) {
		#define SIZE ((256 - 1)/BITS + 1)
		Bit_Set fastmap = bit_set_from_runes((u64[SIZE]){0}, c);
		#undef SIZE

		for (const rune *j = c; *j; ++j) {
			// TODO: handle utf-8 decoding
			assert(rune_width(*j) == 1);
			set_bit(&fastmap, *j, true);
		}

		u64 i = 0;
		for (; s.begin < s.end && !get_bit(fastmap, *s.begin); ++s.begin) {
			++i;
		}
		return i;
	}
	return s.end - s.begin;
}

String string_split_rune(String *it, rune delim) {
	String result = (String){
		.begin = it->begin,
		.end   = it->begin + string_index_rune(*it, delim),
	};
	if (result.end < it->end) {
		it->begin += rune_width(*it->begin);
	}
	return result;
}

String string_split_any(String *it, const rune *delims) {
	String result = (String){
		.begin = it->begin,
		.end   = it->begin + string_index_any(*it, delims),
	};
	if (result.end < it->end) {
		it->begin += rune_width(*it->begin);
	}
	return result;
}

void delete_str(String *s) {
	free((void *)s->begin);
	*s = (String){0};
}

String string_from_stream(FILE *is) {
	if (fseek(is, 0, SEEK_END) >= 0) {
		u64 cap  = ftell(is);
		fseek(is, 0, SEEK_SET);
		char *data = malloc(cap);
		u64 len = fread(data, 1, cap, is);
		assert(len == cap);
		return (String){data, data + len};
	} else {
		u64 cap = 8;
		u64 len = 0;
		u64 num;
		char *data = malloc(cap);

		while ((num = fread(data + len, 1, cap - len, is)) > 0) {
			len += num;
			if (len >= cap) {
				cap *= 2;
				data = realloc(data, cap);
			}
		}

		if (!feof(is)) {
			fprintf(stderr, "[ERROR] reading stream\n");
		}

		data = realloc(data, len);
		return (String){data, data + len};
	}
}

bool string_eq_cstring(String a, const char *b) {
	u64 len = string_len(a);
	for (u64 i = 0; i < len; ++i) {
		if (b[i] != a.begin[i]) { return false; }
	}
	return b[len] == '\0';
}

bool string_eq_string(String a, String b) {
	u64 a_len = string_len(a);
	u64 b_len = string_len(b);

	if (a_len != b_len) { return false; }
	return memcmp(a.begin, b.begin, a_len) == 0;
}
