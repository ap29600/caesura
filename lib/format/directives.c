#include <stdlib.h>

#include "module.h"

#include "procedures.h"

typedef struct {
	Short_String name;
	fmt_procedure_t fmt;
} Fmt_Directive;

#define MAX_DIRECTIVES 256
static Fmt_Directive directives[MAX_DIRECTIVES] = {
	{ { "ptr"   }, fmt_ptr_va      },
	{ { "i64"   }, fmt_i64_va      },
	{ { "u64"   }, fmt_u64_va      },
	{ { "f64"   }, fmt_f64_va      },
	{ { "f32"   }, fmt_f64_va      }, // auto-promotion makes it a f64
	{ { "i32"   }, fmt_i32_va      },
	{ { "u32"   }, fmt_u32_va      },
	{ { "str"   }, fmt_str_va      },
	{ { "cstr"  }, fmt_cstr_va     },
	{ { "rune"  }, fmt_rune_va     },
	{ { "error" }, fmt_error_va    },
	{ { "loc"   }, fmt_location_va },
	{ { "col"   }, fmt_color_va    },
};

static u64 directives_count = 0;

static i32 directive_cmp(const void *a, const void *b) {
	return strncmp(
		((Fmt_Directive*)a)->name.begin,
		((Fmt_Directive*)b)->name.begin,
		sizeof(Short_String));
}

static void init_directives(){
	// count the nonempty directives.
	for(u64 i = 0; i < MAX_DIRECTIVES; ++i) {
		if (directives[i].name.begin[0] == '\0') {
			directives_count = i;
			break;
		}
	}

	qsort(directives, directives_count, sizeof(Fmt_Directive), directive_cmp);
}

bool register_format_directive(Short_String name, fmt_procedure_t format) {
	if (directives_count == 0) { init_directives(); }
	if (directives_count >= MAX_DIRECTIVES) { return false; }

	Fmt_Directive directive = { .name = name, .fmt = format };

	u64 pos = directives_count++;
	for (;pos > 0; --pos) {
		if (directive_cmp(&directive, &directives[pos - 1]) >= 0) { break; }
		directives[pos] = directives[pos - 1];
	}
	// no duplicates
	assert(directive_cmp(&directive, &directives[pos - 1]) != 0);

	directives[pos] = directive;
	return true;
}

fmt_procedure_t lookup_format_directive(Short_String name) {
	if (directives_count == 0) { init_directives(); }

	Fmt_Directive key = { .name = name };
	Fmt_Directive *dir = bsearch(&key,
								 directives,
								 directives_count,
								 sizeof(Fmt_Directive),
								 directive_cmp);

	return dir ? dir->fmt : NULL;
}
