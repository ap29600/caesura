#include "builtin.h"
#include "error.h"

cstring runtime_error_strings[Num_Errors] = {
    [None]              = "None",
    [Empty_Parse]       = "Empty_Parse",
    [Invalid_Exponent]  = "Invalid_Exponent",
    [Invalid_Parse]     = "Invalid_Parse",
};

