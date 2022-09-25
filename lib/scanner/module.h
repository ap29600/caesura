#ifndef FZR_SCANNER
#define FZR_SCANNER

#include <stdio.h>
#include "types.h"
#include "../bit_set.h"

f64  read_f64     (Scanner *scanner);
f64  read_decimal (Scanner *scanner);
i32  read_sign    (Scanner *scanner);
u64  read_u64     (Scanner *scanner);
void report_state  (Scanner *scanner, FILE *stream);
void ensure_total_read(Scanner *scanner, Bit_Set delimiters);
Scanner scanner_from_filename(const char *filename);

static bool  scanner_is_empty (const Scanner *scanner);
static u64   scanned_bytes    (const Scanner *scanner);
static rune  peek            (const Scanner *scanner);
static rune  next            (      Scanner *scanner);

/// static inline functions

static bool scanner_is_empty(const Scanner *scanner) {
	const u64 len = scanner->source.end - scanner->source.begin;
	return scanner->location.byte >= len;
}

static u64 scanned_bytes(const Scanner *scanner) {
	return scanner->location.byte;
}

static rune peek(const Scanner *scanner) {
	if (!scanner_is_empty(scanner)) {
		// TODO: handle utf-8
		return scanner->source.begin[scanned_bytes(scanner)];
	}
	return '\0';
}

static rune next(Scanner *scanner) {
	const rune result = peek(scanner);
	const unsigned tab_width = 8;
	switch(result) {
		break;case '\0':
		break;case '\n':
			++scanner->location.byte;
			++scanner->location.row;
			scanner->location.col = 0;
		break;case '\t':
			++scanner->location.byte;
			scanner->location.col =
				((scanner->location.col / tab_width) + 1) * tab_width;
		break;default:
			scanner->location.byte += rune_width(result);
			++scanner->location.col;
	}
	return result;
}

#endif
