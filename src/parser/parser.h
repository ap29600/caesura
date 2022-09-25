#ifndef PARSER_H
#define PARSER_H

#include "lib/scanner/types.h"
#include "src/eval/eval.h"

typedef struct {
	Ast_Node *nodes;
	u64 count;
	u64 cap;
	Node_Handle parent;
} Ast;

Ast parse_expressions(Scanner *scanner);

#endif // PARSER_H
