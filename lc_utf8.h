#ifndef LC_UTF8_H
#define LC_UTF8_H

#ifndef LC_UTF8_API
#define LC_UTF8_API extern
#endif

#include <stdbool.h>

#include <stddef.h>
#include <stdint.h>

typedef int32_t lc_rune;

LC_UTF8_API bool lc_utf8_decode(size_t *restrict n, const char *restrict *restrict in, lc_rune *restrict out);
LC_UTF8_API bool lc_utf8_encode(lc_rune in, size_t *restrict out_n, char *restrict *restrict out);

#ifdef LC_IMPLEMENTATION

#ifndef IN_RANGE
#define IN_RANGE(X, LO, HI) (((LO) <= (X)) & ((X) <= (HI)))
#endif

LC_UTF8_API bool
lc_utf8_decode(size_t *restrict n, const char *restrict *restrict in, lc_rune *restrict out)
{
	enum
	{
		UTF_INVALID = 0xFFFD,
	};


	const uint8_t *p  = (const uint8_t *)*in;

	lc_rune cp = 0;

	/// Cheat sheet:
	///
	/// Code point <-> UTF-8 conversion
	/// First code point | Last code point | Byte 1   | Byte 2   | Byte 3   | Byte 4
	/// -----------------+-----------------+----------+----------+----------+---------
	/// U+0000           | U+007F          | 0yyyzzzz |          |          |
	/// U+0080           | U+07FF          | 110xxxyy | 10yyzzzz |          |
	/// U+0800           | U+FFFF          | 1110wwww | 10xxxxyy | 10yyzzzz |
	/// U+010000         | U+10FFFF        | 11110uvv | 10vvwwww | 10xxxxyy | 10yyzzzz
	///
	/// Bit-pattern mask | Lower bound | Upper bound
	/// -----------------+-------------+------------
	/// 0yyyzzzz         | 0x00        | 0x7F
	/// 110xxxyy         | 0xC0        | 0xDF
	/// 1110wwww         | 0xE0        | 0xEF
	/// 11110uvv         | 0xF0        | 0xF7
	///
	/// ID | Plane             | Name                                      | Blocks | Characters
	/// ---+-------------------+-------------------------------------------+--------+------
	/// 0  | 0x000000-0x00FFFF | Basic Multilingual Plane (BMP)            | 164    | 55718
	/// 1  | 0x010000-0x01FFFF | Supplementary Multilingual Plane (SMP)    | 168    | 28869
	/// 2  | 0x020000-0x02FFFF | Supplementary Ideographic Plane (SIP)     | 7      | 61513
	/// 3  | 0x030000-0x03FFFF | Tertiary Ideographic Plane (TIP)          | 3      | 13429
	/// 4  | 0x040000-0x04FFFF | Unassigned                                | 0      | 0
	/// 5  | 0x050000-0x05FFFF | Unassigned                                | 0      | 0
	/// 6  | 0x060000-0x06FFFF | Unassigned                                | 0      | 0
	/// 7  | 0x070000-0x07FFFF | Unassigned                                | 0      | 0
	/// 8  | 0x080000-0x08FFFF | Unassigned                                | 0      | 0
	/// 9  | 0x090000-0x09FFFF | Unassigned                                | 0      | 0
	/// 10 | 0x0A0000-0x0AFFFF | Unassigned                                | 0      | 0
	/// 11 | 0x0B0000-0x0BFFFF | Unassigned                                | 0      | 0
	/// 12 | 0x0C0000-0x0CFFFF | Unassigned                                | 0      | 0
	/// 13 | 0x0D0000-0x0DFFFF | Unassigned                                | 0      | 0
	/// 14 | 0x0E0000-0x0EFFFF | Supplementary Special-purpose Plane (SSP) | 2      | 337
	/// 15 | 0x0F0000-0x0FFFFF | Supplementary Private Use Area-A (SPUA-A) | 1      | 0
	/// 16 | 0x100000-0x10FFFF | Supplementary Private Use Area-B (SPUA-B) | 1      | 0

	if (IN_RANGE(*p, 0x00, 0x7F)) {
		cp = (cp << 6) | (0x7F & *p++);
		goto success;
	}
	if (IN_RANGE(*p, 0xC0, 0xDF)) {
		cp = (cp << 6) | (0x1F & *p++);
		if (0x80 != (0xC0 & *p)) goto fail;
		cp = (cp << 6) | (0x3F & *p++);

		if (!IN_RANGE(cp, 0x0080, 0x07FF))
			goto fail;
		goto success;
	}
	if (IN_RANGE(*p, 0xE0, 0xEF)) {
		cp = (cp << 6) | (0x0F & *p++);
		if (0x80 != (0xC0 & *p)) goto fail;
		cp = (cp << 6) | (0x3F & *p++);
		if (0x80 != (0xC0 & *p)) goto fail;
		cp = (cp << 6) | (0x3F & *p++);

		if (!IN_RANGE(cp, 0x0800, 0xFFFF) | (0x1B == (cp >> 11)))
			goto fail;
		goto success;
	}
	if (IN_RANGE(*p, 0xF0, 0xF4)) {
		cp = (cp << 6) | (0x07 & *p++);
		if (0x80 != (0xC0 & *p)) goto fail;
		cp = (cp << 6) | (0x3F & *p++);
		if (0x80 != (0xC0 & *p)) goto fail;
		cp = (cp << 6) | (0x3F & *p++);
		if (0x80 != (0xC0 & *p)) goto fail;
		cp = (cp << 6) | (0x3F & *p++);

		if (!IN_RANGE(cp, 0x010000, 0x10FFFF) | (0x1B == (cp >> 11)))
			goto fail;
		goto success;
	}

	p++;
	goto fail;

success:
	*out = cp;
	*n -= (const char *)p - *in;
	*in = (const char *)p;
	return true;

fail:
	*out = UTF_INVALID;
	*n -= (const char *)p - *in;
	*in = (const char *)p;
	return false;
}


