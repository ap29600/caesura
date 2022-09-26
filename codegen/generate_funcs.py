#!/bin/env python3

types = { \
        'f64':  { 'range' : 2048, 'name': 'Type_Float' },
        'i64':  { 'range' : 64,   'name': 'Type_Int'   },
        'char': { 'range' : 8,    'name': 'Type_Char'  },
        'bool': { 'range' : 1,    'name': 'Type_Bool'  },
        }

def supertype(l, r):
    return min([ name \
            for name, info in types.items() \
            if info['range'] >= max(types[l]['range'], types[r]['range']) \
            ],
            key=lambda name: types[name]['range'])



dyadic_funcs = { \
        'add' : {
            'symbol': '+',
            'op': lambda lt, rt: lambda l, r :  f"{l} + {r}",
            'result' : lambda l, r: 'i64' if supertype(l, r) == 'bool' else supertype(l, r),
            'condition': lambda l, r : True
            },
        'sub' : {
            'symbol': '-',
            'op': lambda lt, rt: lambda l, r :  f"{l} - {r}",
            'result' : lambda l, r: 'i64' if supertype(l, r) == 'bool' else supertype(l, r),
            'condition': lambda l, r : True
            },
        'mul' : {
            'symbol': '*',
            'op': lambda lt, rt: lambda l, r :  f"{l} & {r}" if (lt == 'bool' and rt == 'bool') else f"{l} * {r}",
            'result' : lambda l, r: supertype(l, r),
            'condition': lambda l, r: True
            },
        'div' : {
            'symbol': '/',
            'op': lambda lt, rt: lambda l, r :  f"{l} / {r}",
            'result' : lambda l, r: supertype(l, r),
            'condition': lambda l, r : r != 'bool'
            },
        'eq'  : {
            'symbol': '=',
            'op': lambda lt, rt: lambda l, r :  f"{l} == {r}",
            'result' : lambda l, r: 'bool',
            'condition': lambda l, r : True
            },
        }


def generate_name(name, info, left_t, right_t):
    return f"func_dyad_{name}_{left_t}_{right_t}"


def generate_implementation(name, info, left_t, right_t):
    result_type = info['result'](left_t, right_t)
    op = info['op'](left_t, right_t)
    left_expr  = f"(({left_t}*)left->data)[i]"
    right_expr = f"(({right_t}*)right->data)[i]"
    print(f"IR_Node func_dyad_{name}_{left_t}_{right_t} (IR_Node *left_node, IR_Node *right_node) {{")
    print( "\tassert(left_node->type == IR_Type_Array);")
    print( "\tassert(right_node->type == IR_Type_Array);")
    print( "\tArray *left  = borrow_array(left_node->as.array);")
    print( "\tArray *right = borrow_array(right_node->as.array);")
    print( "")
    print( "\tassert(left->shape == right->shape);")
    print( "\tu64 shape = left->shape;")
    print(f"\tArray *result = make_array(NULL, shape, {types[result_type]['name']});")
    print( "")
    print( "\tfor (u64 i = 0; i < shape; ++i) {")
    print(f"\t\t(({result_type}*)result->data)[i] = {op(left_expr, right_expr)};")
    print( "\t}")
    print( "")
    print( "\trelease_array(left);")
    print( "\trelease_array(right);")
    print( "\treturn (IR_Node) {.type = IR_Type_Array, .as.array = result};")
    print( "}")
    print( "")

def generate_func_binding(name, info, left_t, right_t):
    func_name  = generate_name(name, info, left_t, right_t)
    left_enum  = types[left_t]['name']
    right_enum = types[right_t]['name']
    result_enum = types[info['result'](left_t, right_t)]['name']
    return  "(Lookup_Entry){ "                   + \
           f".name = {{\"{info['symbol']}\"}}, " + \
           f".func = {func_name}, "              + \
           f".left = {left_enum}, "              + \
           f".right = {right_enum}, "            + \
           f".result = {result_enum}, "          + \
           "}"

if __name__ == '__main__':
    bindings = []
    print('#include "runtime.h"')
    print('')
    for name, info in dyadic_funcs.items():
        for left_t in types.keys():
            for right_t in types.keys():
                if (info['condition'](left_t, right_t)):
                    generate_implementation(name, info, left_t, right_t)
                    bindings += [generate_func_binding(name, info, left_t, right_t)]
                else:
                    print(f"// {info['op'](left_t, right_t)(left_t, right_t)} ==> deleted\n")
    print("void bind_generated_funcs (void) {")
    for bind in bindings:
        print(f"\tscope_insert(&default_scope, {bind});")
    print("}")

