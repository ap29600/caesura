#ifndef FUNCS_H
#define FUNCS_H

#include "eval.h"

extern func_t     dyadic_functions[];
extern const char dyadic_function_names[][16];
extern u64        dyadic_function_count;

extern func_t     monadic_functions[];
extern const char monadic_function_names[][16];
extern u64        monadic_function_count;

#endif // FUNCS_H
