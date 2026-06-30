#include "lc.h"

typedef union {
	struct lc_clock clocks[1 * MiB / sizeof (struct lc_clock)];
	u8 bytes[1 * MiB];
} test_data;
static_assert_size(test_data, 1 * MiB);

extern int
main(int argc, const char *argv[argc])
{
	static test_data g_test_data;

	FILE *f_rng = fopen("/dev/random", "rb");
	FILE *f_dat = fopen(argv[1], "wb");

	fread(&g_test_data, sizeof g_test_data, 1, f_rng);

	// Ensure the test data is within the valid range
	for (size_t i = 0; i < LEN(g_test_data.clocks); i++) {
		g_test_data.clocks[i].sec  &= 0x7FFFFFFFFFFFFFFF;
		g_test_data.clocks[i].nsec &= 0x000000003FFFFFFF;
	}

	fwrite(&g_test_data, sizeof g_test_data, 1, f_dat);

	fclose(f_dat);
	fclose(f_rng);
	return 0;
}
