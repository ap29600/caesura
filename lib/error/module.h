#ifndef FZR_ERROR
#define FZR_ERROR

#include "../builtin.h"

typedef enum {
	None = 0,
	Empty_Parse,
	Invalid_Exponent,
	Invalid_Parse,
	Num_Errors,
} Error;

extern const char *runtime_error_strings[Num_Errors];

#endif
