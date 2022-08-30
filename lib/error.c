#include "builtin.h"
#include "error.h"

cstring runtime_error_strings[Num_Errors] = {
    [None]              = "None",
    [Trailing_Chars]    = "Trailing_Chars",
    [Truncated_Literal] = "Truncated_Literal",
    [Unexpected_Char]   = "Unexpected_Char",
};