LC_UTF8_API bool
lc_utf8_encode(lc_rune in, size_t *restrict out_n, char *restrict *restrict out)
{
	char *o = *out;

	/// https://encoding.spec.whatwg.org/#utf-8-encoder
	///
	/// Code point <-> UTF-8 conversion
	/// First code point | Last code point | Byte 1   | Byte 2   | Byte 3   | Byte 4
	/// -----------------+-----------------+----------+----------+----------+---------
	/// U+0000           | U+007F          | 0yyyzzzz |          |          |
	/// U+0080           | U+07FF          | 110xxxyy | 10yyzzzz |          |
	/// U+0800           | U+FFFF          | 1110wwww | 10xxxxyy | 10yyzzzz |
	/// U+010000         | U+10FFFF        | 11110uvv | 10vvwwww | 10xxxxyy | 10yyzzzz

	if (IN_RANGE(in, 0x000000, 0x000007F)) {
		*o++ = 0x00 | ((in >> 00) + 0x00);
		goto done;
	}
	if (IN_RANGE(in, 0x000080, 0x00007FF)) {
		*o++ = 0x00 | ((in >> 06) + 0xC0);
		*o++ = 0x80 | ((in >> 00) & 0x3F);
		goto done;
	}
	if (IN_RANGE(in, 0x000800, 0x000FFFF)) {
		*o++ = 0x00 | ((in >> 12) + 0xE0);
		*o++ = 0x80 | ((in >> 06) & 0x3F);
		*o++ = 0x80 | ((in >> 00) & 0x3F);
		goto done;
	}
	if (IN_RANGE(in, 0x010000, 0x010FFFF)) {
		*o++ = 0x00 | ((in >> 18) + 0xF0);
		*o++ = 0x80 | ((in >> 12) & 0x3F);
		*o++ = 0x80 | ((in >> 06) & 0x3F);
		*o++ = 0x80 | ((in >> 00) & 0x3F);
		goto done;
	}
	return false;

done:
	*out_n = o - *out;
	*out   = o;

	return true;
}

#endif

#endif
