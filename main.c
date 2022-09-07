#include "lib/builtin.h"
#include "lib/error.h"
#include "lib/parsing.h"
#include "lib/bit_set.h"
#include "lib/format.h"
#include "lib/string.h"

#include <assert.h>
#include <stdarg.h>
#include <stdlib.h>

#include "token.h"
#include "eval.h"

#define MAX_TOKENS 1024

// i32 main(i32 argc, char **argv) {
//     register_format_directive((Fmt_Directive){"Token", fmt_token_va});
// 
//     Token tokens[MAX_TOKENS] = {0};
//     u64 num_tokens = 0;
// 
//     Parser_State parser = parser_from_filename(argv[1]);
// 
//     for (; !parser_is_empty(&parser); ) {
//         Token tok = next_token(&parser);
//         if (tok.type == Empty) continue;
//         tokens[num_tokens++] = tok;
//     }
// 
//     for (u64 i = 0; i < num_tokens; i++)
//         format_println("{Token}", tokens[i]);
// 
//     delete_str(&parser.source);
//     return 0;
// }



i64 fmt_eval_node (Byte_Slice dest, Eval_Node src, Fmt_Info info) {
    const char *begin = dest.begin;
    switch(src.type) {
        case Node_None:
            dest.begin += fmt_cstr(dest, "None", info);
            break;

        case Node_Identifier:
            assert(false && "unhandled identifier in eval node");
            break;

        case Node_Array:
            dest.begin += fmt_cstr(dest, "Array{", info);
            for (u64 i = 0; i < src.as.array->shape; ++i) {
                if (i > 0) {
                    dest.begin += fmt_rune(dest, ',', info);
                }
                dest.begin += fmt_i64(dest, (i64)src.as.array->data[i], info);
            }
            dest.begin += fmt_rune(dest, '}', info);
            break;

        case Node_Monad:
            dest.begin += fmt_cstr(dest, "Monad{", info);
            dest.begin += fmt_u64(dest, (u64)src.as.args.callee, info);
            dest.begin += fmt_rune(dest, ',', info);
            dest.begin += fmt_u64(dest, (u64)src.as.args.right, info);
            dest.begin += fmt_rune(dest, '}', info);
            break;

        case Node_Dyad:
            dest.begin += fmt_cstr(dest, "Dyad{", info);
            dest.begin += fmt_u64(dest, (u64)src.as.args.left, info);
            dest.begin += fmt_rune(dest, ',', info);
            dest.begin += fmt_u64(dest, (u64)src.as.args.callee, info);
            dest.begin += fmt_rune(dest, ',', info);
            dest.begin += fmt_u64(dest, (u64)src.as.args.right, info);
            dest.begin += fmt_rune(dest, '}', info);
            break;

        case Node_Function:
            dest.begin += fmt_cstr(dest, "Func@0x", info);
            dest.begin += fmt_ptr(dest, (uintptr_t)src.as.function, info);
            break;
    }
    return dest.begin - begin;
}

i64 fmt_eval_node_va (Byte_Slice dest, va_list va, Fmt_Info info) {
    Eval_Node node = va_arg(va, Eval_Node);
    return fmt_eval_node(dest, node, info);
}

i32 main () {
    Ast_Node expressions[] = {
        [0] = {Node_Array, .as.array = new_array(3, (double[]){1, 2, 3}) },
        [1] = {Node_Array, .as.array = new_array(3, (double[]){4, 5, 6}) },
        [2] = {Node_Identifier, .as.identifier =  "->" },
        [3] = {Node_Identifier, .as.identifier =  "<-" },
        [4] = {Node_Identifier, .as.identifier =  "-"  },
        [5] = {Node_Dyad, .as.args = {.left = 2, .callee = 4, .right = 3 }},
        [6] = {Node_Dyad, .as.args = {.left = 0, .callee = 5, .right = 1 }},
    };

    register_format_directive((Fmt_Directive){"expr", fmt_expression_va});
    register_format_directive((Fmt_Directive){"node", fmt_eval_node_va});

    set_format_user_ptr(expressions);
    format_println("Pretty-printed ast:\n\t{expr}", expressions[6]);

    Eval_Context ctx = {0};
    Node_Handle expr = apply(&ctx, expressions, 6, -1, -1);
    fflush(stdout);

    format_print("Raw ast:\n\t");
    for(int i = 0; i < ctx.count; ++i) {
        format_print("{node}, ", ctx.nodes[i]);
    }
    format_println("");

    expr = eval(&ctx, expr);

    format_println("result:\n\t{node}", ctx.nodes[expr]);

    for(int i = 0; i < ctx.count; ++i) {
        if (ctx.nodes[i].type == Node_Array) {
            free(ctx.nodes[i].as.array);
        }
    }

    free(ctx.nodes);
}
