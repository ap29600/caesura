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
    while(line_end < end     && *line_end != '\n')       line_end++;

    String spaces = string_from("                              ");
    String before_err = {line_begin, begin + parsed_bytes(parser)};
    String after_err = {begin + parsed_bytes(parser), line_end};

    format_println("{loc}: [ERROR `" COL_RED_F "{error}" RESET "`]", parser->location, parser->error);
    format_println(COL_BLU_F "{str}" COL_RED_F "{str}" RESET, before_err, after_err);
    format_println("{str}^", slice(spaces, -1, parser->location.col));
}



static double pow_(double base, int expt) {
    double result = 1;

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


int parse_sign(Parser_State *parser) {
    int result;
    switch(peek(parser)) {
        case '+': next(parser); result =  1; break;
        case '-': next(parser); result = -1; break;
        default:                    result =  1; break;
    }
    parser->error = parser_is_empty(parser) ? None : Trailing_Chars;
    return result;
}

#define NaN (0.0/0.0);
double parse_double(Parser_State *parser) {
    double sign = (double)parse_sign(parser);
    switch (parser->error) {
        case Trailing_Chars: break;
        case None:
            parser->error = Truncated_Literal;
            return NaN;
        default: return NaN;
    }

    double integral = (double)parse_u64(parser);
    switch (parser->error) {
        case Trailing_Chars: break;
        case None: return sign * integral;
        default: return NaN;
    }

    double decimal = 0.0;
    if (peek(parser) == '.') {
        next(parser);
        decimal = parse_decimal(parser);
        switch(parser->error) {
            case None: return sign * (integral + decimal);
            case Trailing_Chars: break;
            default: return NaN;
        }
    }

    int expt = 0;
    if (peek(parser) == 'e') {
        next(parser);
        expt = parse_sign(parser);
        switch(parser->error) {
            case Trailing_Chars: break;
            case None: parser->error = Truncated_Literal;
            default: return NaN;
        }
        const unsigned long state_before_expt = parsed_bytes(parser);
        expt *= (int)parse_u64(parser);
        switch(parser->error) {
            case Trailing_Chars: break;
            case None:           break;
            default: return NaN;
        }
        if (parsed_bytes(parser) == state_before_expt) {
            parser->error = Truncated_Literal;
            return NaN;
        }
    }

    bool is_double_precision = true;
    switch(peek(parser)) {
        case 'f':
            next(parser);
            is_double_precision = false;
            break;
        case 'd':
            next(parser);
            is_double_precision = true;
            break;
        default:;
    }

    if (parser_is_empty(parser)) {
        parser->error = None;
    } else {
        parser->error = Trailing_Chars;
    }

    double result = sign * (integral + decimal) * pow_(10.0, expt);
    result = is_double_precision ? result : (double)(float)result;

    return result;
}

/// given the string "1'234" returns 1234UL.
/// the empty string "" returns 0UL.
unsigned long parse_u64(Parser_State *parser) {
    unsigned long result = 0;
    for (rune c; (c = peek(parser)) != '\0'; next(parser)) {
        if ('0' <= c && c <= '9') {
            result = 10UL * result + (c - '0');
        } else if ( c == '\'' || c == '_') {
            // Nothing
        } else {
            parser->error = Trailing_Chars;
            return result;
        }
    }
    parser->error = None;
    return result;
}


/// given the string "123'4", returns 0.1234d.
/// the empty string ""       returns 0.0d.
double parse_decimal(Parser_State *parser) {
    double result = 0.0;
    double unit   = 1.0;

    for (char c; (c = peek(parser)) != '\0'; next(parser)) {
        if ('0' <= c && c <= '9') {
            result += (unit /= 10UL) * (c - '0');
        } else if ( c == '\'' || c == '_') {
            // Nothing
        } else {
            parser->error = Trailing_Chars;
            return result;
        }
    }

    parser->error = None;
    return result;
}

