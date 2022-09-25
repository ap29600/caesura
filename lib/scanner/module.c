#include "module.h"
#include "../format/module.h"
#include "../string/module.h"

#define CONSOLE_COLORS
#ifdef CONSOLE_COLORS
	#define RESET "\x1b[0m"
	#define COL_RED_F "\x1b[31m"
	#define COL_GRN_F "\x1b[32m"
	#define COL_BLU_F "\x1b[34m"
#else
	#define RESET ""
	#define COL_RED_F ""
	#define COL_GRN_F ""
	#define COL_BLU_F ""
#endif

void report_state(Scanner *scanner, FILE *stream) {
	const char *begin = scanner->source.begin;
	const char *end   = scanner->source.end;

	const char *line_begin = begin + scanned_bytes(scanner);
	const char *line_end   = begin + scanned_bytes(scanner);

	// delimit the line;
	while(line_begin > begin && *(line_begin-1) != '\n') --line_begin;
	while(line_end   < end   && *line_end       != '\n') ++line_end;

	String before_err = {line_begin, begin + scanned_bytes(scanner)};
	String after_err = {begin + scanned_bytes(scanner), line_end};

	format_fprintln(stream, "{loc}: [ERROR `" COL_RED_F "{error}" RESET "`]", scanner->location, scanner->error);
	format_fprintln(stream, COL_BLU_F "{str}" COL_RED_F "{str}" RESET, before_err, after_err);

	u64 col = scanner->location.col;
	for(u64 i = 0; i < col; ++i) { fputc(' ', stream); }
	fputs("^\n", stream);
}



static f64 pow_(f64 base, i32 expt) {
	f64 result = 1;

	if (expt < 0) {
		base = 1/base;
		expt = -expt;
	}

	for(; expt; expt >>= 1){
		if (expt & 1) { result *= base; }
		base *= base;
	}

	return result;
}


i32 parse_sign(Scanner *scanner) {
	switch(peek(scanner)) {
		break;case '+': next(scanner); scanner->error = None;          return  1;
		break;case '-': next(scanner); scanner->error = None;          return -1;
		break;default:                 scanner->error = Empty_Parse;   return  1;
	}
}

#define NaN (0.0/0.0);
f64 read_f64(Scanner *scanner) {
	f64 sign = (f64)parse_sign(scanner);
	bool has_sign = scanner->error == None;

	f64 integral = (f64)read_u64(scanner);
	bool has_digits = scanner->error == None;

	f64 decimal = 0.0;
	bool has_dot = false;
	if (peek(scanner) == '.') {
		has_dot = true;
		next(scanner);
		decimal = read_decimal(scanner);
		has_digits |= scanner->error == None;
	}

	if (!has_digits) {
		scanner->error = (has_sign || has_dot) ? Invalid_Parse : Empty_Parse;
		return NaN;
	}

	i32 expt = 0;
	if (peek(scanner) == 'e') {
		next(scanner);
		expt = parse_sign(scanner);
		// ignore errors, sign is optional and default is +.

		expt *= (i32)read_u64(scanner);
		// exponent is present but not valid
		if (scanner->error != None) {
			scanner->error = Invalid_Exponent;
			return NaN;
		}
	}

	bool is_f64_precision = true;
	switch(peek(scanner)) {
		break;case 'f':
			next(scanner);
			is_f64_precision = false;
		break;case 'd':
			next(scanner);
			is_f64_precision = true;
		break;default:;
	}

	scanner->error = None;
	f64 result = sign * (integral + decimal) * pow_(10.0, expt);
	result = is_f64_precision ? result : (f64)(f32)result;

	return result;
}

/// given the string "1'234" returns 1234UL.
/// the empty string "" returns 0UL.
u64 read_u64(Scanner *scanner) {
	rune c = peek(scanner);
	if (c < '0' || '9' < c) {
		scanner->error = Empty_Parse;
		return 0;
	}

	// first element is a valid digit, from now on every parse is valid
	scanner->error = None;
	u64 result = 0;
	for (rune c; (c = peek(scanner)) != '\0'; next(scanner)) {
		if ('0' <= c && c <= '9') {
			result = 10UL * result + (c - '0');
		} else if ( c == '\'' || c == '_') {
			// Nothing
		} else {
			break;
		}
	}
	return result;
}


/// given the string "123'4", returns 0.1234d.
/// the empty string ""       returns 0.0d.
f64 read_decimal(Scanner *scanner) {
	rune c = peek(scanner);
	if (c < '0' || '9' < c) {
		scanner->error = Empty_Parse;
		return 0;
	}

	// first element is a valid digit, from now on every parse is valid
	scanner->error = None;
	f64 result = 0;
	f64 unit   = 1.0;
	for (rune c; (c = peek(scanner)) != '\0'; next(scanner)) {
		if ('0' <= c && c <= '9') {
			result += (unit /= 10UL) * (c - '0');
		} else if ( c == '\'' || c == '_') {
			// Nothing
		} else {
			break;
		}
	}
	return result;
}

void ensure_total_read(Scanner *scanner, Bit_Set delimiters) {
	if (scanner->error == None) {
		rune r = peek(scanner);
		if (r != '\0' && !get_bit(delimiters, r)) {
			scanner->error = Invalid_Parse;
		}
	}
}

Scanner scanner_from_filename(const char *filename) {
	assert(filename);

	FILE *file = fopen(filename, "rb");
	if (!file) {
		format_println("[ERROR]: unable to open file `{cstr}`", filename);
		return (Scanner){0};
	}

	Scanner result = {
		.source = string_from_stream(file),
		.location = {.fname = filename},
		.error = None,
	};

	fclose(file);
	return result;
}
