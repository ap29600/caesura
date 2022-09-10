#include "lib/builtin.h"
#include "lib/error.h"
#include "lib/parsing.h"
#include "lib/bit_set.h"
#include "lib/format.h"
#include "lib/string.h"

#include <assert.h>
#include <stdarg.h>
#include <stdlib.h>

#include "parser.h"
#include "array.h"
#include "token.h"
#include "eval.h"
#include "formats.h"

#define MAX_TOKENS 1024

i32 main () {
    init_formats();
    char line[1024];

    while (true) {
        format_print("(caesura)> ");

        if (fgets(line,1024, stdin) == NULL) break;
        line[strlen(line) - 1] = '\0';
        String src = string_from_cstring(line);
        if (empty_string(src)) break;

        Parser_State state = {.source = src, .location.fname = "stdin"};
        Ast ast = parse_expressions(&state);

        Eval_Context ctx = {0};
        Node_Handle expr = apply(&ctx, ast.nodes, ast.parent);

        Node_Handle parent = eval(&ctx, expr);

        format_println("\t{node}", ctx.nodes[parent]);

        for(int i = 0; i < ctx.count; ++i)
            release_node(&ctx, i);
        free(ctx.nodes);
    }
}
