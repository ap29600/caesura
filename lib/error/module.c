#include "../builtin.h"
#include "module.h"

cstring runtime_error_strings[Num_Errors] = {
	[None]              = "None",
	[Empty_Parse]       = "Empty_Parse",
	[Invalid_Exponent]  = "Invalid_Exponent",
	[Invalid_Parse]     = "Invalid_Parse",
};
