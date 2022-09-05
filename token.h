#include "lib/string_types.h"
#include "lib/parsing_types.h"
#include "lib/format_types.h"
#include "lib/bit_set.h"

typedef enum {
    Empty,
    Float,
    Operator,
    Identifier,
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
        f64 value;
    };
    Location loc;
    bool is_valid;
} Token;

Token_Type classify(Parser_State *parser);
i64 fmt_token_va(Byte_Slice dest, va_list va, Fmt_Info info);
Token next_token(Parser_State *parser);
String parse_operator(Parser_State *parser, Bit_Set whitespace, Bit_Set specials);
