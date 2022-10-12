#include <stdlib.h>
#include "lib/bit_set.h"

#include "eval.h"
#include "ast.h"
#include "ir.h"

#include "src/runtime/runtime.h"

i32 entry_cmp (const void* a, const void*b) {
	const Lookup_Entry *aa = a;
	const Lookup_Entry *bb = b;
	return strncmp(aa->name.begin, bb->name.begin, sizeof(Short_String));
}

void scope_insert(Lookup_Scope *scope, Lookup_Entry entry) {
	if (scope->count + 1 >= scope->cap) {
		scope->cap = scope->cap ? scope->cap * 2 : 8;
		scope->entries = realloc(scope->entries, scope->cap * sizeof(Lookup_Entry));
	}

	u64 i = scope->count++;
	for (; i > 0 && entry_cmp(&entry, &scope->entries[i-1]) < 0; --i) {
		scope->entries[i] = scope->entries[i-1];
	}
	scope->entries[i] = entry;
}


Lookup_Entry *scope_lookup(Lookup_Scope *scope, Short_String name) {
	Lookup_Entry key = {.name = name};
	// TODO: make this lower_bound
	Lookup_Entry *result = bsearch(
			&key,
			scope->entries,
			scope->count,
			sizeof(Lookup_Entry), entry_cmp
	);

	if (result == NULL) { return NULL; }

	// iterate backwards to the first element that matches key
	for(u64 index = result - scope->entries; ; --index) {
		if (index == 0 || entry_cmp(&scope->entries[index - 1], &key) != 0) {
			return &scope->entries[index];
		}
	}
}

static Node_Handle append_node(Eval_Context *ctx, IR_Node node) {
	if (ctx->count >= ctx->cap) {
		ctx->cap = ctx->cap == 0 ? 8 : (ctx->cap * 2);
		ctx->nodes = realloc(ctx->nodes, ctx->cap * sizeof(IR_Node));
	}

	ctx->nodes[ctx->count] = node;
	return ctx->count++;
}

void release_node(IR_Node *node) {
	--node->ref_count;
	if (node->ref_count == 0) {
		if (node->type == IR_Type_Array) { release_array(node->as.array); }
		*node = (IR_Node){.type = IR_Type_None};
	}
}

Element_Type monadic_typecheck(Lookup_Entry *entry, Element_Type right) {
	if (entry->left != Type_None) { return Type_None; }
	bool right_ok = entry->right == Type_Generic_R || entry->right == right;
	if (!right_ok) { return Type_None; }
	switch (entry->result) {
		break;case Type_None:
			assert(false);
			unreachable();
		break;case Type_Generic_L:
			assert(false);
			unreachable();
		break;case Type_Generic_R: return right;
		break;default: return entry->result;
	}
}

Element_Type dyadic_typecheck(Lookup_Entry *entry, Element_Type left, Element_Type right) {
	bool left_ok  = entry->left  == Type_Generic_R || entry->left  == left;
	bool right_ok = entry->right == Type_Generic_R || entry->right == right;
	if (!right_ok || !left_ok) { return Type_None; }
	switch (entry->result) {
		break;case Type_None:
			assert(false);
			unreachable();
		break;case Type_Generic_L: return left;
		break;case Type_Generic_R: return right;
		break;default: return entry->result;
	}
}

