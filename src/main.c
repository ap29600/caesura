#include <assert.h>
#include <stdarg.h>
#include <stdlib.h>

#include "lib/builtin.h"
#include "lib/scanner/module.h"
#include "lib/format/module.h"
#include "lib/string/module.h"

#include "src/funcs/funcs.h"
#include "src/parser/parser.h"
#include "src/formats/formats.h"

i32 main () {
    init_formats();
    init_default_scope();

    char line[1024];
    while (true) {
        format_print("(caesura)> ");

        if (fgets(line,1024, stdin) == NULL) break;
        line[strlen(line) - 1] = '\0';
        String src = string_from_cstring(line);
        if (empty_string(src)) break;

        Scanner scanner = {.source = src, .location.fname = "stdin"};
        Ast ast = parse_expressions(&scanner);
        set_format_user_ptr(ast.nodes);
        format_println("desugars to: {expr}", ast.nodes[ast.parent]);

        Eval_Context ctx = {.scope = &default_scope};
        Node_Handle expr = apply(&ctx, ast.nodes, ast.parent);

        // TODO: extract
        for (int i = 0; i < ast.count; i++) {
            if (ast.nodes[i].type == Node_Array) {
                release_array(ast.nodes[i].as.array);
            }
            ast.nodes[i] = (Ast_Node){0};
        }
        free(ast.nodes);

        assert(expr == ctx.count-1);

        Eval_Node result = flat_eval(&ctx);
        format_println("\t{node}", result);
        release_node(&result);

        // TODO: extract
        for(int i = 0; i < ctx.count; ++i) {
            ctx.nodes[i].ref_count = 1; // dirty hack
            release_node(&ctx.nodes[i]);
        }
        free(ctx.nodes);
    }
}
