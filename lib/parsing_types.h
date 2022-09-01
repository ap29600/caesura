#ifndef FZR_PARSING_TYPES
#define FZR_PARSING_TYPES

#include "builtin.h"
#include "string_types.h"
#include "error.h"

typedef struct {
    u32 row;
    u32 col;
    u64 byte;
    cstring fname;
} Location;

typedef struct {
    String   source;
    Location location;
    Error    error;
} Parser_State;

#endif
