#include "lib/builtin.h"
#include "lib/error.h"
#include "lib/parsing.h"
#include "lib/bit_set.h"
#include "lib/format.h"
#include "lib/string.h"

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

static i64 fmt_token_va(Byte_Slice dest, va_list va, Fmt_Info info) {
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

static String parse_operator(Parser_State *parser, Bit_Set whitespace, Bit_Set specials) {
    u64 begin = parsed_bytes(parser);
    rune c = peek(parser);
    parser->error = None;

    // empty operator
    if (c == '\0' || get_bit(whitespace, c)) {
        parser->error = Invalid_Parse;
        return slice(parser->source, begin, parsed_bytes(parser));
    }

    // consume a special rune
    if (get_bit(specials, c)) {
        next(parser);
        return slice(parser->source, begin, parsed_bytes(parser));
    }

    next(parser);
    for(; (c = peek(parser)) != '\0'; next(parser)) {
        if (get_bit(whitespace, c) || get_bit(specials, c)) { break; }
    }

    return slice(parser->source, begin, parsed_bytes(parser));
}

static void ensure_total_parse(Parser_State *parser, Bit_Set whitespace, Bit_Set specials) {
    if (parser->error == None) {
        rune r = peek(parser);
        if (!get_bit(whitespace, r) && !get_bit(specials, r)) {
            parser->error = Invalid_Parse;
        }
    }
}

static Token next_token(Parser_State *parser) {
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
            ensure_total_parse(parser, whitespace, specials);
            if (parser->error == None) {
                return (Token){Float, .value = value, .loc = loc, .is_valid = true};
            } else {
                // report_state(parser, stderr);
                for (char c; (c = peek(parser)) != '\0'; next(parser))
                    if (get_bit(whitespace, c) || get_bit(specials, c))
                        break;
                String text = slice(parser->source, loc.byte, parsed_bytes(parser));
                parser->error = None;
                return (Token){Float, .text = text, loc, false};
            }
        } break;
        case Operator: {
            String tok_text = parse_operator(parser, whitespace, specials);
            return (Token){Operator, .text = tok_text, loc, true};
        } break;
        default: assert(false);
    }
}

static Parser_State parser_from_filename(const char *filename) {
    if (!filename) {
        println(string_from_cstring("[ERROR]: no filename was provided"));
        return (Parser_State){0};
    }

    FILE *file = fopen(filename, "rb");
    if (!file) {
        format_println("[ERROR]: unable to open file `{cstr}`", filename);
        return (Parser_State){0};
    }

    Parser_State result = {
        .source = string_from_stream(file),
        .location = {.fname = filename},
        .error = None,
    };
    fclose(file);
    return result;
}

i32 main(i32 argc, char **argv) {
    register_format_directive((Fmt_Directive){"Token", fmt_token_va});

    u64 valid_float   = 0;
    u64 valid_op      = 0;
    u64 invalid_float = 0;
    u64 invalid_op    = 0;
    u64 empty         = 0;

    Parser_State parser = parser_from_filename(argv[1]);
    for (; !parser_is_empty(&parser); ) {
        Token tok = next_token(&parser);

        switch(tok.type) {
            case Float:
                if (tok.is_valid) ++valid_float;
                else ++invalid_float;
                break;
            case Operator:
                if (tok.is_valid) ++valid_op;
                else ++invalid_op;
                break;
            case Empty:
                ++empty;
                break;
        }
    }

    format_print("float:\n"
                 "\tvalid:   {u64}\n"
                 "\tinvalid: {u64}\n",
                 valid_float, invalid_float);
    format_print("op:\n"
                 "\tvalid:   {u64}\n"
                 "\tinvalid: {u64}\n",
                 valid_op, invalid_op);
    format_print("empty: {u64}\n", empty);

    delete_str(&parser.source);
    return 0;
}
