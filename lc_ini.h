#ifndef LC_INI_H
#define LC_INI_H

#ifndef LC_INI_API
#define LC_INI_API extern
#endif

#ifndef LC_HAVE_SV
#define LC_HAVE_SV
#include <stddef.h>

struct lc_sv {
	size_t length;
	const char *s;
};

#endif

#include <stdbool.h>

struct lc_ini_key_value {
	struct lc_sv section;
	struct lc_sv key;
	struct lc_sv value;
};

LC_INI_API bool lc_ini_parse_sv(
	struct lc_sv *iter,
	struct lc_ini_key_value *kv,
	size_t n, char section[restrict n],
	size_t m, char buffer[restrict m]
);

#ifdef LC_IMPLEMENTATION

LC_INI_API bool lc_ini_parse_sv(
	struct lc_sv *iter,
	struct lc_ini_key_value *kv,
	size_t n, char section[restrict n],
	size_t m, char buffer[restrict m]
)
{
	char *s = section;
	char *t = section + n;
	char *p = buffer;
	char *q = buffer + m;
	char *d = p;

skip_whitespace:
	if (0 == iter->length)
		return false;

	switch (iter->s[0]) {
	case ' ':  iter->s++, iter->length--; goto skip_whitespace;
	case '\t': iter->s++, iter->length--; goto skip_whitespace;
	case '\r': iter->s++, iter->length--; goto skip_whitespace;
	case '\n': iter->s++, iter->length--; goto skip_whitespace;
	default:   break;
	}

	switch (iter->s[0]) {
	case ';':  iter->s++, iter->length--; d = p; goto skip_comment;
	case '#':  iter->s++, iter->length--; d = p; goto skip_comment;
	case '[':  iter->s++, iter->length--; d = s; goto parse_section;
	default:                              d = p; goto parse_key;
	}

parse_section:
	if (0 == iter->length)
		return false;
	if (t <= s)
		return false;

	switch (iter->s[0]) {
	case ']':
		kv->section.s = d, kv->section.length = s - d;
		iter->s++, iter->length--;
		goto skip_whitespace;
	default:
		*s++ = *iter->s++, iter->length--;
		goto parse_section;
	}

parse_key:
	if (0 == iter->length)
		return false;
	if (q <= p)
		return false;

	switch (iter->s[0]) {
	case '=':
		kv->key.s = d, kv->key.length = p - d;
		do {
			iter->s++, iter->length--;
		} while (iter->length && ' ' == iter->s[0]);
		d = p;
		goto parse_value;
	default:
		*p++ = *iter->s++, iter->length--;
		goto parse_key;
	}

parse_value:
	if (0 == iter->length)
		return false;
	if (q <= p)
		return false;

	switch (iter->s[0]) {
	case '\n':
		kv->value.s = d, kv->value.length = p - d;
		iter->s++, iter->length--;
		d = p;
		goto done;
	default:
		*p++ = *iter->s++, iter->length--;
		goto parse_value;
	}


skip_comment:
	if (0 == iter->length)
		return false;

	switch (iter->s[0]) {
	default:   iter->s++, iter->length--; goto skip_comment;
	case '\n': goto skip_whitespace;
	}

done:
	while (kv->key.length && ' ' == kv->key.s[kv->key.length - 1])
		kv->key.length--;

	return true;
}

#endif

#endif