Node_Handle apply_with(Eval_Context *ctx, const Ast_Node *base, Node_Handle expr, Node_Handle left, Node_Handle right) {
	// outer loop for TCO
	// TODO: use a stack for recursion
	assert(expr >= 0);
	while (true) {
		switch (base[expr].type) {
			break;case Ast_Type_None:
				assert(false);
				unreachable();

			MATCH_AST(Array_Ptr, array, base[expr]) {
				return append_node(ctx, (IR_Node){
					.type = IR_Type_Array,
					.eval_type = array->type,
					.as.array = borrow_array(array),
				});
			}

			MATCH_AST(Identifier, identifier, base[expr]) {
				Lookup_Entry key = { .name = identifier };
				Lookup_Entry *matches = scope_lookup(ctx->scope, key.name);

				if (!matches) {
					fmt_println("Error: `{cstr}` not found in environment", identifier.begin);
					fflush(stdout);
					assert(false && "lookup failed");
					unreachable();
				}

				u64 guard = ctx->scope->count - (matches - ctx->scope->entries);
				// try dyadic application
				if (left >= 0 && right >= 0) {
					Element_Type left_t  = ctx->nodes[left ].eval_type;
					Element_Type right_t = ctx->nodes[right].eval_type;
					for (u64 i = 0; i < guard && entry_cmp(&matches[i], &key) == 0; ++i) {
						Element_Type result;
						if ((result = dyadic_typecheck(&matches[i], left_t, right_t))) {
							Node_Handle func_node = append_node(ctx, (IR_Node) {
								.type = IR_Type_Function,
								.eval_type = Type_None,
								.as.function = matches[i].func,
							});

							++ctx->nodes[left     ].ref_count;
							++ctx->nodes[func_node].ref_count;
							++ctx->nodes[right    ].ref_count;
							return append_node(ctx, (IR_Node) {
								.type = IR_Type_Dyad,
								.eval_type = result,
								.as.args = {
									.left   = left,
									.callee = func_node,
									.right  = right,
								},
							});
						}
					}
				}

				if (right >= 0) {
					Element_Type right_t = ctx->nodes[right].eval_type;
					for (u64 i = 0; i < guard && entry_cmp(&matches[i], &key) == 0; ++i) {
						Element_Type result;
						if ((result = monadic_typecheck(&matches[i], right_t))) {
							Node_Handle func_node = append_node( ctx, (IR_Node) {
								.type = IR_Type_Function,
								.eval_type = Type_None,
								.as.function = matches[i].func,
							});

							++ctx->nodes[func_node].ref_count;
							++ctx->nodes[right    ].ref_count;
							return append_node(ctx, (IR_Node) {
								.type = IR_Type_Monad,
								.eval_type = result,
								.as.args = {
									.callee = func_node,
									.right  = right,
								},
							});
						}
					}
				}

				fmt_fprintln(stderr, "Error: `{cstr}` matches {u64} rules, but none type check",
					identifier.begin,
					guard
				);
				fmt_fprintln(stderr, "\ttypes are: {type}, {type}",
					left >= 0 ? ctx->nodes[left].eval_type : Type_None,
					right >= 0 ? ctx->nodes[right].eval_type : Type_None
				);
				fmt_fprintln(stderr, "candidates are:");
				for(u64 i = 0; i < guard && entry_cmp(&matches[i], &key) == 0; ++i) {
					fmt_fprintln(stderr, "\t{u64}: {cstr} : {type} -> {type} -> {type}",
						i,
						matches[i].name.begin,
						matches[i].left,
						matches[i].right,
						matches[i].result
					);
				}
				assert(false && "not implemented");
				unreachable();
			}

			MATCH_AST(Monadic, monad, base[expr]) {
				right = apply_with(ctx, base, monad.right, left, right);
				left = -1;
				expr = monad.func;
				continue;
			}

			MATCH_AST(Dyadic, dyad, base[expr]) {
				Node_Handle new_left  = apply_with(ctx, base, dyad.left,  left, right);
				Node_Handle new_right = apply_with(ctx, base, dyad.right, left, right);
				left  = new_left;
				right = new_right;
				expr  = dyad.func;
				continue;
			}

			break;case Ast_Type_Assignment: {

				assert(false && "unimplemented");
				unreachable();
				// Node_Handle bind_name  = base[expr].as.args.left;
				// Node_Handle bind_value = base[expr].as.args.right;
				// format_println("register `{cstr}`", base[bind_name].as.identifier);

				// Eval_Context new_ctx = {.scope = ctx->scope};

				// Node_Handle formal_left_arg = append_node(&new_ctx, (IR_Node){.type = Ast_Type_None});
				// Node_Handle formal_right_arg = append_node(&new_ctx, (IR_Node){.type = Ast_Type_None});

				// bind_value = apply_with(&new_ctx, base, bind_value, formal_left_arg, formal_right_arg);

				// Lookup_Entry entry = {0};
				// strncpy(entry.name.begin, base[bind_name].as.identifier, 15);

				// if ( new_ctx.nodes[formal_left_arg ].ref_count == 0
				//   && new_ctx.nodes[formal_right_arg].ref_count == 0 ) {

				// 	assert(bind_value == (Node_Handle)new_ctx.count - 1);
				// 	entry.value = flat_eval(&new_ctx);
				// 	assert(entry.value.type == Ast_Type_Array);
				// 	++entry.value.as.array->ref_count;
				// 	scope_insert(ctx->scope, entry);

				// 	free(new_ctx.nodes);
				// 	return append_node(ctx, entry.value);

				// } else {
				// 	assert(false);
				// 	entry.eval_tree.nodes = realloc(new_ctx.nodes, sizeof(IR_Node) * new_ctx.count);
				// 	entry.eval_tree.count = new_ctx.count;

				// 	assert(false && "assigning functions is not implemented yet");
				// }
			}
			break;default: assert(false);
		}
	}
}

