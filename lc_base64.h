#ifndef LC_BASE64_H
#define LC_BASE64_H

#include <stdint.h>
#include <stddef.h>

#if __STDC_VERSION__ >= 202311L
#define LC_BASE64_UNREACHABLE() unreachable()
#elif defined(__GNUC__) && __GNUC__
#define LC_BASE64_UNREACHABLE() __builtin_unreachable()
#else
extern void abort(void);
#define LC_BASE64_UNREACHABLE() abort()
#endif

static inline size_t
lc_base64_encode(size_t n, const uint8_t in[restrict n], size_t m, char out[restrict m])
{
	uint32_t buffer    = 0;
	uint32_t remaining = 0;

	const uint8_t *in_p   = in;
	const uint8_t *in_end = in + n;

	char *out_p   = out;
	char *out_end = out + m;

	enum
	{
		UINT8_WIDTH = 8,
		COUNT = 6,
		MASK  = (1 << COUNT) - 1,
	};
	static const char decode_tbl[64] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

	while (in_p < in_end && out_p < out_end) {
		if (remaining < COUNT) {
			buffer    <<= UINT8_WIDTH;
			buffer    |=  *in_p++;
			remaining +=  UINT8_WIDTH;
		}

		remaining -= COUNT;
		const uint32_t idx = (buffer >> remaining) & MASK;

		*out_p++ = decode_tbl[idx];
	}
	if (out_p < out_end && remaining) {

		buffer <<= COUNT - remaining;
		const uint32_t idx  = buffer & MASK;

		*out_p++ = decode_tbl[idx];
		switch (remaining) {
		case 5: case 2: *out_p++ = '=';
		case 4: case 1: *out_p++ = '=';
		case 3: case 0: break;
		}
	}

	return out_p - out;
}

static inline size_t
lc_base64_decode(size_t n, const char in[restrict n], size_t m, uint8_t out[restrict m])
{
	uint32_t buffer    = 0;
	uint32_t remaining = 0;

	const char *in_p   = in;
	const char *in_end = in + n;

	uint8_t *out_p   = (uint8_t *)out;
	uint8_t *out_end = (uint8_t *)out + m;

	while (in_p < in_end && out_p < out_end) {
		uint8_t bits = 0;
		if (0) {
		} else if (((uint32_t)*in_p - 'A') <= ('Z' - 'A')) {
			bits = 00 + *in_p++ - 'A';
		} else if (((uint32_t)*in_p - 'a') <= ('z' - 'a')) {
			bits = 26 + *in_p++ - 'a';
		} else if (((uint32_t)*in_p - '0') <= ('9' - '0')) {
			bits = 52 + *in_p++ - '0';
		} else switch (*in_p) {
		case '+': bits = 62 + *in_p++ - '+'; break;
		case '/': bits = 63 + *in_p++ - '/'; break;
		case '=': bits = 00 + *in_p++ - '='; break;
		default: LC_BASE64_UNREACHABLE();
		}

		enum
		{
			UINT8_WIDTH = 8,
			COUNT = 6,
			MASK  = (1 << COUNT) - 1,
		};

		buffer    <<= COUNT;
		buffer     |= MASK & bits;
		remaining  += COUNT;
		if (remaining >= UINT8_WIDTH) {
			remaining -= UINT8_WIDTH;
			*out_p++   = buffer >> remaining;
		}
	}

	return out_p - out;
}
#endif
