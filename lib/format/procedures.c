#include "procedures.h"

i64 fmt_f64(Byte_Slice dest, f64 val, Fmt_Info info) {
	const char *begin = dest.begin;
	if (val < 0) {
		dest.begin += fmt_rune(dest,  '-', info);
		val = -val;
	}

	u64 integral = (u64)val;
	dest.begin += fmt_u64(dest, integral, info);
	u64 decimal = (val - integral) * 1000.0 + 0.5;

	if (decimal) {
		dest.begin += fmt_rune(dest, '.', info);
		if (decimal < 100) { dest.begin += fmt_rune(dest,  '0', info); }
		if (decimal < 10) {  dest.begin += fmt_rune(dest,  '0', info); }
		dest.begin += fmt_u64(dest, decimal, info);
	}

	return dest.begin - begin;
}


i64 fmt_i64(Byte_Slice destination, i64 val, Fmt_Info) {
	char bytes[32];
	u64  bytes_count = 0;
	bool is_negative = val < 0;

	if (val < 0) { val = -val; }

	if (val == 0) {
		bytes[bytes_count++] = '0';
	} else {
		for(; val > 0; val /= 10) {
			bytes[bytes_count++] = (val % 10) + '0';
		}
	}

	if (is_negative) { bytes[bytes_count++] = '-'; }

	u64 len = destination.end - destination.begin;
	for (u64 i = 0; i < bytes_count && i < len; ++i) {
		destination.begin[i] = bytes[bytes_count - i - 1];
	}
	return bytes_count;
}


i64 fmt_ptr(Byte_Slice destination, uintptr_t val, Fmt_Info) {
	char map[16] = "0123456789ABCDEF";
	u64 i = 0;
	for(; i < (sizeof(uintptr_t) * 2) && destination.begin < destination.end; ++i, ++destination.begin) {
		*destination.begin = map[val >> 60];
		val <<= 4;
	}
	return i;
}


i64 fmt_u64(Byte_Slice destination, u64 val, Fmt_Info) {
	char bytes[32];
	u64  bytes_count = 0;

	if (val == 0) {
		bytes[bytes_count++] = '0';
	} else {
		for(; val > 0; val /= 10) {
			bytes[bytes_count++] = (val % 10) + '0';
		}
	}

	u64 len = destination.end - destination.begin;
	for (u64 i = 0; i < bytes_count && i < len; ++i) {
		destination.begin[i] = bytes[bytes_count - i - 1];
	}

	return bytes_count;
}

i64 fmt_str(Byte_Slice destination, String src, Fmt_Info) {
	u64 bytes = min(src.end - src.begin, destination.end - destination.begin);
	for (u64 i = 0; i < bytes; ++i) {
		destination.begin[i] = src.begin[i];
	}
	return bytes;
}

i64 fmt_cstr(Byte_Slice destination, const char *src, Fmt_Info) {
	if (!src) { return 0; }
	i64 max_len = destination.end - destination.begin;
	i64 bytes = 0;
	for (; bytes < max_len && src[bytes] != '\0'; ++bytes) {
		destination.begin[bytes] = src[bytes];
	}
	return bytes;
}


i64 fmt_rune(Byte_Slice destination, rune src, Fmt_Info) {
	/// TODO: handle utf-8 encoding
	assert(rune_width(src) == 1);
	if (destination.end - destination.begin > 0) {
		destination.begin[0] = src;
		return 1;
	} else {
		return 0;
	}
}

i64 fmt_location(Byte_Slice dest, Location loc, Fmt_Info info) {
	info = (Fmt_Info){0};
	char *const begin = dest.begin;
	dest.begin += fmt_cstr (dest, loc.fname,   info);
	dest.begin += fmt_rune (dest, ':',         info);
	dest.begin += fmt_u64  (dest, loc.row + 1, info);
	dest.begin += fmt_rune (dest, ':',         info);
	dest.begin += fmt_u64  (dest, loc.col + 1, info);
	return dest.begin - begin;
}

i64 fmt_error(Byte_Slice dest, Error src, Fmt_Info info) {
	return fmt_cstr(dest, runtime_error_strings[src], info);
}

i64 fmt_color(Byte_Slice dest, Terminal_Color src, Fmt_Info info) {
	char *const begin = dest.begin;
	dest.begin += fmt_cstr(dest, "\x1b[", info);
	dest.begin += fmt_u64(dest, src, info);
	dest.begin += fmt_rune(dest, 'm', info);
	return dest.begin - begin;
}

i64 fmt_color_va(Byte_Slice dest, va_list va, Fmt_Info info) {
	Terminal_Color src = va_arg(va, Terminal_Color);
	return fmt_color(dest, src, info);
}

/// these versions of formatting functions work on a va_list

i64 fmt_f64_va(Byte_Slice data, va_list va, Fmt_Info info) {
	return fmt_f64(data, va_arg(va, f64), info);
}

i64 fmt_ptr_va (Byte_Slice destination, va_list va, Fmt_Info info) {
	return fmt_ptr(destination, va_arg(va, uintptr_t), info);
}

i64 fmt_u64_va(Byte_Slice destination, va_list va, Fmt_Info info) {
	return fmt_u64(destination, va_arg(va, u64), info);
}

i64 fmt_u32_va(Byte_Slice destination, va_list va, Fmt_Info info) {
	return fmt_u64(destination, va_arg(va, u32), info);
}

i64 fmt_i64_va(Byte_Slice destination, va_list va, Fmt_Info info) {
	return fmt_i64(destination, va_arg(va, i64), info);
}

i64 fmt_i32_va(Byte_Slice destination, va_list va, Fmt_Info info) {
	return fmt_i64(destination, va_arg(va, i32), info);
}

i64 fmt_str_va(Byte_Slice destination, va_list va, Fmt_Info info) {
	return fmt_str(destination, va_arg(va, String), info);
}

i64 fmt_cstr_va(Byte_Slice destination, va_list va, Fmt_Info info) {
	return fmt_cstr(destination, va_arg(va, const char*), info);
}

i64 fmt_rune_va(Byte_Slice destination, va_list va, Fmt_Info info) {
	return fmt_rune(destination, va_arg(va, i32), info);
}

i64 fmt_error_va(Byte_Slice dest, va_list va, Fmt_Info info) {
	return fmt_error(dest, va_arg(va, Error), info);
}

i64 fmt_location_va(Byte_Slice dest, va_list va, Fmt_Info info) {
	return fmt_location(dest, va_arg(va, Location), info);
}
