#ifndef PARSER_H
#define PARSER_H

#include "lib/scanner/types.h"
#include "src/eval/eval.h"
#include "src/eval/ast.h"

Ast parse_expressions(Scanner *scanner);

#endif // PARSER_H
