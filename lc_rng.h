#ifndef LC_RNG_H
#define LC_RNG_H

#ifndef LC_RNG_API
#define LC_RNG_API extern
#endif

#include <stdint.h>

typedef float  f32;
typedef double f64;

#ifndef LC_NODISCARD
#if __STDC_VERSION__ < 202000L && !defined(__cplusplus)
#if LC_CC_GNU
#define LC_NODISCARD __attribute__((__warn_unused_result__))
#else
#define LC_NODISCARD
#endif
#else
#define LC_NODISCARD [[nodiscard]]
#endif
#endif

#if defined(LC_CC_GNU) && LC_CC_GNU
#define LC_EXPECT(expr, cond) __builtin_expect(expr, cond)
#else
#define LC_EXPECT(expr, cond) expr
#endif


struct lc_rng_pcg32_state { uint64_t state; uint64_t inc; };

#define LC_RNG_PCG32_INITIALIZER                                                                   \
	((struct lc_rng_pcg32_state){                                                              \
	        .state = UINT64_C(0x853C49E6748FEA9B),                                             \
	        .inc   = UINT64_C(0xDA3E39CB94B95BDB),                                             \
	})


LC_NODISCARD LC_RNG_API uint32_t lc_pcg32_random(struct lc_rng_pcg32_state *rng);
LC_NODISCARD LC_RNG_API f32 lc_pcg32_random_f32(struct lc_rng_pcg32_state *rng);

#ifdef LC_IMPLEMENTATION

LC_NODISCARD
LC_RNG_API uint32_t
lc_pcg32_random(struct lc_rng_pcg32_state *rng)
{
	uint64_t old_state = rng->state;
	// Advance internal state
	rng->state = old_state * UINT64_C(6364136223846793005) + (rng->inc|1);
	// Calculate output function (XSH RR), uses old state for max ILP
	uint32_t xor_shifted = ((old_state >> UINT32_C(18)) ^ old_state) >> UINT32_C(27);
	uint32_t rot = old_state >> UINT32_C(59);
	return (xor_shifted >> rot) | (xor_shifted << ((-rot) & 31));
}

LC_NODISCARD
LC_RNG_API f32
lc_pcg32_random_f32(struct lc_rng_pcg32_state *rng)
{
	uint32_t proto_exp_offset = lc_pcg32_random(rng);

	if (LC_EXPECT(0 == proto_exp_offset, false))
		return 0;

	extern f32 ldexpf(f32 arg, int exp);
	return ldexpf((f32)(lc_pcg32_random(rng) | 0x80000001), -32 - __builtin_clz(proto_exp_offset));
}

#endif

#endif
