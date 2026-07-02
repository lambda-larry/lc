#ifndef LC_CSV_H
#define LC_CSV_H

#ifndef LC_CSV_API
#define LC_CSV_API extern
#endif

///
/// RFC 4180:
/// https://datatracker.ietf.org/doc/html/rfc4180.html
///

#ifndef LC_HAVE_SV
#define LC_HAVE_SV
#define LEN(X) (sizeof(X) / sizeof(X)[0])

#include <stddef.h>

struct lc_sv {
	size_t length;
	const char *s;
};

#define sv(...)       ((struct lc_sv){__VA_ARGS__})
#define sv_c(str)     sv(.length = LEN(str) - 1, .s = str)

#endif

#ifndef lc_unreachable
#if __STDC_VERSION__ < 202311L
#if LC_CC_GNU
#define lc_unreachable __builtin_unreachable
#else
#define lc_unreachable() *((void *volatile *volatile)NULL)
#endif
#else
#define lc_unreachable unreachable
#endif
#endif

#include <stdbool.h>

struct lc_csv {
	struct {
		struct lc_sv buf;
	} state;
	struct {
		char delimiter;
	} config;
};

struct lc_csv_row {
	struct {
		struct lc_sv buf;
	} state;
};

LC_CSV_API bool lc_csv_iter(struct lc_csv *csv, struct lc_csv_row *row);
LC_CSV_API bool lc_csv_row_iter(
	struct lc_csv *csv,
	struct lc_csv_row *row,
	size_t n, char out[restrict n],
	struct lc_sv *field
);

#ifdef LC_IMPLEMENTATION

LC_CSV_API bool
lc_csv_iter(struct lc_csv *csv, struct lc_csv_row *row)
{
	const char *p = csv->state.buf.s;
	const char *e = csv->state.buf.length + p;
	const char *o = p;

	if (p >= e)
		return false;

	bool in_quote = false;
	for (; p < e; p++) {
		if (!in_quote) {
			if ('"' == p[0]) {
				in_quote = true;
			}
			if ('\r' == p[0]) {
				if ('\n' == p[1]) {
					o = p + 2;
					goto done;
				}
			}
		} else {
			if ('"' == p[0]) {
				if ('"' == p[1]) {
					p++; p++;
				} else {
					in_quote = false;
				}
			}
		}
	}

	o = p;

done:
	row->state.buf.s      = csv->state.buf.s;
	row->state.buf.length = p - csv->state.buf.s;
	csv->state.buf.length = e - o;
	csv->state.buf.s      = o;

	return true;
}

LC_CSV_API bool
lc_csv_row_iter(
	struct lc_csv *csv,
	struct lc_csv_row *row,
	size_t n, char out[restrict n],
	struct lc_sv *field
)
{
	const char *p = row->state.buf.s;
	const char *e = row->state.buf.length + p;

	char *o = out;

	if (p >= e)
		return false;

	if (!csv->config.delimiter) {
		csv->config.delimiter = ',';
	}

	if (csv->config.delimiter == *p)
		goto done;

	switch (*p) {
	case 0x22: p++; goto quoted_begin;

	case 0x20 ... 0x21: goto field_begin;
	case 0x23 ... 0x2B: goto field_begin;
	case 0x2C ... 0x2C: goto field_begin; // comma (in case another delimiter is used)
	case 0x2D ... 0x7E: goto field_begin;

	}

	lc_unreachable();


quoted_begin:

quoted:
	switch (*p) {
	case 0x0D ... 0x0D: *o++ = *p++; goto quoted;
	case 0x0A ... 0x0A: *o++ = *p++; goto quoted;

	case 0x20 ... 0x21: *o++ = *p++; goto quoted;
	case 0x22 ... 0x22:         p++; goto quoted_end;
	case 0x23 ... 0x2B: *o++ = *p++; goto quoted;
	case 0x2C ... 0x2C: *o++ = *p++; goto quoted;
	case 0x2D ... 0x7E: *o++ = *p++; goto quoted;
	}

	lc_unreachable();

quoted_end:
	switch (*p) {
	case 0x22 ... 0x22: *o++ = *p++; goto quoted;
	default:
		if (csv->config.delimiter == *p)
			goto done;
		goto done_eof;
	}

	lc_unreachable();

field_begin:

field:
	if (csv->config.delimiter == *p)
		goto field_end;

	if (p >= e)
		goto done_eof;

	switch (*p) {
	case 0x20 ... 0x21: *o++ = *p++; goto field;
	case 0x23 ... 0x2B: *o++ = *p++; goto field;
	case 0x2C ... 0x2C: *o++ = *p++; goto field;
	case 0x2D ... 0x7E: *o++ = *p++; goto field;
	}

	lc_unreachable();

field_end:
	goto done;

done:
	++p;

done_eof:
	field->s      = out;
	field->length = o - out;

	row->state.buf.s      = p;
	row->state.buf.length = e - p;

	return true;
}

#endif

#endif
