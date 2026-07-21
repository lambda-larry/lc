#ifndef LC_JSON_H
#define LC_JSON_H

/// Table of content
///
/// @lc_toc_begin
/// - Config ........................................................8. [LC_JSC]
/// - Types ........................................................24. [LC_JST]
/// - Macro ........................................................29. [LC_JSM]
/// - Parser ......................................................505. [LC_JSP]
/// - Expermiental API ............................................198. [LC_JXP]
/// @lc_toc_end

////////////////////////////////////////////////////////////////////////////////
/// Config                                                          [LC_JSC] ///
////////////////////////////////////////////////////////////////////////////////

#ifndef LC_JSON_API
#define LC_JSON_API extern
#endif

////////////////////////////////////////////////////////////////////////////////
/// Types                                                           [LC_JST] ///
////////////////////////////////////////////////////////////////////////////////

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

typedef int8_t  i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef float  f32;
typedef double f64;

struct lc_json {
	const char *raw;
};

////////////////////////////////////////////////////////////////////////////////
/// Macro                                                           [LC_JSM] ///
////////////////////////////////////////////////////////////////////////////////


#define lc_json_obj_foreach_begin(json, size, buffer, n)                                                     \
	if (lc_json_obj_begin(json)) {                                                                       \
		if (!lc_json_obj_end(json)) {                                                                \
			do {                                                                                 \
				if (lc_json_key(json, size, buffer, n)) {
#define lc_json_obj_foreach_end(json)                                                                        \
				}                                                                            \
			} while (lc_json_comma(json));                                                       \
                                                                                                             \
			lc_json_obj_end(json);                                                               \
		}                                                                                            \
	}

#define lc_json_arr_foreach_begin(json)                                                                      \
	if (lc_json_arr_begin(json)) {                                                                       \
		if (!lc_json_arr_end(json)) {                                                                \
			do {
#define lc_json_arr_foreach_end(json)                                                                        \
			} while (lc_json_comma(json));                                                       \
			lc_json_arr_end(json);                                                               \
		}                                                                                            \
	}


////////////////////////////////////////////////////////////////////////////////
/// Parser                                                          [LC_JSP] ///
////////////////////////////////////////////////////////////////////////////////

LC_JSON_API bool lc_json_null(struct lc_json *json);
LC_JSON_API bool lc_json_bool(struct lc_json *json, bool *out);
LC_JSON_API void lc_json_ws(struct lc_json *json);
LC_JSON_API bool lc_json_comma(struct lc_json *json);
LC_JSON_API bool lc_json_obj_begin(struct lc_json *json);
LC_JSON_API bool lc_json_obj_end(struct lc_json *json);
LC_JSON_API bool lc_json_arr_begin(struct lc_json *json);
LC_JSON_API bool lc_json_arr_end(struct lc_json *json);
LC_JSON_API bool lc_json_string(struct lc_json *json, size_t n, char out[restrict n], size_t *out_n);

LC_JSON_API bool lc_json_i8(struct lc_json *json, i8 *out);
LC_JSON_API bool lc_json_i16(struct lc_json *json, i16 *out);
LC_JSON_API bool lc_json_i32(struct lc_json *json, i32 *out);
LC_JSON_API bool lc_json_i64(struct lc_json *json, i64 *out);
LC_JSON_API bool lc_json_u8(struct lc_json *json, u8 *out);
LC_JSON_API bool lc_json_u16(struct lc_json *json, u16 *out);
LC_JSON_API bool lc_json_u32(struct lc_json *json, u32 *out);
LC_JSON_API bool lc_json_u64(struct lc_json *json, u64 *out);

LC_JSON_API bool lc_json_f32(struct lc_json *json, f32 *out);
LC_JSON_API bool lc_json_f64(struct lc_json *json, f64 *out);

LC_JSON_API bool lc_json_key(struct lc_json *json, size_t n, char out[restrict n], size_t *out_n);

#ifdef LC_IMPLEMENTATION

LC_JSON_API bool
lc_json_null(struct lc_json *json)
{
	const char *p = json->raw;
	extern int memcmp(const void *s1, const void *s2, size_t n);

	switch (*p) {
	case 'n': if (0 == memcmp(p, "null", 4)) p += 4; else return false; break;
	default: return false;
	}

	json->raw = p;
	return true;
}

