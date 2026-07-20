// #define _POSIX_C_SOURCE 200809L
// #define LC_ENABLE_TEST
#define LC_IMPLEMENTATION
#include "../lc.h"

LC_WARN_IGNORE("-Waddress")

#define TEST(msg)                                  if (msg)
#define TEST_GROUP(symbol)                         if (symbol)

#undef assert
#define assert(expr)                                                                                         \
	do {                                                                                                 \
		if (expr) {                                                                                  \
			lc_log_info("SUCCESS");                                                              \
		} else {                                                                                     \
			lc_log_fatal("Failed");                                                              \
		}                                                                                            \
	} while (false)

#define assert_memcmp(actually, expected, size)                                                              \
	do {                                                                                                 \
		extern int memcmp(const void *, const void *, size_t);                                       \
		if (0 == memcmp(actually, expected, size)) {                                                 \
			lc_log_info("SUCCESS");                                                              \
		} else {                                                                                     \
			lc_log_fatal("Failed");                                                              \
		}                                                                                            \
	} while (false)

static void
test_string_view(struct lc_context *ctx, bool enabled)
{
	if (false == enabled) {
		lc_log_info(VT_BOLD "SKIPPED" VT_RESET " %s", __func__);
		return;
	}


	const char _test_data[] = "0123456789";
	static_assert(LEN(_test_data) == 11, "");

	const struct lc_sv test_data = sv_c(_test_data);
	assert(10 == test_data.length);

	TEST_GROUP(lc_sv_sub)
	{
		TEST("Case 1. Normal")
		{
			/// Case 1. Normal
			///
			/// idx: [0;   sv.length]
			/// len: [0; sv.length - idx]
			///
			/// Returns substring as expected

			const size_t idx      = 4;
			const size_t length   = 2;
			struct lc_sv actually = lc_sv_sub(test_data, length, idx);
			struct lc_sv expected = sv(.s = _test_data + idx, .length = length);

			assert_memcmp(&actually, &expected, sizeof expected);
		}

		TEST("Case 1. Normal")
		{
			/// Case 1. Normal
			///
			/// idx: [0;   sv.length]
			/// len: [0; sv.length - idx]
			///
			/// Returns substring as expected

			for (size_t idx = 0; idx < 10; idx++) {
				const size_t length   = 10 - idx;
				struct lc_sv actually = lc_sv_sub(test_data, length, idx);
				struct lc_sv expected = sv(.s = _test_data + idx, .length = length);


				assert_memcmp(&actually, &expected, sizeof expected);
			}
		}

		TEST("Case 2. Index out of bound")
		{
			/// Case 2. Index out of bound
			///
			/// idx: [sv.length; ]
			/// len: does not matter
			///
			/// Returns empty string

			const size_t idx      = 10;
			struct lc_sv actually = lc_sv_sub(test_data, 6, idx);
			struct lc_sv expected = sv(0);

			assert(test_data.length <= idx);
			assert(test_data.length == 10);
			assert_memcmp(&actually, &expected, sizeof expected);
		}


		TEST("Case 3. Substring length truncate")
		{
			/// Case 3. Substring length truncate
			///
			/// idx: [0;   sv.length)
			/// len: (sv.length - idx; ]
			///
			/// Returns trucated substring

			const size_t idx            = 6;
			const size_t length         = 10;
			const struct lc_sv actually = lc_sv_sub(test_data, length, idx);
			const struct lc_sv expected = sv(.s = _test_data + idx, .length = 10 - idx);

			assert_memcmp(&actually, &expected, sizeof expected);
		}
	}

	TEST_GROUP(lc_sv_is_prefix)
	{
		TEST("String view prefix normal fail")
		{
			assert(false == lc_sv_is_prefix(test_data, sv_c("1234")));
		}
		TEST("String view prefix normal success")
		{
			assert(true == lc_sv_is_prefix(test_data, sv_c("01234")));
		}
		TEST("String view prefix too long")
		{
			assert(false == lc_sv_is_prefix(test_data, sv_c("0123467890")));
		}
		TEST("String view prefix empty")
		{
			assert(true == lc_sv_is_prefix(test_data, sv(0)));
		}
	}

	TEST_GROUP(lc_sv_is_suffix)
	{
		TEST("String view suffix normal fail")
		{
			assert(false == lc_sv_is_suffix(test_data, sv_c("67890")));
		}
		TEST("String view suffix normal success")
		{
			assert(true == lc_sv_is_suffix(test_data, sv_c("6789")));
		}
		TEST("String view suffix too long")
		{
			assert(false == lc_sv_is_suffix(test_data, sv_c("0123467890")));
		}
		TEST("String view suffix empty")
		{
			assert(true == lc_sv_is_suffix(test_data, sv(0)));
		}
	}

	TEST_GROUP(lc_sv_is_substring)
	{
		TEST("Normal")
		{
			assert(true == lc_sv_is_substring(test_data, sv_c("1234")));
		}

		TEST("Fail")
		{
			assert(false == lc_sv_is_substring(test_data, sv_c("21")));
		}

		TEST("Too long!")
		{
			assert(false == lc_sv_is_substring(test_data, sv_c("0123456789111")));
		}

		TEST("Empty RHS")
		{
			assert(true == lc_sv_is_substring(test_data, sv(0)));
		}

		TEST("Empty LHS")
		{
			assert(false == lc_sv_is_substring(sv(0), test_data));
		}
	}
}

