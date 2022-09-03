#ifndef FZR_PARSING
#define FZR_PARSING

#include <stdio.h>
#include "parsing_types.h"
#include "bit_set.h"

f64  parse_f64     (Parser_State *parser);
f64  parse_decimal (Parser_State *parser);
i32  parse_sign    (Parser_State *parser);
u64  parse_u64     (Parser_State *parser);
void report_state  (Parser_State *parser, FILE *stream);
void ensure_total_parse(Parser_State *parser, Bit_Set delimiters);
Parser_State parser_from_filename(const char *filename);

static bool  parser_is_empty (const Parser_State *parser);
static u64   parsed_bytes    (const Parser_State *parser);
static rune  peek            (const Parser_State *parser);
static rune  next            (      Parser_State *parser);

/// static inline functions

static bool parser_is_empty(const Parser_State *parser) {
    const u64 len = parser->source.end - parser->source.begin;
    return parser->location.byte >= len;
}

static u64 parsed_bytes(const Parser_State *parser) {
    return parser->location.byte;
}

static rune peek(const Parser_State *parser) {
    if (!parser_is_empty(parser)) {
        // TODO: handle utf-8
        return parser->source.begin[parsed_bytes(parser)];
    }
    return '\0';
}

static rune next(Parser_State *parser) {
    const rune result = peek(parser);
    const unsigned tab_width = 8;
    switch(result) {
        case '\0':
            break;
        case '\n':
            parser->location.byte++;
            parser->location.row++;
            parser->location.col = 0;
            break;
        case '\t':
            parser->location.byte++;
            parser->location.col =
                ((parser->location.col / tab_width) + 1) * tab_width;
            break;
        default:
            parser->location.byte += rune_width(result);
            parser->location.col++;
            break;
    }
    return result;
}

#endif
