#include <stdlib.h>
#include "ir.h"

void delete_eval_context(Eval_Context *ctx) {
	for(u64 i = 0; i < ctx->count; ++i) {
		 // dirty hack
		ctx->nodes[i].ref_count = 1;
		release_node(&ctx->nodes[i]);
	}
	free(ctx->nodes);
	*ctx = (Eval_Context){0};
}

void flush_eval_context(Eval_Context *ctx) {
	for(u64 i = 0; i < ctx->count; ++i) {
		 // dirty hack
		ctx->nodes[i].ref_count = 1;
		release_node(&ctx->nodes[i]);
	}
	ctx->count = 0;
}
