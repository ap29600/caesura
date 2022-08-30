#include "lib/builtin.h"
#include "lib/error.h"
#include "lib/string.h"
#include "lib/parsing.h"
#include "lib/bit_set.h"
#include "lib/format.h"

#include <assert.h>
#include <stdarg.h>

const char *whitespace = " \t\n";

typedef enum {
    Empty,
    Float,
    Operator,
} Token_Type;

typedef struct {
    Token_Type type;
    union {
        String text;
        f64 value;
    };
    Location loc;
    bool is_valid;
} Token;

static Token_Type classify(Parser_State *parser) {
    if (parser_is_empty(parser)) return Empty;

    const char first = peek(parser);
    if ('0' <= first && first <= '9') return Float;

    if (first == '-' || first == '+' || first == '.') {
        Parser_State tmp = *parser;
        next(&tmp);
        const char second = peek(&tmp);
        if ('0' <= second && second <= '9') {
            return Float;
        }
    }

    return Operator;
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

i64 fmt_token_va(Byte_Slice dest, va_list va, Fmt_Info info) {
    info = (Fmt_Info){0};
    Token tok = va_arg(va, Token);
    unsigned char *const begin = dest.begin;

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
            dest.begin += fmt_cstr(dest, "op: " COL_BLU_F "`", info);
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

String parse_operator(Parser_State *parser, Bit_Set whitespace, Bit_Set specials) {
    u64 begin = parsed_bytes(parser);

    for(rune c; (c = peek(parser)) != '\0'; next(parser)) {
        if (get_bit(whitespace, c)) {
            parser->error = Trailing_Chars;
            return slice(parser->source, begin, parsed_bytes(parser));
        }

        if (get_bit(specials, c)) {
            if (parsed_bytes(parser) == begin) {
                next(parser);
                parser->error = parser_is_empty(parser) ? None : Trailing_Chars;
            } else {
                parser->error = Trailing_Chars;
            }
            return slice(parser->source, begin, parsed_bytes(parser));
        }
    }

    parser->error = None;
    return slice(parser->source, begin, parsed_bytes(parser));
}

Token next_token(Parser_State *parser) {
    static u64 whitespace_data[256 / BITS];
    static Bit_Set whitespace;
    if (whitespace.data == NULL)
        whitespace = bit_set_from_runes(whitespace_data, U"\n\t ");

    static u64 specials_data[256 / BITS];
    static Bit_Set specials;
    if (specials.data == NULL)
        specials = bit_set_from_runes(specials_data, U",.()");

    while(get_bit(whitespace, peek(parser))) next(parser);

    Location loc = parser->location;
    switch(classify(parser)) {
        case Empty: {
            return (Token){ .type = Empty, .loc = loc, .is_valid = false };
        } break;
        case Float: {
            f64 value = parse_f64(parser);
            switch(parser->error) {
                case None: // fallthrough
                case Trailing_Chars: {
                    char trail = peek(parser);
                    if (get_bit(whitespace, trail) || get_bit(specials, trail))
                        return (Token){Float, .value = value, loc, true};
                } // fallthrough;
                default: {
                    report_state(parser, stderr);
                    for (char c; (c = peek(parser)) != '\0'; next(parser))
                        if (get_bit(whitespace, c) || get_bit(specials, c))
                            break;
                    String text = slice(parser->source, loc.byte, parsed_bytes(parser));
                    parser->error = None;
                    return (Token){Float, .text = text, loc, false};
                } break;
            }
        } break;
        case Operator: {
            String tok_text = parse_operator(parser, whitespace, specials);
            return (Token){Operator, .text = tok_text, loc, true};
        } break;
        default: assert(false);
    }
}

Parser_State parser_from_filename(const char *filename) {
    if (!filename) {
        println(string_from("[ERROR]: no filename was provided"));
        return (Parser_State){0};
    }

    FILE *file = fopen(filename, "rb");
    if (!file) {
        format_println("[ERROR]: unable to open file `{cstr}`", filename);
        return (Parser_State){0};
    }

    Parser_State result = {
        .source = string_from(file),
        .location = {.fname = filename},
        .error = None,
    };
    fclose(file);
    return result;
}

i32 main(i32 argc, char **argv) {
    register_format_directive((Fmt_Directive){"Token", fmt_token_va});

    Parser_State parser = parser_from_filename(argv[1]);
    for (; !parser_is_empty(&parser); ) {
        Token tok = next_token(&parser);
        if (tok.type == Empty) continue;
        if (!tok.is_valid) format_println("{Token}", tok);
    }
    delete_str(&parser.source);
    return 0;
}
