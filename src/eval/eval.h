#ifndef EVAL_H
#define EVAL_H

#include "lib/builtin.h"
#include "lib/format/format.h"

#include "src/array/array.h"

typedef enum {
    Dyadic,
    Monadic,
} Function_Attribute;

typedef struct {
    char name[16];
    u64 flags;
} Function_t;

typedef i32 Node_Handle;

typedef enum {
    Node_None = 0,
    Node_Array,
    Node_Monad,
    Node_Dyad,
    Node_Assign,
    Node_Function,
    Node_Identifier,
} Node_Type;

typedef struct Ast_Node {
    Node_Type type;
    union {
        Array *array;
        char   identifier[16];
        struct {
            Node_Handle left;
            Node_Handle right;
            Node_Handle callee;
        } args;
    } as;
} Ast_Node;

struct Eval_Node;
struct Eval_Context;

typedef struct Eval_Node (*func_t)(
    struct Eval_Context *ctx,
    Node_Handle left,
    Node_Handle right);

typedef struct Eval_Node {
    Node_Type type;
    u64 ref_count;
    union {
        Array *array;
        func_t  function;
        char    identifier[16];
        struct {
            Node_Handle left;
            Node_Handle right;
            Node_Handle callee;
        } args;
    } as;
} Eval_Node;

i64     fmt_expression    (Byte_Slice dest, Ast_Node src, Fmt_Info info);
i64     fmt_expression_va (Byte_Slice dest, va_list va, Fmt_Info info);
Array new_array         (u64 shape, double *data);

typedef struct {
    char   name[16];
    func_t as_monadic;
    func_t as_dyadic;
    struct {Eval_Node *nodes; u64 count;} eval_tree;
    Eval_Node value;
} Lookup_Entry;

typedef struct {
    Lookup_Entry *entries;
    u64           count;
    u64           cap;
} Lookup_Scope;

i32  entry_cmp (const void* a, const void*b);
Lookup_Entry *scope_lookup(Lookup_Scope *scope, const char name[16]);
void scope_insert(Lookup_Scope *scope, Lookup_Entry entry);

typedef struct Eval_Context {
    Eval_Node    *nodes;
    u64           count;
    u64           cap;
    Lookup_Scope *scope;
} Eval_Context;

void release_node(Eval_Context *ctx, Node_Handle expr);

/// compiles into ctx as an expression tree the AST contained in base, starting from expression base[expr]
/// returns the index of the parent expression;
Node_Handle apply(Eval_Context *ctx, const Ast_Node *base, Node_Handle expr);

/// evaluates in place the expression tree contained in ctx, starting from expression expr.
/// returns the index of the result expression;
Node_Handle eval(Eval_Context *ctx, Node_Handle expr);

#endif // EVAL_H