LC_JSON_API bool
lc_json_bool(struct lc_json *json, bool *out)
{
	const char *p = json->raw;
	extern int memcmp(const void *s1, const void *s2, size_t n);

	switch (*p) {
	case 't': if (0 == memcmp(p, "true",  4)) p += 4, *out = true;  else return false; break;
	case 'f': if (0 == memcmp(p, "false", 5)) p += 5, *out = false; else return false; break;
	default: return false;
	}

	json->raw = p;
	return true;
}

LC_JSON_API void
lc_json_ws(struct lc_json *json)
{
	const char *p = json->raw;

skip:
	switch (*p) {
	case 0x20: p++; goto skip;
	case '\n': p++; goto skip;
	case '\r': p++; goto skip;
	case '\t': p++; goto skip;
	default:        goto done;
	}

done:
	json->raw = p;
}

LC_JSON_API bool
lc_json_comma(struct lc_json *json)
{
	lc_json_ws(json);

	const char *p = json->raw;

	switch (*p) {
	case ',': p++; break;
	default: return false;
	}

	json->raw = p;
	lc_json_ws(json);
	return true;
}

LC_JSON_API bool
lc_json_obj_begin(struct lc_json *json)
{
	const char *p = json->raw;

	if ('{' != *p++)
		return false;

	json->raw = p;
	lc_json_ws(json);
	return true;
}

LC_JSON_API bool
lc_json_obj_end(struct lc_json *json)
{
	lc_json_ws(json);

	const char *p = json->raw;

	if ('}' != *p++)
		return false;

	json->raw = p;
	lc_json_ws(json);
	return true;
}

LC_JSON_API bool
lc_json_arr_begin(struct lc_json *json)
{
	const char *p = json->raw;

	if ('[' != *p++)
		return false;

	json->raw = p;
	lc_json_ws(json);
	return true;
}

LC_JSON_API bool
lc_json_arr_end(struct lc_json *json)
{
	lc_json_ws(json);

	const char *p = json->raw;

	if (']' != *p++)
		return false;

	json->raw = p;
	lc_json_ws(json);
	return true;
}

LC_JSON_API bool
lc_json_string(struct lc_json *json, size_t n, char out[restrict n], size_t *out_n)
{
	const char *p = json->raw;
	const char *end = out + n;

	if ('"' != *p++)
		return false;

parse:

	if (out == end)
		return false;

	switch (*p) {
	case '"':            p++; goto done;
	case '\\':           p++; goto escaped;
	default:   *out++ = *p++; goto parse;
	}

	return false;

escaped:

	switch (*p) {
	case '"':  p++; *out++ = '"';  goto parse;
	case '/':  p++; *out++ = '/';  goto parse;
	case '\\': p++; *out++ = '\\'; goto parse;
	case 'b':  p++; *out++ = '\b'; goto parse;
	case 'f':  p++; *out++ = '\f'; goto parse;
	case 'n':  p++; *out++ = '\n'; goto parse;
	case 'r':  p++; *out++ = '\r'; goto parse;
	case 't':  p++; *out++ = '\t'; goto parse;
	case 'u':  p++; goto escaped_hex;
	}

	typedef u32 rune;
	typedef u16 utf16;
escaped_hex:
	;

	utf16 utf16_codepoint = 0;
	for (u16 i = 0; i < 4; i++) {
		switch (*p) {
		case '0' ... '9': utf16_codepoint <<= 4; utf16_codepoint += 00 + *p++ - '0'; continue;
		case 'a' ... 'f': utf16_codepoint <<= 4; utf16_codepoint += 10 + *p++ - 'a'; continue;
		case 'A' ... 'F': utf16_codepoint <<= 4; utf16_codepoint += 10 + *p++ - 'A'; continue;
		default: return false;
		}
	}

	rune code_point = 0;

utf16_decode:
	{
		enum
		{
			LEAD_OFFSET      = 0x0D800 - (0x10000 >> 10),
			SURROGATE_OFFSET = 0x10000 - (0x0D800 << 10) - 0x0DC00,
		};

		// computations
		const utf16 lead  = LEAD_OFFSET + (utf16_codepoint >> 10);
		const utf16 trail = 0xDC00      + (utf16_codepoint  & 0x3FF);

		code_point = (lead << 10) + trail + SURROGATE_OFFSET;
	}

utf8_encode:
	{
		/// https://encoding.spec.whatwg.org/#utf-8-encoder

		switch (code_point) {
		case 0x000000 ... 0x000007F:
			if (out + 0 >= end) return false;
			*out++ = 0x00 | ((code_point >> 00) + 0x00);
			goto parse;
		case 0x000080 ... 0x00007FF:
			if (out + 1 >= end) return false;
			*out++ = 0x00 | ((code_point >> 06) + 0xC0);
			*out++ = 0x80 | ((code_point >> 00) & 0x3F);
			goto parse;
		case 0x000800 ... 0x000FFFF:
			if (out + 2 >= end) return false;
			*out++ = 0x00 | ((code_point >> 12) + 0xE0);
			*out++ = 0x80 | ((code_point >> 06) & 0x3F);
			*out++ = 0x80 | ((code_point >> 00) & 0x3F);
			goto parse;
		case 0x010000 ... 0x010FFFF:
			if (out + 3 >= end) return false;
			*out++ = 0x00 | ((code_point >> 18) + 0xF0);
			*out++ = 0x80 | ((code_point >> 12) & 0x3F);
			*out++ = 0x80 | ((code_point >> 06) & 0x3F);
			*out++ = 0x80 | ((code_point >> 00) & 0x3F);
			goto parse;
		default: return false;
		}
	}

done:
	json->raw = p;
	*out = 0;
	*out_n = n - (end - out);

	return true;
}

