#include "funcs.h"

#include <stdlib.h>
#include <math.h>


static Eval_Node func_right_identity(Eval_Node *left, Eval_Node *right) {
    assert(right->type == Node_Array);
    return (Eval_Node) {.type = Node_Array, .as.array = borrow_array(right->as.array)};
}

static Eval_Node func_left_identity(Eval_Node *left, Eval_Node *right) {
    assert(left->type == Node_Array);
    return (Eval_Node) {.type = Node_Array, .as.array = borrow_array(left->as.array)};
}

void init_default_scope() {
     scope_insert(&default_scope, (Lookup_Entry){ .name = "->", .as_monadic = func_right_identity, .as_dyadic = NULL               });
     scope_insert(&default_scope, (Lookup_Entry){ .name = "<-", .as_monadic = NULL,                .as_dyadic = func_left_identity });
     scope_insert(&default_scope, (Lookup_Entry){ .name = "+",  .as_monadic = func_right_identity, .as_dyadic = func_plus          });
     scope_insert(&default_scope, (Lookup_Entry){ .name = "-",  .as_monadic = func_negate,         .as_dyadic = func_minus         });
     scope_insert(&default_scope, (Lookup_Entry){ .name = "*",  .as_monadic = NULL,                .as_dyadic = func_multiply      });
     scope_insert(&default_scope, (Lookup_Entry){ .name = "/",  .as_monadic = func_square_root,    .as_dyadic = func_divide        });
     scope_insert(&default_scope, (Lookup_Entry){ .name = "~",  .as_monadic = func_complement,     .as_dyadic = func_mismatch      });
     scope_insert(&default_scope, (Lookup_Entry){ .name = "=",  .as_monadic = NULL,                .as_dyadic = func_equal         });
     scope_insert(&default_scope, (Lookup_Entry){ .name = "?",  .as_monadic = NULL,                .as_dyadic = func_filter        });
     scope_insert(&default_scope, (Lookup_Entry){ .name = "$",  .as_monadic = func_shape,          .as_dyadic = func_reshape        });
}

Lookup_Scope default_scope = {0};
