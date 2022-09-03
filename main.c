#include "lib/builtin.h"
#include "lib/error.h"
#include "lib/parsing.h"
#include "lib/bit_set.h"
#include "lib/format.h"
#include "lib/string.h"

#include <assert.h>
#include <stdarg.h>

#include "token.h"

#define MAX_TOKENS 1024

i32 main(i32 argc, char **argv) {
    register_format_directive((Fmt_Directive){"Token", fmt_token_va});

    Token tokens[MAX_TOKENS] = {0};
    u64 num_tokens = 0;

    Parser_State parser = parser_from_filename(argv[1]);

    for (; !parser_is_empty(&parser); ) {
        Token tok = next_token(&parser);
        if (tok.type == Empty) continue;
        tokens[num_tokens++] = tok;
    }

    for (u64 i = 0; i < num_tokens; i++)
        format_println("{Token}", tokens[i]);

    delete_str(&parser.source);
    return 0;
}
