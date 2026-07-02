LC: lambda-larry C
==================

STB style single header library

Overview
========

library                      | category    | C Version | description
---------------------------- | ----------- | --------- | -----------
[lc.h](lc.h)                 | Core        |     >=c11 | Monolith library inspired by odin lang, contains everything
[lc_hashmap.h](lc_hashmap.h) | Core        |     >=c99 | Open addressing hashmap
[lc_deflate.h](lc_deflate.h) | Compression |       cXX | Deflate decompression routine
[lc_csv.h](lc_csv.h)         | Parser      |     >=c99 | **BYOB** csv parser
[lc_ini.h](lc_ini.h)         | Parser      |       cXX | **BYOB** ini parser
[lc_json.h](lc_json.h)       | Parser      |       cXX | **BYOB** json parser
[lc_rng.h](lc_rng.h)         | RNG         |       cXX |

Zero allocation API design
==========================

Libraries that does not do any allocation, will be marked as **BYOB** (Bring Your Own Buffer).

The user is expected to provide their own allocated buffer, instead of the
implementation uses `malloc` or custom allocator.

```c
#define LC_IMPLEMENTATION
#include "lc_csv.h"

#include <stdio.h>

extern int
main(void)
{
	const struct lc_sv csv_content = sv_c(
		"name,age,team\r\n"
		"larry,22,red\r\n"
		"alice,20,jng\r\n"
	);

	struct lc_csv csv = {
		.state.buf = csv_content,
		.config.delimiter = ',',
	};

	struct lc_csv_row row = {};

	lc_csv_iter(&csv, &row);
	while (lc_csv_iter(&csv, &row)) {

		extern bool lc_csv_row_iter(
			struct lc_csv *csv,
			struct lc_csv_row *row,
			size_t n, char out[restrict n], // <-- BYOB (Bring Your Own Buffer)
			struct lc_sv *field
		);

		char buffer[128];
		struct lc_sv field = {};
		lc_csv_row_iter(&csv, &row, sizeof buffer, buffer, &field);
		printf("name: %.*s\n", (int)field.length, field.s);
		lc_csv_row_iter(&csv, &row, sizeof buffer, buffer, &field);
		printf("age:  %.*s\n", (int)field.length, field.s);
		lc_csv_row_iter(&csv, &row, sizeof buffer, buffer, &field);
		printf("team: %.*s\n", (int)field.length, field.s);
	}


	return 0;
}
```
