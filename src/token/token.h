#ifndef TOKEN_H
#define TOKEN_H

#include "lib/string/types.h"
#include "lib/scanner/types.h"
#include "lib/format/types.h"
#include "lib/bit_set.h"

typedef enum {
	Token_Type_Empty,
	Token_Type_Int,
	Token_Type_Float,
	Token_Type_Operator,
	Token_Type_Identifier,
} Token_Type;

typedef enum {
	List,
	Monad,
	Dyad,
	Assign,
	Immediate,
	LParen,
	RParen,
	Num_Operators,
} Operator_t;

extern cstring operator_strings[Num_Operators];

typedef struct {
	Token_Type type;
	union {
		String text;
		Operator_t op;
		f64 f64value;
		i64 i64value;
	};
	Location loc;
	bool is_valid;
} Token;

Token_Type classify(Scanner *scanner);
Token next_token(Scanner *scanner);
String parse_operator(Scanner *scanner, Bit_Set whitespace, Bit_Set specials);

#endif // TOKEN_H
