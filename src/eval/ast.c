#include <stdlib.h>

#include "ast.h"

void delete_ast(Ast *ast) {
	for (u64 i = 0; i < ast->count; ++i) {
		if (ast->nodes[i].type == Ast_Type_Array_Ptr) {
			release_array(ast->nodes[i].as_Array_Ptr);
		}
		ast->nodes[i] = (Ast_Node){0};
	}
	free(ast->nodes);
	*ast = (Ast){0};
}

void flush_ast(Ast *ast) {
	for (u64 i = 0; i < ast->count; ++i) {
		if (ast->nodes[i].type == Ast_Type_Array_Ptr) {
			release_array(ast->nodes[i].as_Array_Ptr);
		}
		ast->nodes[i] = (Ast_Node){0};
	}
	ast->count = 0;
	ast->parent = 0;
}
