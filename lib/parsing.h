#ifndef FZR_PARSING
#define FZR_PARSING

#include "string.h"
#include "error.h"

typedef struct {
    unsigned int  row;
    unsigned int  col;
    unsigned long byte;
    const char *fname;
} Location;

typedef struct {
    String   source;
    Location location;
    Error    error;
} Parser_State;

/// forward declarations
double        parse_double  (Parser_State *parser);
double        parse_decimal (Parser_State *parser);
int           parse_sign    (Parser_State *parser);
unsigned long parse_u64     (Parser_State *parser);
void          report_state  (Parser_State *parser, FILE *stream);

static inline bool          parser_is_empty (const Parser_State *parser);
static inline unsigned long parsed_bytes    (const Parser_State *parser);
static inline rune          peek            (const Parser_State *parser);
static inline rune          next            (      Parser_State *parser);

/// static inline functions

static inline bool parser_is_empty(const Parser_State *parser) {
    const unsigned long len = parser->source.end - parser->source.begin;
    return parser->location.byte >= len;
}

static inline unsigned long parsed_bytes(const Parser_State *parser) {
    return parser->location.byte;
}

static inline rune peek(const Parser_State *parser) {
    if (!parser_is_empty(parser)) {
        return parser->source.begin[parsed_bytes(parser)];
    }
    return '\0';
}

static inline rune next(Parser_State *parser) {
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
            parser->location.byte++;
            parser->location.col++;
            break;
    }
    return result;
}

#endif