#define LC_JSON_INT(type)                                                                                    \
	LC_JSON_API bool lc_json_##type(struct lc_json *json, type *out)                                     \
	{                                                                                                    \
		const char *p          = json->raw;                                                          \
		i64 acc                = 0;                                                                  \
		u32 exponent           = 0;                                                                  \
		u32 frac               = 0;                                                                  \
		u32 fraction_len       = 0;                                                                  \
		bool negative          = false;                                                              \
		bool exponent_negative = false;                                                              \
                                                                                                             \
		if ('-' == *p) {                                                                             \
			negative = true;                                                                     \
			p++;                                                                                 \
		}                                                                                            \
                                                                                                             \
		switch (*p) {                                                                                \
		case '0' ... '0': acc *= 10; acc += *p++ - '0'; goto parse_zero;                             \
		case '1' ... '9': acc *= 10; acc += *p++ - '0'; goto parse_digit;                            \
		default: return false;                                                                       \
		}                                                                                            \
                                                                                                             \
parse_zero:                                                                                                  \
		switch (*p) {                                                                                \
		case '.': p++; goto parse_fraction;                                                          \
		case 'e': p++; goto parse_exponent;                                                          \
		case 'E': p++; goto parse_exponent;                                                          \
		default:  goto done;                                                                         \
		}                                                                                            \
                                                                                                             \
parse_digit:                                                                                                 \
		switch (*p) {                                                                                \
		case '0' ... '9': acc *= 10; acc += *p++ - '0'; goto parse_digit;                            \
		case '.': p++; goto parse_fraction;                                                          \
		case 'e': p++; goto parse_exponent;                                                          \
		case 'E': p++; goto parse_exponent;                                                          \
		default:  goto done;                                                                         \
		}                                                                                            \
                                                                                                             \
parse_fraction:                                                                                              \
		switch (*p) {                                                                                \
		case '0' ... '9': frac *= 10; frac += *p++ - '0'; fraction_len++; goto parse_fraction;       \
		case 'e': p++; goto parse_exponent;                                                          \
		case 'E': p++; goto parse_exponent;                                                          \
		default:  goto done;                                                                         \
		}                                                                                            \
                                                                                                             \
parse_exponent:                                                                                              \
                                                                                                             \
		switch (*p) {                                                                                \
		default: exponent_negative = false; goto parse_exponent_digit;                               \
		case '-':                                                                                    \
			p++;                                                                                 \
			exponent_negative = true;                                                            \
			goto parse_exponent_digit;                                                           \
		case '+':                                                                                    \
			p++;                                                                                 \
			exponent_negative = false;                                                           \
			goto parse_exponent_digit;                                                           \
		}                                                                                            \
                                                                                                             \
                                                                                                             \
parse_exponent_digit:                                                                                        \
		switch (*p) {                                                                                \
		case '0' ... '9':                                                                            \
			exponent *= 10;                                                                      \
			exponent += *p++ - '0';                                                              \
			goto parse_exponent_digit;                                                           \
		default: goto done;                                                                          \
		}                                                                                            \
                                                                                                             \
done:                                                                                                        \
                                                                                                             \
		if (negative)                                                                                \
			acc = -acc;                                                                          \
                                                                                                             \
		if (exponent_negative) {                                                                     \
			/* At this point, we don't care about fraction */                                    \
                                                                                                             \
			for (u32 i = 0; i < exponent; i++)                                                   \
				acc /= 10;                                                                   \
		} else {                                                                                     \
			/* At this point, we _do_ care about fraction */                                     \
			/* 1. trim trailing zero from fraction        */                                     \
			/* 2. resize fraction to exponent size        */                                     \
			/* 3. add to acc                              */                                     \
                                                                                                             \
			for (u32 i = 0; i < exponent; i++)                                                   \
				acc *= 10;                                                                   \
                                                                                                             \
			while (0 < frac && 0 == frac % 10) {                                                 \
				fraction_len--;                                                              \
				frac /= 10;                                                                  \
			}                                                                                    \
                                                                                                             \
			if (fraction_len < exponent) {                                                       \
				for (u32 i = fraction_len; i < exponent; i++)                                \
					frac *= 10;                                                          \
			} else {                                                                             \
				for (u32 i = fraction_len; i < exponent; i++)                                \
					frac /= 10;                                                          \
			}                                                                                    \
                                                                                                             \
			acc += frac;                                                                         \
		}                                                                                            \
                                                                                                             \
		json->raw = p;                                                                               \
		*out = acc;                                                                                  \
                                                                                                             \
		return true;                                                                                 \
	}                                                                                                    \
	LC_JSON_API bool lc_json_##type(struct lc_json *json, type *out)