static void
test_allocator(LC_ARGS_DECL, bool enabled)
{
	if (false == enabled) {
		lc_log_info(VT_BOLD "SKIPPED" VT_RESET " %s", __func__);
		return;
	}

	TEST_GROUP(lc_bump_allocator_)
	{

		// Some arbitrary memory allocation size
		enum
		{
			blob_len = 80
		};

		typedef size_t blob[blob_len];


		TEST("Init normal")
		{
			size_t memory_block[128]           = {0};
			size_t expected[LEN(memory_block)] = {
			        [0] = sizeof memory_block - sizeof(size_t[2]),
			        [1] = 0,
			};

			enum lc_allocator_error err   = LC_ALLOCATOR_ERROR_NONE;
			struct lc_allocator allocator = lc_bump_allocator(mut_slice_c(memory_block), &err);
			(void)allocator;


			assert(err == LC_ALLOCATOR_ERROR_NONE);
			assert_memcmp(memory_block, expected, sizeof expected);
		}

		TEST("Init fail")
		{
			size_t memory_block[1]             = {0};
			size_t expected[LEN(memory_block)] = {
			        [0] = 0,
			};

			enum lc_allocator_error err   = 0;
			struct lc_allocator allocator = lc_bump_allocator(mut_slice_c(memory_block), &err);
			(void)allocator;


			assert(err == LC_ALLOCATOR_ERROR_OUT_OF_MEMORY);
			assert_memcmp(memory_block, expected, sizeof expected);
		}

		TEST("Allocate once")
		{
			size_t memory_block[128]           = {0};
			size_t expected[LEN(memory_block)] = {
			        [0]                = sizeof memory_block - sizeof(size_t[2]),
			        [1]                = sizeof(blob),
			        [blob_len * 0 + 2] = 0x1337,
			};

			enum lc_allocator_error err   = 0;
			struct lc_allocator allocator = lc_bump_allocator(mut_slice_c(memory_block), &err);

			assert(err == LC_ALLOCATOR_ERROR_NONE);
			size_t *p = lc_mem_alloc_(allocator, &err, sizeof(blob));
			*p       = 0x1337;

			assert(err == LC_ALLOCATOR_ERROR_NONE);
			assert_memcmp(memory_block, expected, sizeof expected);
		}

		TEST("Allocate twice")
		{
			size_t memory_block[256]           = {0};
			size_t expected[LEN(memory_block)] = {
			        [0]                = sizeof memory_block - sizeof(size_t[2]),
			        [1]                = sizeof(blob) * 2,
			        [blob_len * 0 + 2] = 0x1337,
			        [blob_len * 1 + 2] = 0xdeadbeef,
			};

			enum lc_allocator_error err   = 0;
			struct lc_allocator allocator = lc_bump_allocator(mut_slice_c(memory_block), &err);

			assert(err == LC_ALLOCATOR_ERROR_NONE);
			size_t *p = lc_mem_alloc_(allocator, &err, sizeof(blob));
			*p       = 0x1337;

			assert(err == LC_ALLOCATOR_ERROR_NONE);

			p  = lc_mem_alloc_(allocator, &err, sizeof(blob));
			*p = 0xdeadbeef;

			assert(err == LC_ALLOCATOR_ERROR_NONE);
			assert_memcmp(memory_block, expected, sizeof expected);
		}
		TEST("Allocate reset")
		{
			size_t memory_block[256]           = {0};
			size_t expected[LEN(memory_block)] = {
			        [0] = sizeof memory_block - sizeof(size_t[2]),
			        [1] = 0,
			};

			enum lc_allocator_error err   = 0;
			struct lc_allocator allocator = lc_bump_allocator(mut_slice_c(memory_block), &err);

			assert(err == LC_ALLOCATOR_ERROR_NONE);
			(void)lc_mem_alloc_(allocator, &err, sizeof(blob));

			assert(err == LC_ALLOCATOR_ERROR_NONE);

			(void)lc_mem_alloc_(allocator, &err, sizeof(blob));

			assert(err == LC_ALLOCATOR_ERROR_NONE);

			lc_mem_free_all_(allocator, &err);

			assert(err == LC_ALLOCATOR_ERROR_NONE);
			assert_memcmp(memory_block, expected, sizeof expected);
		}
		TEST("Nested bump allocator")
		{
			size_t memory_block[256]           = {0};
			size_t expected[LEN(memory_block)] = {
			        [0]                = sizeof memory_block - sizeof(size_t[2]),
			        [1]                = sizeof(blob) * 2,
			        [blob_len * 1 + 2] = sizeof(blob) - sizeof(size_t[2]),
			};

			enum lc_allocator_error err   = LC_ALLOCATOR_ERROR_NONE;
			struct lc_allocator allocator = lc_bump_allocator(mut_slice_c(memory_block), &err);

			(void)lc_mem_alloc_(allocator, &err, sizeof(blob));

			struct lc_mut_bslice inner_slice =
			        mut_slice(sizeof(blob), lc_mem_alloc_(allocator, &err, sizeof(blob)));

			struct lc_allocator nested_allocator = lc_bump_allocator(inner_slice, &err);
			(void)nested_allocator;


			assert(err == LC_ALLOCATOR_ERROR_NONE);
			assert_memcmp(memory_block, expected, sizeof expected);
		}

		TEST("Free no-op")
		{
			size_t memory_block[256]           = {0};
			size_t expected[LEN(memory_block)] = {
			        [0]                = sizeof memory_block - sizeof(size_t[2]),
			        [1]                = sizeof(blob) * 2,
			        [blob_len * 0 + 2] = 0x1337,
			        [blob_len * 1 + 2] = 0xdeadbeef,
			};

			enum lc_allocator_error err   = 0;
			struct lc_allocator allocator = lc_bump_allocator(mut_slice_c(memory_block), &err);

			assert(LC_ALLOCATOR_ERROR_NONE == err);
			size_t *p = lc_mem_alloc_(allocator, &err, sizeof(blob));
			*p       = 0x1337;

			assert(LC_ALLOCATOR_ERROR_NONE == err);
			p  = lc_mem_alloc_(allocator, &err, sizeof(blob));
			*p = 0xdeadbeef;

			p = lc_mem_free_(allocator, &err, p);
			assert(NULL == p);
			p = lc_mem_free_(allocator, &err, p);

			assert(LC_ALLOCATOR_ERROR_NONE == err);
			assert_memcmp(memory_block, expected, sizeof expected);
		}

		TEST("Free sized first no-op")
		{
			size_t memory_block[256]           = {0};
			size_t expected[LEN(memory_block)] = {
			        [0]                = sizeof memory_block - sizeof(size_t[2]),
			        [1]                = sizeof(blob) * 2,
			        [blob_len * 0 + 2] = 0x1337,
			        [blob_len * 1 + 2] = 0xdeadbeef,
			};

			enum lc_allocator_error err   = 0;
			struct lc_allocator allocator = lc_bump_allocator(mut_slice_c(memory_block), &err);

			assert(LC_ALLOCATOR_ERROR_NONE == err);
			size_t *p = lc_mem_alloc_(allocator, &err, sizeof(blob));
			*p       = 0x1337;

			assert(LC_ALLOCATOR_ERROR_NONE == err);
			size_t *pp = lc_mem_alloc_(allocator, &err, sizeof(blob));
			*pp       = 0xdeadbeef;

			p = lc_mem_free_sized_(allocator, &err, p, sizeof(blob));
			assert(NULL == p);

			assert(LC_ALLOCATOR_ERROR_NONE == err);
			assert_memcmp(memory_block, expected, sizeof expected);
		}

		TEST("Free sized last success")
		{
			size_t memory_block[256]           = {0};
			size_t expected[LEN(memory_block)] = {
			        [0]                = sizeof memory_block - sizeof(size_t[2]),
			        [1]                = sizeof(blob) * 1,
			        [blob_len * 0 + 2] = 0x1337,
			        [blob_len * 1 + 2] = 0xdeadbeef,
			};

			enum lc_allocator_error err   = 0;
			struct lc_allocator allocator = lc_bump_allocator(mut_slice_c(memory_block), &err);

			assert(LC_ALLOCATOR_ERROR_NONE == err);
			size_t *p = lc_mem_alloc_(allocator, &err, sizeof(blob));
			*p       = 0x1337;

			assert(LC_ALLOCATOR_ERROR_NONE == err);
			size_t *pp = lc_mem_alloc_(allocator, &err, sizeof(blob));
			*pp       = 0xdeadbeef;

			pp = lc_mem_free_sized_(allocator, &err, pp, sizeof(blob));
			assert(NULL != p);
			assert(NULL == pp);

			assert(LC_ALLOCATOR_ERROR_NONE == err);
			assert_memcmp(memory_block, expected, sizeof expected);
		}
	}
}

int
main(int argc, const char *argv[argc])
{
	struct lc_context ctx = {
		.logger = lc_log_console_create(LC_LOG_LEVEL_DEFAULT, LC_LOG_ENABLE_DEFAULT),
	};

	test_string_view(&ctx, true);
	test_allocator(&ctx, lc_src_loc(), true);

	return 0;
}
