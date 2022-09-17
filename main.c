#include "lib/builtin.h"
#include "lib/error.h"
#include "lib/parsing.h"
#include "lib/bit_set.h"
#include "lib/format.h"
#include "lib/string.h"

#include <assert.h>
#include <stdarg.h>
#include <stdlib.h>

#include "funcs.h"
#include "parser.h"
#include "array.h"
#include "token.h"
#include "eval.h"
#include "formats.h"

void dbg_lookup(Lookup_Scope *scope) {
    for (i64 i = 0; i < scope->count; i++) {
        Lookup_Entry entry = scope->entries[i];
        format_println("{cstr} : {ptr} : {ptr} : {ptr}", &entry.name[0], entry.as_monadic, entry.as_dyadic, entry.value.as.array);
    }
}

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

        Parser_State state = {.source = src, .location.fname = "stdin"};
        Ast ast = parse_expressions(&state);

        Eval_Context ctx = {.scope = &default_scope};
        Node_Handle expr = apply(&ctx, ast.nodes, ast.parent);
        Node_Handle parent = eval(&ctx, expr);

        format_println("\t{node}", ctx.nodes[parent]);

        for(int i = 0; i < ctx.count; ++i)
            release_node(&ctx, i);
        free(ctx.nodes);
    }
}
