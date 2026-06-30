#define LC_IMPLEMENTATION
#define LC_fEATURE_LIST
#include "lc.h"

typedef union {
	struct lc_clock clocks[1 * MiB / sizeof (struct lc_clock)];
	u8 bytes[1 * MiB];
} test_data;
static_assert_size(test_data, 1 * MiB);

static const test_data g_lhs = {
	.bytes = {
#embed "lhs.rand.dat" limit(1 * MiB)
	},
};

static const test_data g_rhs = {
	.bytes = {
#embed "rhs.rand.dat" limit(1 * MiB)
	},
};

extern int
main(int argc, const char *argv[argc])
{

	static enum lc_order output[LEN(g_lhs.clocks)];

	struct lc_clock timer = lc_clock_init();
	for (int i = 0; i < 1024 * 16; i++) {
		for (size_t i = 0; i < LEN(g_lhs.clocks); i++) {
			output[i] = lc_clock_cmp(g_lhs.clocks[i], g_rhs.clocks[i]);
		}
	}


	f32 delta = lc_clock_since_ms(timer);
	printf("delta: %.2fms\n", delta);

	if (1 < argc) {
		const char *output_file = argv[1];
		FILE *f = fopen(output_file, "wb");
		fwrite(output, sizeof output, 1, f);
		fclose(f);
	}

	return 0;
}
