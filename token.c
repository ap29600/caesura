#include "lib/parsing.h"
#include "lib/format.h"
#include "lib/string.h"
#include "lib/bit_set.h"

#include "token.h"


Token_Type classify(Parser_State *parser) {
    switch(peek(parser)) {
        case '\0':
            return Empty;

        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':
            return Float;

        case '.': case ',': case '(': case ')': case '\'':
            return Operator;

        case '-': case '+': {
            Parser_State tmp = *parser;
            next(&tmp);
            switch(peek(&tmp)) {
                case '0': case '1': case '2': case '3': case '4':
                case '5': case '6': case '7': case '8': case '9':
                    return Float;

                default:
                    return Identifier;
            }
        }

        default:
            return Identifier;
    }
}

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

cstring operator_strings[Num_Operators] = {
    [List]   = "List",
    [Monad]  = "Monad",
    [Dyad]   = "Dyad",
    [Assign] = "Assign",
    [LParen] = "LParen",
    [RParen] = "RParen",
};

i64 fmt_token_va(Byte_Slice dest, va_list va, Fmt_Info info) {
    info = (Fmt_Info){0};
    Token tok = va_arg(va, Token);
    char *const begin = dest.begin;

    dest.begin += fmt_location(dest, tok.loc, info);
    dest.begin += fmt_cstr(dest, ": ", info);
    dest.begin += fmt_cstr(dest, "Token{ ", info);
    if (!tok.is_valid) {
        dest.begin += fmt_cstr(dest, "[" COL_RED_F "INVALID" RESET "] ", info);
    }

    switch (tok.type) {
        case Float:
            dest.begin += fmt_cstr(dest, "f64: " COL_GRN_F, info);
            dest.begin += fmt_i64(dest, (i64)tok.value, info);
            dest.begin += fmt_cstr(dest, RESET " ", info);
            break;

        case Operator:
            dest.begin += fmt_cstr(dest, "op: " COL_BLU_F, info);
            dest.begin += fmt_cstr(dest, operator_strings[tok.op], info);
            dest.begin += fmt_cstr(dest, RESET " ", info);
            break;

        case Identifier:
            dest.begin += fmt_cstr(dest, "identifier: " COL_BLU_F "`", info);
            dest.begin += fmt_str(dest, tok.text, info);
            dest.begin += fmt_cstr(dest, "`" RESET " ", info);
            break;

        case Empty:
            dest.begin += fmt_cstr(dest, "nil ", info);
            break;
    }

    dest.begin += fmt_cstr(dest, "}", info);
    return dest.begin - begin;
}

static bool    bit_sets_initialized = false;
static Bit_Set whitespace = {(u64[256 / BITS]){0}};
static Bit_Set specials   = {(u64[256 / BITS]){0}};
static Bit_Set delimiters = {(u64[256 / BITS]){0}};

#define WHITESPACE U"\n\t "
#define SPECIALS   U",.()"
#define DELIMITERS WHITESPACE SPECIALS

static void initialize_bit_sets() {
    bit_sets_initialized = true;
    whitespace = bit_set_from_runes(whitespace.data, WHITESPACE);
    specials   = bit_set_from_runes(specials.data,   SPECIALS);
    delimiters = bit_set_from_runes(delimiters.data, DELIMITERS);
}

Token next_token(Parser_State *parser) {
    if (!bit_sets_initialized)
        initialize_bit_sets();

    Token result = {0};

    while(get_bit(whitespace, peek(parser))) next(parser);
    result.loc = parser->location;
    result.is_valid = true;

    switch (classify(parser)) {

        case Empty: {
            result.type = Empty;
            result.is_valid = false;
        } break;

        case Float: {
            result.type = Float;
            result.value = parse_f64(parser);
            ensure_total_parse(parser, delimiters);

            if (parser->error != None) {
                while(peek(parser) && !get_bit(delimiters, peek(parser))) next(parser);
                result.is_valid = false;
                result.text = (String){
                    .begin = &parser->source.begin[result.loc.byte],
                    .end =   &parser->source.begin[parser->location.byte]
                };
            }
        } break;

        case Operator: {
            result.type = Operator;
            switch (peek(parser)) {
                case ',':
                    next(parser);
                    result.op = List;
                    break;
                case '.':
                    next(parser);
                    result.op = Monad;
                    break;
                case '\'':
                    next(parser);
                    result.op = Dyad;
                    break;
                case '(':
                    next(parser);
                    result.op = LParen;
                    break;
                case ')':
                    next(parser);
                    result.op = RParen;
                    break;
                case ':':
                    next(parser);
                    assert(next(parser) == '=');
                    result.op = Assign;
                    break;
                default:
                    assert(false);
                    break;
            }
        } break;

        case Identifier: {
            result.type = Identifier;
            while(peek(parser) != '\0' && !get_bit(delimiters, peek(parser))) next(parser);
            result.text = (String){
                .begin = &parser->source.begin[result.loc.byte],
                .end =   &parser->source.begin[parser->location.byte]
            };
        } break;

    }

    return result;
}
