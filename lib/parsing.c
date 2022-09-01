#include "parsing.h"
#include "format.h"

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

void report_state(Parser_State *parser, FILE *stream) {
    const char *begin = parser->source.begin;
    const char *end   = parser->source.end;

    const char *line_begin = begin + parsed_bytes(parser);
    const char *line_end   = begin + parsed_bytes(parser);

    // delimit the line;
    while(line_begin > begin && *(line_begin-1) != '\n') line_begin--;
    while(line_end   < end   && *line_end       != '\n') line_end++;

    String before_err = {line_begin, begin + parsed_bytes(parser)};
    String after_err = {begin + parsed_bytes(parser), line_end};

    format_println("{loc}: [ERROR `" COL_RED_F "{error}" RESET "`]", parser->location, parser->error);
    format_println(COL_BLU_F "{str}" COL_RED_F "{str}" RESET, before_err, after_err);

    u64 col = parser->location.col;
    while (col--) { putchar(' '); }
    puts("^\n");
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


i32 parse_sign(Parser_State *parser) {
    switch(peek(parser)) {
        case '+': next(parser); parser->error = None;          return  1;
        case '-': next(parser); parser->error = None;          return -1;
        default:                parser->error = Empty_Parse;   return  1;
    }
}

#define NaN (0.0/0.0);
f64 parse_f64(Parser_State *parser) {
    f64 sign = (f64)parse_sign(parser);
    bool has_sign = parser->error == None;

    f64 integral = (f64)parse_u64(parser);
    bool has_digits = parser->error == None;

    f64 decimal = 0.0;
    bool has_dot = false;
    if (peek(parser) == '.') {
        has_dot = true;
        next(parser);
        decimal = parse_decimal(parser);
        has_digits |= parser->error == None;
    }

    if (!has_digits) {
        parser->error = (has_sign || has_dot) ? Invalid_Parse : Empty_Parse;
        return NaN;
    }

    i32 expt = 0;
    if (peek(parser) == 'e') {
        next(parser);
        expt = parse_sign(parser);
        // ignore errors, sign is optional and default is +.

        expt *= (i32)parse_u64(parser);
        // exponent is present but not valid
        if (parser->error != None) {
            parser->error = Invalid_Exponent;
            return NaN;
        }
    }

    bool is_f64_precision = true;
    switch(peek(parser)) {
        case 'f':
            next(parser);
            is_f64_precision = false;
            break;
        case 'd':
            next(parser);
            is_f64_precision = true;
            break;
        default:;
    }

    parser->error = None;
    f64 result = sign * (integral + decimal) * pow_(10.0, expt);
    result = is_f64_precision ? result : (f64)(f32)result;

    return result;
}

/// given the string "1'234" returns 1234UL.
/// the empty string "" returns 0UL.
u64 parse_u64(Parser_State *parser) {
    rune c = peek(parser);
    if (c < '0' || '9' < c) {
        parser->error = Empty_Parse;
        return 0;
    }

    // first element is a valid digit, from now on every parse is valid
    parser->error = None;
    u64 result = 0;
    for (rune c; (c = peek(parser)) != '\0'; next(parser)) {
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
f64 parse_decimal(Parser_State *parser) {
    rune c = peek(parser);
    if (c < '0' || '9' < c) {
        parser->error = Empty_Parse;
        return 0;
    }

    // first element is a valid digit, from now on every parse is valid
    parser->error = None;
    f64 result = 0;
    f64 unit   = 1.0;
    for (rune c; (c = peek(parser)) != '\0'; next(parser)) {
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

