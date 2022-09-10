#ifndef PARSER_H
#define PARSER_H

#include "lib/parsing_types.h"
#include "eval.h"

typedef struct {
    Ast_Node *nodes;
    u64 count;
    u64 cap;
    Node_Handle parent;
} Ast;

Ast parse_expressions(Parser_State *state);


#endif // PARSER_H
