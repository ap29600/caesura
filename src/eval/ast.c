#include <stdlib.h>

#include "ast.h"

void flush_ast(Ast *ast) {
	for (u64 i = 0; i < Vec_size(ast->nodes); ++i) {
		Ast_Node *const node = &ast->nodes[i];
		if (node->type == Ast_Type_Array_Ptr) {
			release_array(node->as_Array_Ptr);
		}
	}
	Vec_flush(&ast->nodes);
	ast->parent = 0;
}

void delete_ast(Ast *ast) {
	flush_ast(ast);
	Vec_delete(&ast->nodes);
	*ast = (Ast){0};
}

