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


i64 fmt_eval_context_as_dot_va(Byte_Slice dest, va_list va, Fmt_Info info) {
    Eval_Context *src = va_arg(va, Eval_Context*);
    return fmt_eval_context_as_dot(dest, src, info);
}

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
            switch(src.as.array->type) {
                case Type_Float: {
                    f64 *data = src.as.array->data;
                    for (u64 i = 0; i < src.as.array->shape; ++i) {
                        if (i > 0) dest.begin += fmt_rune(dest, ',', info);
                        dest.begin += fmt_f64(dest, data[i], info);
                    }
                } break;

                case Type_Int: {
                    i64 *data = src.as.array->data;
                    for (u64 i = 0; i < src.as.array->shape; ++i) {
                        if (i > 0) dest.begin += fmt_rune(dest, ',', info);
                        dest.begin += fmt_i64(dest, data[i], info);
                    }
                } break;

                case Type_UInt: {
                    u64 *data = src.as.array->data;
                    for (u64 i = 0; i < src.as.array->shape; ++i) {
                        if (i > 0) dest.begin += fmt_rune(dest, ',', info);
                        dest.begin += fmt_u64(dest, data[i], info);
                    }
                } break;

                case Type_Char: {
                    String data = {.begin = src.as.array->data, .end = (char*)src.as.array->data + src.as.array->shape};
                    dest.begin += fmt_rune(dest, '"', info);
                    dest.begin += fmt_str(dest, data, info);
                    dest.begin += fmt_rune(dest, '"', info);
                } break;

                case Type_Bool: {
                    bool *data = src.as.array->data;
                    for (u64 i = 0; i < src.as.array->shape; ++i) {
                        if (i > 0) dest.begin += fmt_rune(dest, ',', info);
                        dest.begin += fmt_rune(dest, "01"[data[i]], info);
                    }
                } break;
            }
            break;

        case Node_Monad:
            dest.begin += fmt_cstr(dest, "Monad", info);
            break;

        case Node_Dyad:
            dest.begin += fmt_cstr(dest, "Dyad", info);
            break;

        case Node_Function:
            dest.begin += fmt_cstr(dest, "Func@0x", info);
            dest.begin += fmt_ptr(dest, (uintptr_t)src.as.function, info);
            break;
        default:
            assert(false);
    }
    return dest.begin - begin;
}

i64 fmt_eval_node_va (Byte_Slice dest, va_list va, Fmt_Info info) {
    Eval_Node node = va_arg(va, Eval_Node);
    return fmt_eval_node(dest, node, info);
}

