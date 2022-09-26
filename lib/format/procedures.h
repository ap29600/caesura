#include "module.h"

i64 fmt_i64_va      [[nodiscard]] (Byte_Slice data, va_list va, Fmt_Info info);
i64 fmt_u64_va      [[nodiscard]] (Byte_Slice data, va_list va, Fmt_Info info);
i64 fmt_f64_va      [[nodiscard]] (Byte_Slice data, va_list va, Fmt_Info info);
i64 fmt_i32_va      [[nodiscard]] (Byte_Slice data, va_list va, Fmt_Info info);
i64 fmt_u32_va      [[nodiscard]] (Byte_Slice data, va_list va, Fmt_Info info);
i64 fmt_str_va      [[nodiscard]] (Byte_Slice data, va_list va, Fmt_Info info);
i64 fmt_cstr_va     [[nodiscard]] (Byte_Slice data, va_list va, Fmt_Info info);
i64 fmt_rune_va     [[nodiscard]] (Byte_Slice data, va_list va, Fmt_Info info);
i64 fmt_error_va    [[nodiscard]] (Byte_Slice dest, va_list va, Fmt_Info info);
i64 fmt_location_va [[nodiscard]] (Byte_Slice dest, va_list va, Fmt_Info info);
i64 fmt_ptr_va      [[nodiscard]] (Byte_Slice dest, va_list va, Fmt_Info info);
i64 fmt_color_va    [[nodiscard]] (Byte_Slice dest, va_list va, Fmt_Info info);