void apply(Eval_Context *ctx, const Ast_Node *base, Node_Handle expr) {
	// no expression
	if (expr < 0) return;

	Node_Handle result = apply_with(ctx, base, expr, -1, -1);
	ctx->nodes[result].ref_count += 1;
	return;
}

IR_Node eval_once(Eval_Context *ctx, IR_Node *partial_results, Node_Handle expr) {
	switch(ctx->nodes[expr].type) {
		break;case IR_Type_None:
			return ctx->nodes[expr];

		break;case IR_Type_Array:
			ctx->nodes[expr].as.array->ref_count ++;
			return ctx->nodes[expr];

		break;case IR_Type_Function:
			return ctx->nodes[expr];

		break;case IR_Type_Monad: {
			// assume everything needed is already evaluated;
			IR_Node *func  = &partial_results[ctx->nodes[expr].as.args.callee];
			IR_Node *right = &partial_results[ctx->nodes[expr].as.args.right ];
			assert(func->type == IR_Type_Function);
			IR_Node result = func->as.function(NULL, right);
			result.ref_count = ctx->nodes[expr].ref_count;
			release_node(func);
			release_node(right);
			return result;
		}

		break;case IR_Type_Dyad: {
			// assume everything needed is already evaluated;
			IR_Node *left  = &partial_results[ctx->nodes[expr].as.args.left  ];
			IR_Node *func  = &partial_results[ctx->nodes[expr].as.args.callee];
			IR_Node *right = &partial_results[ctx->nodes[expr].as.args.right ];
			assert(func->type == IR_Type_Function);
			IR_Node result = func->as.function(left, right);
			result.ref_count = ctx->nodes[expr].ref_count;
			release_node(left);
			release_node(func);
			release_node(right);
			return result;
		}

		break;case IR_Type_Assign: {
			assert(false && "assign unhandled in application");
			unreachable();
		}

		default:
			assert(false);
			unreachable();
	}
}

IR_Node flat_eval(Eval_Context *ctx) {
	if (ctx->count == 0) { return (IR_Node){0}; }

	IR_Node partial_results[ctx->count];

	for(Node_Handle idx = 0; idx < (Node_Handle) ctx->count; ++idx) {
		partial_results[idx] = eval_once(ctx, partial_results, idx);
	}

	// at the end of evaluation, partial results have been cleaned up
	for(Node_Handle idx = 0; idx < (Node_Handle) ctx->count - 1; ++idx) {
		assert(partial_results[idx].type == IR_Type_None);
	}

	return partial_results[ctx->count - 1];
}
