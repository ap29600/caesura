#include <assert.h>
#include <stdarg.h>
#include <unistd.h>
#include <stdio.h>

#include "lib/builtin.h"
#include "lib/scanner/module.h"
#include "lib/format/module.h"
#include "lib/string/module.h"
#include "lib/vector/vector.h"
#include "lib/algorithm/module.h"

#include "src/runtime/runtime.h"
#include "src/parser/parser.h"
#include "src/formats/formats.h"

// TODO: extract this functionality into a library
String read_line_into(char *buf, usize size) {
	if (fgets(buf, size, stdin) == NULL) { return (String){0}; }
	buf[strlen(buf) - 1] = '\0';
	return string_from_cstring(buf);
}

i32 main () {
	init_formats();
	init_default_scope();

	Eval_Context ctx = { .scope = &default_scope };

	for (;;) {
		// only print the prompt if running in interactive mode
		if (isatty(0)) { format_print("{col}[csr]>{col} ", Term_Col_FG_Green, Term_Col_Reset); }

		char line[1024];
		String src = read_line_into(line, sizeof(line));
		if (feof(stdin)) { break; }
		if (empty_string(src)) { continue; }

		Scanner scanner = { .source = src, .location.fname = "stdin" };

		Ast ast = parse_expressions(&scanner);
		apply(&ctx, ast.nodes, ast.parent);
		delete_ast(&ast);

		IR_Node result = flat_eval(&ctx);
		format_println("\t{node}", result);
		release_node(&result);

		flush_eval_context(&ctx);
	}

	delete_eval_context(&ctx);
	return EXIT_SUCCESS;
}
