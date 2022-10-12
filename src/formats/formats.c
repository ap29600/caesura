#ifndef FORMAT_H
#define FORMAT_H

#include "formats.h"

#define CONSOLE_COLORS
#ifdef CONSOLE_COLORS
	#define RESET "\x1b[0m"
	#define COL_RED_F "\x1b[31m"
	#define COL_GRN_F "\x1b[32m"
	#define COL_BLU_F "\x1b[34m"
#else
	#define RESET ""
	#define COL_RED_F ""
	#define COL_GRN_F ""
	#define COL_BLU_F ""
#endif

#include "src/eval/ast.h"
#include "src/eval/ir.h"

i64 fmt_eval_context_as_dot_va(Byte_Slice dest, va_list va, Fmt_Info info) {
	Eval_Context *src = va_arg(va, Eval_Context*);
	return fmt_eval_context_as_dot(dest, src, info);
}

i64 fmt_eval_node (Byte_Slice dest, IR_Node src, Fmt_Info info) {
	const char *begin = dest.begin;
	switch(src.type) {
		break;case IR_Type_None:
			dest.begin += fmt_cstr(dest, "None", info);

		break;case IR_Type_Array:
			switch(src.as.array->type) {
				break;case Type_None: assert(false);
				break;case Type_Generic_L: assert(false);
				break;case Type_Generic_R: assert(false);
				break;case Type_Float: {
					dest.begin += fmt_cstr(dest, "[f64]", info);
					f64 *data = src.as.array->data;
					for (u64 i = 0; i < src.as.array->shape; ++i) {
						if (i > 0) { dest.begin += fmt_rune(dest, ',', info); }
						dest.begin += fmt_f64(dest, data[i], info);
					}
				}

				break;case Type_Int: {
					dest.begin += fmt_cstr(dest, "[i64]", info);
					i64 *data = src.as.array->data;
					for (u64 i = 0; i < src.as.array->shape; ++i) {
						if (i > 0) { dest.begin += fmt_rune(dest, ',', info); }
						dest.begin += fmt_i64(dest, data[i], info);
					}
				}

				break;case Type_Char: {
					dest.begin += fmt_cstr(dest, "[char]", info);
					String data = {.begin = src.as.array->data, .end = (char*)src.as.array->data + src.as.array->shape};
					dest.begin += fmt_rune(dest, '"', info);
					dest.begin += fmt_str(dest, data, info);
					dest.begin += fmt_rune(dest, '"', info);
				}

				break;case Type_Bool: {
					dest.begin += fmt_cstr(dest, "[bool]", info);
					bool *data = src.as.array->data;
					for (u64 i = 0; i < src.as.array->shape; ++i) {
						if (i > 0) { dest.begin += fmt_rune(dest, ',', info); }
						dest.begin += fmt_rune(dest, "01"[data[i]], info);
					}
				}

				break;case Types_Count: assert(false);
			}

		break;case IR_Type_Monad:
			dest.begin += fmt_cstr(dest, "Monad", info);

		break;case IR_Type_Dyad:
			dest.begin += fmt_cstr(dest, "Dyad", info);

		break;case IR_Type_Function:
			dest.begin += fmt_cstr(dest, "Func@0x", info);
			dest.begin += fmt_ptr(dest, (uintptr_t)src.as.function, info);

		break;default:
			assert(false);
	}
	return dest.begin - begin;
}

i64 fmt_eval_node_va (Byte_Slice dest, va_list va, Fmt_Info info) {
	IR_Node node = va_arg(va, IR_Node);
	return fmt_eval_node(dest, node, info);
}

i64 fmt_eval_context_as_dot(Byte_Slice dest, Eval_Context *src, Fmt_Info info) {
	const char *begin = dest.begin;
	dest.begin += fmt_cstr(dest, "digraph {\n", info);

	for(u64 i = 0; i < src->count; ++i) {
		dest.begin += string_len(fmt_to(dest, "\tnode_{u64} [label=\"{node}\"]\n", i, src->nodes[i]));
	}

	for(u64 i = 0; i < src->count; ++i) {
		switch(src->nodes[i].type) {
			break;case IR_Type_Monad:
				dest.begin += string_len(fmt_to(dest, "\tnode_{u64} -> node_{u64} [label=func]\n", i, src->nodes[i].as.args.callee));
				dest.begin += string_len(fmt_to(dest, "\tnode_{u64} -> node_{u64} [label=right]\n", i, src->nodes[i].as.args.right));

			break;case IR_Type_Dyad:
				dest.begin += string_len(fmt_to(dest, "\tnode_{u64} -> node_{u64} [label=left]\n", i, src->nodes[i].as.args.left));
				dest.begin += string_len(fmt_to(dest, "\tnode_{u64} -> node_{u64} [label=func]\n", i, src->nodes[i].as.args.callee));
				dest.begin += string_len(fmt_to(dest, "\tnode_{u64} -> node_{u64} [label=right]\n", i, src->nodes[i].as.args.right));

			break;default:;
		}
	}

	dest.begin += fmt_rune(dest, '}', info);
	return dest.begin - begin;
}


