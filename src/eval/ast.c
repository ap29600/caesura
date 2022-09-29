#include <stdlib.h>

#include "ast.h"

void flush_ast(Ast *ast) {
	for (u64 i = 0; i < ast->nodes.count; ++i) {
		Ast_Node *const node = &vec_array(&ast->nodes, Ast_Node)[i];
		if (node->type == Ast_Type_Array_Ptr) {
			release_array(node->as_Array_Ptr);
		}
	}
	ast->nodes.count = 0;
	ast->parent = 0;
}

void delete_ast(Ast *ast) {
	flush_ast(ast);
	vec_delete(&ast->nodes);
	*ast = (Ast){0};
}