i64 fmt_eval_context_as_dot(Byte_Slice dest, Eval_Context *src, Fmt_Info info) {
    const char *begin = dest.begin;
    dest.begin += fmt_cstr(dest, "digraph {\n", info);

    for(u64 i = 0; i < src->count; i++)
        dest.begin += string_len(format_to(dest, "\tnode_{u64} [label=\"{node}\"]\n", i, src->nodes[i]));

    for(u64 i = 0; i < src->count; i++) {
        switch(src->nodes[i].type) {
            case Node_Monad:
                dest.begin += string_len(format_to(dest, "\tnode_{u64} -> node_{u64} [label=func]\n", i, src->nodes[i].as.args.callee));
                dest.begin += string_len(format_to(dest, "\tnode_{u64} -> node_{u64} [label=right]\n", i, src->nodes[i].as.args.right));
                break;
            case Node_Dyad:
                dest.begin += string_len(format_to(dest, "\tnode_{u64} -> node_{u64} [label=left]\n", i, src->nodes[i].as.args.left));
                dest.begin += string_len(format_to(dest, "\tnode_{u64} -> node_{u64} [label=func]\n", i, src->nodes[i].as.args.callee));
                dest.begin += string_len(format_to(dest, "\tnode_{u64} -> node_{u64} [label=right]\n", i, src->nodes[i].as.args.right));
                break;
            default:
                break;
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
        case Float:
            dest.begin += fmt_cstr(dest, "f64: " COL_GRN_F, info);
            dest.begin += fmt_f64(dest, tok.value, info);
            dest.begin += fmt_cstr(dest, RESET " ", info);
            break;

        case Operator:
            dest.begin += fmt_cstr(dest, "op: " COL_BLU_F, info);
            dest.begin += fmt_cstr(dest, operator_strings[tok.op], info);
            dest.begin += fmt_cstr(dest, RESET " ", info);
            break;

        case Identifier:
            dest.begin += fmt_cstr(dest, "identifier: " COL_BLU_F "`", info);
            dest.begin += fmt_str(dest, tok.text, info);
            dest.begin += fmt_cstr(dest, "`" RESET " ", info);
            break;

        case Empty:
            dest.begin += fmt_cstr(dest, "nil ", info);
            break;
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
        case Node_None:
            dest.begin += fmt_cstr(dest, "<No_Expression>", info);
            break;

        case Node_Monad:
            dest.begin += fmt_rune(dest, '(', info);
            dest.begin += fmt_expression(dest, ctx[src.as.args.callee], info);
            dest.begin += fmt_rune(dest, ' ', info);
            dest.begin += fmt_expression(dest, ctx[src.as.args.right], info);
            dest.begin += fmt_rune(dest, ')', info);
            break;

        case Node_Dyad:
            dest.begin += fmt_rune(dest, '(', info);
            dest.begin += fmt_expression(dest, ctx[src.as.args.left], info);
            dest.begin += fmt_rune(dest, ' ', info);
            dest.begin += fmt_expression(dest, ctx[src.as.args.callee], info);
            dest.begin += fmt_rune(dest, ' ', info);
            dest.begin += fmt_expression(dest, ctx[src.as.args.right], info);
            dest.begin += fmt_rune(dest, ')', info);
            break;

        case Node_Function:
            assert(false);

        case Node_Identifier:
            dest.begin += fmt_cstr(dest, src.as.identifier, info);
            break;

        case Node_Array:
            switch(src.as.array->type) {
                case Type_Float: {
                    f64 *data = src.as.array->data;
                    for (u64 i = 0; i < src.as.array->shape; ++i) {
                        if (i > 0) dest.begin += fmt_rune(dest, ',', info);
                        dest.begin += fmt_f64(dest, data[i], info);
                    }
                } break;

                case Type_Int: {
                    i64 *data = src.as.array->data;
                    for (u64 i = 0; i < src.as.array->shape; ++i) {
                        if (i > 0) dest.begin += fmt_rune(dest, ',', info);
                        dest.begin += fmt_i64(dest, data[i], info);
                    }
                } break;

                case Type_UInt: {
                    u64 *data = src.as.array->data;
                    for (u64 i = 0; i < src.as.array->shape; ++i) {
                        if (i > 0) dest.begin += fmt_rune(dest, ',', info);
                        dest.begin += fmt_u64(dest, data[i], info);
                    }
                } break;

                case Type_Char: {
                    String data = {.begin = src.as.array->data, .end = (char*)src.as.array->data + src.as.array->shape};
                    dest.begin += fmt_rune(dest, '"', info);
                    dest.begin += fmt_str(dest, data, info);
                    dest.begin += fmt_rune(dest, '"', info);
                } break;

                case Type_Bool: {
                    bool *data = src.as.array->data;
                    for (u64 i = 0; i < src.as.array->shape; ++i) {
                        if (i > 0) dest.begin += fmt_rune(dest, ',', info);
                        dest.begin += fmt_rune(dest, "01"[data[i]], info);
                    }
                } break;
            }
            break;
        case Node_Assign:
            dest.begin += fmt_rune(dest, '(', info);
            dest.begin += fmt_expression(dest, ctx[src.as.args.left], info);
            dest.begin += fmt_cstr(dest, ":", info);
            dest.begin += fmt_expression(dest, ctx[src.as.args.callee], info);
            dest.begin += fmt_rune(dest, ')', info);
    }

    return dest.begin - begin;
}

void init_formats(void) {
    register_format_directive((Fmt_Directive){"expr",    fmt_expression_va});
    register_format_directive((Fmt_Directive){"node",    fmt_eval_node_va});
    register_format_directive((Fmt_Directive){"ctx_dot", fmt_eval_context_as_dot_va});
    register_format_directive((Fmt_Directive){"token",   fmt_token_va});
}

#endif // FORMAT_H
