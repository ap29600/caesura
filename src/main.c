#include <assert.h>
#include <stdarg.h>
#include <stdlib.h>

#include "lib/builtin.h"
#include "lib/scanner/module.h"
#include "lib/format/module.h"
#include "lib/string/module.h"

#include "src/runtime/runtime.h"
#include "src/parser/parser.h"
#include "src/formats/formats.h"

#include "src/eval/eval.h"
#include "src/eval/ir.h"
#include "src/eval/ast.h"

i32 main () {
	init_formats();
	init_default_scope();

	char line[1024];
	while (true) {
		format_print("(caesura)> ");

		if (fgets(line,1024, stdin) == NULL) { break; }
		line[strlen(line) - 1] = '\0';
		String src = string_from_cstring(line);
		if (empty_string(src)) { break; }

		Scanner scanner = {.source = src, .location.fname = "stdin"};
		Ast ast = parse_expressions(&scanner);
		// set_format_user_ptr(ast.nodes);
		// format_println("desugars to: {expr}", ast.nodes[ast.parent]);

		Eval_Context ctx = {.scope = &default_scope};
		Node_Handle expr = apply(&ctx, ast.nodes, ast.parent);

		// TODO: extract
		for (u64 i = 0; i < ast.count; ++i) {
			if (ast.nodes[i].type == Ast_Type_Array_Ptr) {
				release_array(ast.nodes[i].as_Array_Ptr);
			}
			ast.nodes[i] = (Ast_Node){0};
		}
		free(ast.nodes);

		assert(expr == (Node_Handle)ctx.count-1);
		(void)expr;

		IR_Node result = flat_eval(&ctx);
		format_println("\t{node}", result);
		release_node(&result);

		// TODO: extract
		for(u64 i = 0; i < ctx.count; ++i) {
			ctx.nodes[i].ref_count = 1; // dirty hack
			release_node(&ctx.nodes[i]);
		}
		free(ctx.nodes);
	}
}