LC_JSON_INT(i8);
LC_JSON_INT(i16);
LC_JSON_INT(i32);
LC_JSON_INT(i64);

LC_JSON_INT(u8);
LC_JSON_INT(u16);
LC_JSON_INT(u32);
LC_JSON_INT(u64);

LC_JSON_API bool
lc_json_f32(struct lc_json *json, f32 *out)
{
	const char *p = json->raw;

	if ('-' == *p) {
		p++;
	}

	switch (*p) {
	case '0' ... '0': p++; goto parse_zero;
	case '1' ... '9': p++; goto parse_digit;
	default: return false;
	}

parse_zero:
	switch (*p) {
	case '.':         p++; goto parse_fraction;
	case 'e':         p++; goto parse_exponent;
	case 'E':         p++; goto parse_exponent;
	default:  goto done;
	}

parse_digit:
	switch (*p) {
	case '0' ... '9': p++; goto parse_digit;
	case '.':         p++; goto parse_fraction;
	case 'e':         p++; goto parse_exponent;
	case 'E':         p++; goto parse_exponent;
	default:  goto done;
	}

parse_fraction:
	switch (*p) {
	case '0' ... '9': p++; goto parse_fraction;
	case 'e':         p++; goto parse_exponent;
	case 'E':         p++; goto parse_exponent;
	default:          goto done;
	}

parse_exponent:
	switch (*p) {
	case '-': p++; goto parse_exponent_digit;
	case '+': p++; goto parse_exponent_digit;
	default:       goto parse_exponent_digit;
	}

parse_exponent_digit:
	switch (*p) {
	case '0' ... '9': p++; goto parse_exponent_digit;
	default:               goto done;
	}

done:
	;

	extern f32 strtof(const char *restrict str, char **restrict str_end);
	*out = strtof(json->raw, NULL);
	json->raw = p;

	return true;
}

