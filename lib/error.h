#ifndef FZR_ERROR
#define FZR_ERROR

typedef enum {
    None = 0,
    Trailing_Chars,
    Truncated_Literal,
    Unexpected_Char,
    Num_Errors,
} Error;

extern const char *runtime_error_strings[Num_Errors];

#endif
