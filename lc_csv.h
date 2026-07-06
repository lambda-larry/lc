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

		/// single line comment token if non-zero
		/// Only parsed at start of line
		char comment;
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

#ifndef IN_RANGE
#define IN_RANGE(X, LO, HI) (((LO) <= (X)) & ((X) <= (HI)))
#endif

LC_CSV_API bool
lc_csv_iter(struct lc_csv *csv, struct lc_csv_row *row)
{
	const char *p = csv->state.buf.s;
	const char *e = csv->state.buf.length + p;
	const char *o = p;

	if (p >= e)
		return false;

skip_comment:
	if (csv->config.comment && csv->config.comment == *p) {

skip_comment_next:
		if (++p >= e)
			return false;

		if (IN_RANGE(p[0], 0x0D, 0x0D) & IN_RANGE(p[1], 0x0A, 0x0A)) {
			p += 2;
			goto skip_comment;
		}

		goto skip_comment_next;
	}

	csv->state.buf.s      = p;
	csv->state.buf.length = e - p;

quote_outside:
	if (p >= e)
		goto eof;

	if (IN_RANGE(*p, 0x22, 0x22)) {
		p++; goto quote_inside;
	}

	if (IN_RANGE(p[0], 0x0D, 0x0D) & IN_RANGE(p[1], 0x0A, 0x0A)) {
		o = p + 2;
		goto eol;
	}

	p++; goto quote_outside;

quote_inside:
	if (p >= e)
		return false;

	if (IN_RANGE(p[0], 0x22, 0x22)) {
		if (IN_RANGE(p[1], 0x22, 0x22)) {
			p++; p++; goto quote_inside;
		}
		p++; goto quote_outside;
	}

	p++; goto quote_inside;

eof:
	o = p;

eol:
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

	if (!csv->config.delimiter)
		csv->config.delimiter = ',';

	if (csv->config.delimiter == *p)
		goto done;

	if (IN_RANGE(*p, 0x22, 0x22)) {
		p++;
		goto quoted_begin;
	}

	if (IN_RANGE(*p, 0x20, 0x7E))
		goto field_begin;

	lc_unreachable();


quoted_begin:

quoted:
	if (IN_RANGE(*p, 0x22, 0x22)) {
		p++;
		goto quoted_end;
	}

	if (IN_RANGE(*p, 0x20, 0x7E) | IN_RANGE(*p, 0x0D, 0x0D) | IN_RANGE(*p, 0x0A, 0x0A)) {
		*o++ = *p++;
		goto quoted;
	}

	lc_unreachable();

quoted_end:
	if (IN_RANGE(*p, 0x22, 0x22)) {
		*o++ = *p++;
		goto quoted;
	}

	if (csv->config.delimiter == *p)
		goto done;
	goto done_eof;

	lc_unreachable();

field_begin:

field:
	if (csv->config.delimiter == *p)
		goto field_end;

	if (p >= e)
		goto done_eof;

	if (IN_RANGE(*p, 0x20, 0x7E)) {
		*o++ = *p++;
		goto field;
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