LC_JSON_API bool
lc_json_f64(struct lc_json *json, f64 *out)
{
	const char *p = json->raw;

	if ('-' == *p) {
		p++;
	}

	switch (*p) {
	case '0' ... '0': p++; goto parse_zero;
	case '1' ... '9': p++; goto parse_digit;
	default: return false;
	}

parse_zero:
	switch (*p) {
	case '.':         p++; goto parse_fraction;
	case 'e':         p++; goto parse_exponent;
	case 'E':         p++; goto parse_exponent;
	default:  goto done;
	}

parse_digit:
	switch (*p) {
	case '0' ... '9': p++; goto parse_digit;
	case '.':         p++; goto parse_fraction;
	case 'e':         p++; goto parse_exponent;
	case 'E':         p++; goto parse_exponent;
	default:  goto done;
	}

parse_fraction:
	switch (*p) {
	case '0' ... '9': p++; goto parse_fraction;
	case 'e':         p++; goto parse_exponent;
	case 'E':         p++; goto parse_exponent;
	default:          goto done;
	}

parse_exponent:
	switch (*p) {
	case '-': p++; goto parse_exponent_digit;
	case '+': p++; goto parse_exponent_digit;
	default:       goto parse_exponent_digit;
	}

parse_exponent_digit:
	switch (*p) {
	case '0' ... '9': p++; goto parse_exponent_digit;
	default:               goto done;
	}

done:
	;

	extern f64 strtod(const char *restrict str, char **restrict str_end);
	*out = strtod(json->raw, NULL);
	json->raw = p;

	return true;
}

#define lc_json_key_match_c(json, key) lc_json_key_match(json, sizeof key - 1, key)

LC_JSON_API bool
lc_json_key(struct lc_json *json, size_t n, char out[restrict n], size_t *out_n)
{
	const char *p = json->raw;
	if (lc_json_string(json, n, out, out_n)) {
		lc_json_ws(json);
		switch (*json->raw) {
		default:  goto fail;
		case ':': json->raw++;
		}
		lc_json_ws(json);
		return true;
	}
fail:

	json->raw = p;
	return false;
}

#endif

////////////////////////////////////////////////////////////////////////////////
/// Expermiental API                                                [LC_JXP] ///
////////////////////////////////////////////////////////////////////////////////

LC_JSON_API bool lc_json_key_match(struct lc_json *json, size_t n, char key[restrict n]);
LC_JSON_API bool lc_json_key_f32(struct lc_json *json, size_t n, char key[restrict n], f32 *out);
LC_JSON_API void lc_json_value_skip(struct lc_json *json, size_t n, char buffer[restrict n]);

#ifdef LC_IMPLEMENTATION

