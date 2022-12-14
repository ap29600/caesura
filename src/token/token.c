#include "lib/scanner/module.h"
#include "lib/format/module.h"
#include "lib/string/module.h"
#include "lib/bit_set.h"

#include "token.h"


Token_Type classify(Scanner *scanner) {
	switch(peek(scanner)) {
		break;case '\0':
			return Token_Type_Empty;

		break;case '0': case '1': case '2': case '3': case '4':
			case '5': case '6': case '7': case '8': case '9':
			return Token_Type_Float;

		break;case '.': case ',': case '(': case ')': case ':': case '!':
			return Token_Type_Operator;

		break;case '-': case '+': {
			Scanner tmp = *scanner;
			next(&tmp);
			switch(peek(&tmp)) {
				break;case '0': case '1': case '2': case '3': case '4':
					case '5': case '6': case '7': case '8': case '9':
					return Token_Type_Float;

				break;default:
					return Token_Type_Identifier;
			}
		}

		break;default:
			return Token_Type_Identifier;
	}
	unreachable();
}
cstring operator_strings[Num_Operators] = {
	[List]      = "List",
	[Monad]     = "Monad",
	[Dyad]      = "Dyad",
	[Assign]    = "Assign",
	[Immediate] = "Immediate",
	[LParen]    = "LParen",
	[RParen]    = "RParen",
};


static bool    bit_sets_initialized = false;
static Bit_Set whitespace = {(u64[256 / BITS]){0}};
static Bit_Set specials   = {(u64[256 / BITS]){0}};
static Bit_Set delimiters = {(u64[256 / BITS]){0}};

#define WHITESPACE U"\n\t "
#define SPECIALS   U",.:!()"
#define DELIMITERS WHITESPACE SPECIALS

static void initialize_bit_sets() {
	bit_sets_initialized = true;
	whitespace = bit_set_from_runes(whitespace.data, WHITESPACE);
	specials   = bit_set_from_runes(specials.data,   SPECIALS);
	delimiters = bit_set_from_runes(delimiters.data, DELIMITERS);
}

Token next_token(Scanner *scanner) {
	if (!bit_sets_initialized) { initialize_bit_sets(); }

	Token result = {0};

	while(get_bit(whitespace, peek(scanner))) next(scanner);
	result.loc = scanner->location;
	result.is_valid = true;

	switch (classify(scanner)) {

		break;case Token_Type_Empty: {
			result.type = Token_Type_Empty;
			result.is_valid = false;
		}

		break;case Token_Type_Int:
		[[fallthrough]];case Token_Type_Float: {
			f64 value = read_f64(scanner);
			ensure_total_read(scanner, delimiters);
			if ((value - (i64)value) != 0) {
				result.type     = Token_Type_Float;
				result.f64value = value;
			} else {
				result.type = Token_Type_Int;
				result.i64value = (i64)value;
			}

			if (scanner->error != None) {
				while(peek(scanner) && !get_bit(delimiters, peek(scanner))) next(scanner);
				result.is_valid = false;
				result.text = (String){
					.begin = &scanner->source.begin[result.loc.byte],
					.end =   &scanner->source.begin[scanner->location.byte]
				};
			}
		}

		break;case Token_Type_Operator: {
			result.type = Token_Type_Operator;
			switch (next(scanner)) {
				break;case ',':
					result.op = List;
				break;case '.':
					result.op = Monad;
				break;case ':':
					if (peek(scanner) == ':') {
						next(scanner);
						result.op = Assign;
					} else {
						result.op = Dyad;
					}
				break;case '!':
					result.op = Immediate;
				break;case '(':
					result.op = LParen;
				break;case ')':
					result.op = RParen;
				break;default:
					assert(false);
			}
		}

		break;case Token_Type_Identifier: {
			result.type = Token_Type_Identifier;
			for (; peek(scanner) != '\0' && !get_bit(delimiters, peek(scanner)); next(scanner)){};
			result.text = (String){
				.begin = &scanner->source.begin[result.loc.byte],
				.end =   &scanner->source.begin[scanner->location.byte]
			};
		}
	}

	return result;
}
