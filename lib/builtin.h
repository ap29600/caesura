#ifndef FZR_BUILTIN
#define FZR_BUILTIN

#include <inttypes.h>
#include <stdbool.h>

/// 8-bit unsigned integer type
typedef uint8_t  u8;
/// 16-bit unsigned integer type
typedef uint16_t u16;
/// 32-bit unsigned integer type
typedef uint32_t u32;
/// 64-bit unsigned integer type
typedef uint64_t u64;

/// 8-bit signed integer type
typedef int8_t  i8;
/// 16-bit signed integer type
typedef int16_t i16;
/// 32-bit signed integer type
typedef int32_t i32;
/// 64-bit signed integer type
typedef int64_t i64;

/// signed integer type with the same bitwidth as a pointer
typedef intptr_t  isize;

/// unsigned integer type with the same bitwidth as a pointer
typedef uintptr_t usize;

/// 32-bit floating point type
typedef float  f32;
/// 64-bit floating point type
typedef double f64;

/// NULL-terminated string type
typedef const char *cstring;

/// 32-bit UTF character type
typedef u32 rune;

static inline i64 min(i64 a, i64 b) { return a <= b ? a : b; }
static inline i64 max(i64 a, i64 b) { return a >= b ? a : b; }

// Uses compiler specific extensions if possible.
#ifdef __GNUC__ // GCC, Clang, ICC
 
#define unreachable() (__builtin_unreachable())
 
#elifdef _MSC_VER // MSVC
 
#define unreachable() (__assume(false))
 
#else
// Even if no extension is used, undefined behavior is still raised by
// the empty function body and the noreturn attribute.
 
// The external definition of unreachable_impl must be emitted in a separated TU
// due to the rule for inline functions in C.
 
[[noreturn]] inline void unreachable_impl() {}
#define unreachable() (unreachable_impl())
 
#endif

#define CONCAT(a, b) CONCAT2(a, b)

#define CONCAT2(a, b) a ## b

#define MATCH_TYPED_UNION(prefix, variant, bind_name, expression, field) \
	break;case CONCAT(prefix, variant): for(variant bind_name = (expression).CONCAT(as_, variant), *__cond = (void*)0x1; __cond; __cond = NULL)

#define REF_MATCH_TYPED_UNION(prefix, variant, bind_name, expression, field) \
	break;case CONCAT(prefix, variant): for(variant *bind_name = &(expression).CONCAT(as_, variant), *__cond = (void*)0x1; __cond; __cond = NULL)

#endif