LC_JSON_API bool
lc_json_key_match(struct lc_json *json, size_t n, char key[restrict n])
{
	const char *p   = json->raw;
	const char *s   = key;
	const char *end = key + n;

	if ('"' != *p++)
		return false;

parse:


	switch (*p) {
	case '"':            p++; goto done_string;
	case '\\':           p++; goto escaped;
	default:
		if (s == end)
			return false;

		if (*s++ == *p++)
			goto parse;

		return false;
	}

	return false;

escaped:

	switch (*p) {
	case '"':  p++; if ('"'  == *s++) goto parse; return false;
	case '/':  p++; if ('/'  == *s++) goto parse; return false;
	case '\\': p++; if ('\\' == *s++) goto parse; return false;
	case 'b':  p++; if ('\b' == *s++) goto parse; return false;
	case 'f':  p++; if ('\f' == *s++) goto parse; return false;
	case 'n':  p++; if ('\n' == *s++) goto parse; return false;
	case 'r':  p++; if ('\r' == *s++) goto parse; return false;
	case 't':  p++; if ('\t' == *s++) goto parse; return false;
	case 'u':  p++; goto escaped_hex;
	}

	typedef u32 rune;
	typedef u16 utf16;
escaped_hex:
	;

	utf16 utf16_codepoint = 0;
	for (u16 i = 0; i < 4; i++) {
		switch (*p) {
		case '0' ... '9': utf16_codepoint <<= 4; utf16_codepoint += 00 + *p++ - '0'; continue;
		case 'a' ... 'f': utf16_codepoint <<= 4; utf16_codepoint += 10 + *p++ - 'a'; continue;
		case 'A' ... 'F': utf16_codepoint <<= 4; utf16_codepoint += 10 + *p++ - 'A'; continue;
		default: return false;
		}
	}

	rune code_point = 0;

utf16_decode:
	{
		enum
		{
			LEAD_OFFSET      = 0x0D800 - (0x10000 >> 10),
			SURROGATE_OFFSET = 0x10000 - (0x0D800 << 10) - 0x0DC00,
		};

		// computations
		const utf16 lead  = LEAD_OFFSET + (utf16_codepoint >> 10);
		const utf16 trail = 0xDC00      + (utf16_codepoint  & 0x3FF);

		code_point = (lead << 10) + trail + SURROGATE_OFFSET;
	}

utf8_encode:
	{
		/// https://encoding.spec.whatwg.org/#utf-8-encoder

		switch (code_point) {
		case 0x000000 ... 0x000007F:
			if (s + 0 >= end) return false;
			if ((char)(0x00 | ((code_point >> 00) + 0x00)) != *s++) return false;
			goto parse;
		case 0x000080 ... 0x00007FF:
			if (s + 1 >= end) return false;
			if ((char)(0x00 | ((code_point >> 06) + 0xC0)) != *s++) return false;
			if ((char)(0x80 | ((code_point >> 00) & 0x3F)) != *s++) return false;
			goto parse;
		case 0x000800 ... 0x000FFFF:
			if (s + 2 >= end) return false;
			if ((char)(0x00 | ((code_point >> 12) + 0xE0)) != *s++) return false;
			if ((char)(0x80 | ((code_point >> 06) & 0x3F)) != *s++) return false;
			if ((char)(0x80 | ((code_point >> 00) & 0x3F)) != *s++) return false;
			goto parse;
		case 0x010000 ... 0x010FFFF:
			if (s + 3 >= end) return false;
			if ((char)(0x00 | ((code_point >> 18) + 0xF0)) != *s++) return false;
			if ((char)(0x80 | ((code_point >> 12) & 0x3F)) != *s++) return false;
			if ((char)(0x80 | ((code_point >> 06) & 0x3F)) != *s++) return false;
			if ((char)(0x80 | ((code_point >> 00) & 0x3F)) != *s++) return false;
			goto parse;
		default: return false;
		}
	}

done_string:

skip_ws_1:
	switch (*p) {
	case 0x20: p++; goto skip_ws_1;
	case '\n': p++; goto skip_ws_1;
	case '\r': p++; goto skip_ws_1;
	case '\t': p++; goto skip_ws_1;
	case ':':  p++; goto skip_ws_2;
	default: return false;
	}

skip_ws_2:
	switch (*p) {
	case 0x20: p++; goto skip_ws_1;
	case '\n': p++; goto skip_ws_1;
	case '\r': p++; goto skip_ws_1;
	case '\t': p++; goto skip_ws_1;
	default:        goto done;
	}

done:
	json->raw = p;

	return true;
}

LC_JSON_API bool
lc_json_key_f32(struct lc_json *json, size_t n, char key[restrict n], f32 *out)
{
	const char *p = json->raw;

	if (!lc_json_key_match(json, n, key))
		goto fail;

	if (!lc_json_f32(json, out))
		goto fail;

	return true;

fail:
	json->raw = p;
	return false;
}




LC_JSON_API void
lc_json_value_skip(struct lc_json *json, size_t n, char buffer[restrict n])
{
	if (lc_json_null(json))
		return;
	if (lc_json_bool(json, &(bool){0}))
		return;
	if (lc_json_f32(json,  &(f32){0}))
		return;
	if (lc_json_string(json, n, buffer, &(size_t){0}))
		return;
	if (lc_json_obj_begin(json)) {
		if (!lc_json_obj_end(json)) {
			do {
				if (lc_json_key(json, n, buffer, &(size_t){0})) {
					lc_json_value_skip(json, n, buffer);
				}
			} while (lc_json_comma(json));
			lc_json_obj_end(json);
		}
		return;
	}

	if (lc_json_arr_begin(json)) {
		if (!lc_json_arr_end(json)) {
			do {
				lc_json_value_skip(json, n, buffer);
			} while (lc_json_comma(json));
			lc_json_arr_end(json);
		}
		return;
	}
}

#endif

#endif