i64 fmt_token(Byte_Slice dest, Token tok, Fmt_Info info) {
	char *const begin = dest.begin;

	dest.begin += fmt_location(dest, tok.loc, info);
	dest.begin += fmt_cstr(dest, ": ", info);
	dest.begin += fmt_cstr(dest, "Token{ ", info);
	if (!tok.is_valid) {
		dest.begin += fmt_cstr(dest, "[" COL_RED_F "INVALID" RESET "] ", info);
	}

	switch (tok.type) {
		break;case Token_Type_Int:
			dest.begin += fmt_cstr(dest, "i64: " COL_GRN_F, info);
			dest.begin += fmt_i64(dest, tok.i64value, info);
			dest.begin += fmt_cstr(dest, RESET " ", info);

		break;case Token_Type_Float:
			dest.begin += fmt_cstr(dest, "f64: " COL_GRN_F, info);
			dest.begin += fmt_f64(dest, tok.f64value, info);
			dest.begin += fmt_cstr(dest, RESET " ", info);

		break;case Token_Type_Operator:
			dest.begin += fmt_cstr(dest, "op: " COL_BLU_F, info);
			dest.begin += fmt_cstr(dest, operator_strings[tok.op], info);
			dest.begin += fmt_cstr(dest, RESET " ", info);

		break;case Token_Type_Identifier:
			dest.begin += fmt_cstr(dest, "identifier: " COL_BLU_F "`", info);
			dest.begin += fmt_str(dest, tok.text, info);
			dest.begin += fmt_cstr(dest, "`" RESET " ", info);

		break;case Token_Type_Empty:
			dest.begin += fmt_cstr(dest, "nil ", info);
	}

	dest.begin += fmt_cstr(dest, "}", info);
	return dest.begin - begin;
}

i64 fmt_token_va(Byte_Slice dest, va_list va, Fmt_Info info) {
	Token tok = va_arg(va, Token);
	return fmt_token(dest, tok, info);
}

i64 fmt_expression(Byte_Slice dest, Ast_Node src, Fmt_Info info) {
	const Ast_Node *ctx = info.user_ptr;
	const char *begin = dest.begin;
	switch (src.type) {
		break;case Ast_Type_None:
			dest.begin += fmt_cstr(dest, "<No_Expression>", info);

		MATCH_AST(Monadic, monad, src) {
			dest.begin += fmt_rune(dest, '(', info);
			dest.begin += fmt_expression(dest, ctx[monad.func], info);
			dest.begin += fmt_rune(dest, ' ', info);
			dest.begin += fmt_expression(dest, ctx[monad.right], info);
			dest.begin += fmt_rune(dest, ')', info);
		}

		MATCH_AST(Dyadic, dyad, src) {
			dest.begin += fmt_rune(dest, '(', info);
			dest.begin += fmt_expression(dest, ctx[dyad.left], info);
			dest.begin += fmt_rune(dest, ' ', info);
			dest.begin += fmt_expression(dest, ctx[dyad.func], info);
			dest.begin += fmt_rune(dest, ' ', info);
			dest.begin += fmt_expression(dest, ctx[dyad.right], info);
			dest.begin += fmt_rune(dest, ')', info);
		}

		MATCH_AST(Identifier, id, src) {
			dest.begin += fmt_cstr(dest, id.begin, info);
		}

		MATCH_AST(Array_Ptr, arr, src) {
			switch(arr->type) {
				break;case Type_None: assert(false);
				break;case Type_Generic_R: assert(false);
				break;case Type_Generic_L: assert(false);
				break;case Type_Float: {
					f64 *data = arr->data;
					for (u64 i = 0; i < arr->shape; ++i) {
						if (i > 0) { dest.begin += fmt_rune(dest, ',', info); }
						dest.begin += fmt_f64(dest, data[i], info);
					}
				}

				break;case Type_Int: {
					i64 *data = arr->data;
					for (u64 i = 0; i < arr->shape; ++i) {
						if (i > 0) { dest.begin += fmt_rune(dest, ',', info); }
						dest.begin += fmt_i64(dest, data[i], info);
					}
				}

				break;case Type_Char: {
					String data = {.begin = arr->data, .end = (char*)arr->data + arr->shape};
					dest.begin += fmt_rune(dest, '"', info);
					dest.begin += fmt_str(dest, data, info);
					dest.begin += fmt_rune(dest, '"', info);
				}

				break;case Type_Bool: {
					bool *data = arr->data;
					for (u64 i = 0; i < arr->shape; ++i) {
						if (i > 0) { dest.begin += fmt_rune(dest, ',', info); }
						dest.begin += fmt_rune(dest, "01"[data[i]], info);
					}
				}

				break;case Types_Count: assert(false);
			}
		}

		MATCH_AST(Assignment, assign, src) {
			dest.begin += fmt_rune(dest, '(', info);
			dest.begin += fmt_expression(dest, ctx[assign.left], info);
			dest.begin += fmt_cstr(dest, ":", info);
			dest.begin += fmt_expression(dest, ctx[assign.right], info);
			dest.begin += fmt_rune(dest, ')', info);
		}
	}

	return dest.begin - begin;
}

i64 fmt_expression_va(Byte_Slice dest, va_list va, Fmt_Info info) {
	Ast_Node src = va_arg(va, Ast_Node);
	return fmt_expression(dest, src, info);
}

i64 fmt_type (Byte_Slice dest, Element_Type src, Fmt_Info info) {
	return fmt_cstr(dest, runtime_type_strings[src],info);
}

i64 fmt_type_va (Byte_Slice dest, va_list va, Fmt_Info info) {
	Element_Type src = va_arg(va, Element_Type);
	return fmt_type(dest, src, info);
}

void init_formats(void) {
	register_format_directive((Short_String){"expr"},    fmt_expression_va);
	register_format_directive((Short_String){"node"},    fmt_eval_node_va);
	register_format_directive((Short_String){"ctx_dot"}, fmt_eval_context_as_dot_va);
	register_format_directive((Short_String){"token"},   fmt_token_va);
	register_format_directive((Short_String){"type"},    fmt_type_va);
}

#endif // FORMAT_H
