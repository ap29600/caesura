#include "lib/format/module.h"

#include "src/eval/eval.h"
#include "src/token/token.h"

i64 fmt_eval_node_va (Byte_Slice dest, va_list va, Fmt_Info info);
i64 fmt_eval_node (Byte_Slice dest, Eval_Node src, Fmt_Info info);

i64 fmt_eval_context_as_dot(Byte_Slice dest, Eval_Context *src, Fmt_Info info);
i64 fmt_eval_context_as_dot_va(Byte_Slice dest, va_list va, Fmt_Info info);

i64 fmt_token(Byte_Slice dest, Token tok, Fmt_Info info);
i64 fmt_token_va(Byte_Slice dest, va_list va, Fmt_Info info);

void init_formats(void);
