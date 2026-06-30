#ifndef LC_INCLUDE_H
#define LC_INCLUDE_H
// vim: set ft=c:

/// Table of content
///
/// @lc_toc_begin
/// - Preprocessor ..................................................8. [LC_XPP]
/// - Feature detection ...........................................313. [LC_XFD]
/// - Types .......................................................100. [LC_XTY]
/// - Platform code ................................................14. [LC_XPL]
/// - Utility macro ................................................95. [LC_XUM]
/// - Math .........................................................34. [LC_XMT]
/// - Relative pointer .............................................48. [LC_XRP]
/// - Ordering ....................................................143. [LC_XOD]
/// - Assert .......................................................71. [LC_XAS]
/// - Slice .......................................................108. [LC_XSC]
/// - Source location ..............................................40. [LC_XSL]
/// - Allocator ...................................................449. [LC_XAC]
/// - ANSI Escape Code .............................................54. [LC_XAE]
/// - Time ........................................................191. [LC_XCH]
/// - String view .................................................272. [LC_XSV]
/// - Logger ......................................................348. [LC_XLG]
/// - Context ......................................................28. [LC_XCT]
/// - Input/Output ................................................274. [LC_XIO]
/// - Input/Output implementation ................................1068. [LC_XIH]
/// - Vector .......................................................77. [LC_XVC]
/// - Strings .....................................................120. [LC_XSS]
/// - Path ........................................................188. [LC_XPH]
/// - Dynamic library ..............................................34. [LC_XDL]
/// - Bit stream ..................................................236. [LC_XBS]
/// - Endian .......................................................91. [LC_XED]
/// - Compression ..................................................11. [LC_XCM]
/// - Base64 .......................................................78. [LC_XBC]
/// @lc_toc_end


////////////////////////////////////////////////////////////////////////////////
/// Preprocessor                                                    [LC_XPP] ///
////////////////////////////////////////////////////////////////////////////////

#define LC_PP_STR(x)         #x
#define LC_PP_CONCAT(x, y)   x##y
#define LC_PP_DEFER(PP, ...) PP(__VA_ARGS__)

////////////////////////////////////////////////////////////////////////////////
/// Feature detection                                               [LC_XFD] ///
////////////////////////////////////////////////////////////////////////////////

#ifdef __AVX__
#ifdef LC_ENABLE_AVX
#include <immintrin.h>
#define LC_CPU_AVX
#endif
#endif

// TODO: Detect architecture
#if __amd64__
#define LC_ARCH_AMD64
#endif

#ifdef __cplusplus
#define LC_STD_CXX
#endif

#define LC_STDC_C95 199409L
#define LC_STDC_C99 199901L
#define LC_STDC_C11 201112L
#define LC_STDC_C17 201710L
#if __clang_major__ <= 17 // C23 on clang 17
#define LC_STDC_C23 202000L
#else
#define LC_STDC_C23 202311L
#endif

#if defined(__GNUC__)
#define LC_CC_GCC 1
#else
#define LC_CC_GCC 0
#endif
#if defined(__clang__)
#define LC_CC_LLVM 1
#else
#define LC_CC_LLVM 0
#endif

#define LC_CC_GNU (LC_CC_GCC | LC_CC_LLVM)

#ifndef LC_STD_CXX
#if __STDC_VERSION__ < LC_STDC_C11
#if LC_CC_GNU
#define alignas _Alignas
#define alignof __alignof__
#else
#define alignas(x)
#define alignof(x)
#endif
#elif __STDC_VERSION__ < LC_STDC_C23
#define alignas _Alignas
#define alignof _Alignof
#endif
#endif

#if __STDC_VERSION__ < LC_STDC_C23
#include <stdbool.h>
#endif

#if LC_CC_GNU
#define LC_PACKED __attribute__((__packed__))
#define LC_HAVE_PACKED
#endif

#if __STDC_VERSION__ >= LC_STDC_C23
#define LC_HAVE_TYPEOF
#elif LC_CC_GNU
#define typeof __typeof__
#define LC_HAVE_TYPEOF
#endif


#ifndef LC_STD_CXX
#if __STDC_VERSION__ < LC_STDC_C11
#if LC_CC_GNU
#define static_assert _Static_assert
#else
#define static_assert(...) //
#endif
#elif __STDC_VERSION__ < LC_STDC_C23
#define static_assert _Static_assert
#endif
#endif

#ifdef LC_STD_CXX
#define LC_DEPRECATED(msg) [[deprecated(msg)]]
#else
#if __STDC_VERSION__ < LC_STDC_C23
#if LC_CC_GNU
#define LC_DEPRECATED(msg) __attribute__((deprecated(msg)))
#else
#define LC_DEPRECATED(msg)
#endif
#else
#define LC_DEPRECATED(msg) [[deprecated(msg)]]
#endif
#endif

#ifdef LC_STD_CXX
#define LC_FALLTHROUGH [[fallthrough]]
#else
#if __STDC_VERSION__ < LC_STDC_C23
#if LC_CC_GNU
#define LC_FALLTHROUGH __attribute__((fallthrough))
#else
#define LC_FALLTHROUGH
#endif
#else
#define LC_FALLTHROUGH [[fallthrough]]
#endif
#endif

#ifdef LC_STD_CXX
#define restrict
#endif

#if LC_CC_GNU
#define LC_NORETURN __attribute__((noreturn))
#else
#define LC_NORETURN
#endif

#if LC_CC_GNU
#define LC_EXPECT(expr, cond) __builtin_expect(expr, cond)
#else
#define LC_EXPECT(expr, cond) expr
#endif

#if __STDC_HOSTED__

#ifdef __unix__
#define LC_PLATFORM_UNIX
#endif
#ifdef _DEFAULT_SOURCE
#define LC_PLATFORM_POSIX
#endif
#ifdef _GNU_SOURCE
#define LC_PLATFORM_POSIX
#endif
#ifdef _POSIX_C_SOURCE
#define LC_PLATFORM_POSIX
#endif
#ifdef __linux__
#define LC_PLATFORM_LINUX
#endif
#ifdef __APPLE__
#define LC_PLATFORM_APPLE
#endif
#ifdef _WIN32
#define LC_PLATFORM_WIN
#endif
#ifdef __ANDROID__
#define LC_PLATFORM_ANDROID
#endif


#else
#define LC_PLATFORM_FREESTANDING
#endif

#ifndef LC_FEATURE_LIST
#define LC_ENABLE_ANSI_ESC
#define LC_ENABLE_DLOPEN
#define LC_ENABLE_IO
#endif

#ifndef LC_STD_CXX
#if __STDC_VERSION__ < LC_STDC_C11
#if LC_CC_GNU
#define thread_local __thread
#else
#define thread_local
#endif
#elif __STDC_VERSION__ < LC_STDC_C23
#define thread_local _Thread_local
#else
#endif
#endif

#if __STDC_VERSION__ < LC_STDC_C23
#define nullptr NULL
#endif


#if __STDC_VERSION__ < LC_STDC_C23 && !defined(LC_STD_CXX)
#define LC_NODISCARD
#else
#define LC_NODISCARD [[nodiscard]]
#endif

#if __STDC_VERSION__ < LC_STDC_C23 && !defined(LC_STD_CXX)
#if LC_CC_GNU
#define LC_UNUSED __attribute__((__unused__))
#else
#define LC_UNUSED
#endif
#else
#define LC_UNUSED [[maybe_unused]]
#endif

#ifdef __FAST_MATH__
#define LC_FAST_MATH
#endif


#if LC_CC_GCC
#define LC_PRINTF_LIKE(fmt_arg_pos, arg_pos) __attribute__((__format__(printf, fmt_arg_pos, arg_pos)))
#else
#define LC_PRINTF_LIKE(fmt_arg_pos, arg_pos)
#endif

#if LC_CC_GNU
#define LC_FILE_NAME __BASE_FILE__
#else
#define LC_FILE_NAME __FILE__
#endif

#if LC_CC_GNU
#define LC_ALWAYS_INLINE inline __attribute__((__always_inline__))
#else
#define LC_ALWAYS_INLINE inline
#endif

// TODO: Clean this up when I get a newer version of GCC/clang
// GCC has backported unsequenced to older version of the language, so let's
// wrap arround that instead of using `gnu::pure` and `gnu::const`.
#if LC_STDC_C23 <= __STDC_VERSION__
#if __has_c_attribute(reproducible)
#define LC_REPRODUCIBLE [[reproducible]]
#else
#define LC_REPRODUCIBLE
#endif
#if __has_c_attribute(unsequenced)
#define LC_UNSEQUENCED [[unsequenced]]
#elif LC_CC_GNU
#define LC_UNSEQUENCED LC_REPRODUCIBLE
#else
#define LC_UNSEQUENCED LC_REPRODUCIBLE
#endif
#if LC_CC_GNU
#define LC_CONST [[gnu::const]]
#else
#define LC_CONST LC_UNSEQUENCED
#endif
#elif LC_CC_GNU
#if __has_c_attribute(unsequenced)
#define LC_UNSEQUENCED __attribute__((unsequenced))
#else
#define LC_UNSEQUENCED
#endif
#define LC_CONST       __attribute__((const))
#define LC_REPRODUCIBLE
#else
#define LC_UNSEQUENCED
#define LC_REPRODUCIBLE
#define LC_CONST
#endif

// Because floating point operations can affect floating point enviornment,
// unsequenced is unsuitable for standard compliant code.
// When fast math optimazations is enabled, we can ignore the problem.
#ifdef LC_FAST_MATH
#define LC_UNSEQUENCED_FP LC_UNSEQUENCED
#else
#define LC_UNSEQUENCED_FP
#endif

#ifdef LC_HAVE_TYPEOF
#define LC_UNSEQUENCED_FUNC(func)    typeof (func) LC_UNSEQUENCED    func
#define LC_UNSEQUENCED_FP_FUNC(func) typeof (func) LC_UNSEQUENCED_FP func
#else
#define LC_UNSEQUENCED_FUNC(func)    static_assert(1)
#define LC_UNSEQUENCED_FP_FUNC(func) static_assert(1)
#endif

#define LC_PRAGMA(...) _Pragma(LC_PP_STR(__VA_ARGS__))

#if LC_CC_GCC
#define LC_WARN_PUSH         _Pragma("GCC diagnostic push")
#define LC_WARN_POP          _Pragma("GCC diagnostic pop")
#define LC_WARN_IGNORE(warn) LC_PRAGMA(GCC diagnostic ignored warn)
#else
#define LC_WARN_PUSH
#define LC_WARN_POP
#define LC_WARN_IGNORE(warn)
#endif

#if LC_CC_GCC
#define LC_FLAG_ENUM __attribute__((flag_enum))
#else
#define LC_FLAG_ENUM
#endif

#if LC_CC_GCC
#define LC_DESIGNATED_INIT __attribute__((designated_init))
#else
#define LC_DESIGNATED_INIT
#endif

#ifdef LC_STD_CXX
LC_WARN_PUSH
LC_WARN_IGNORE("-Wc99-extensions")
LC_WARN_IGNORE("-Wvla-cxx-extension")
LC_WARN_IGNORE("-Wmissing-field-initializers")
#endif

#ifdef LC_STD_CXX
extern "C" {
#endif

////////////////////////////////////////////////////////////////////////////////
/// Types                                                           [LC_XTY] ///
////////////////////////////////////////////////////////////////////////////////

#include <inttypes.h>
#include <limits.h>
#include <stddef.h>
#include <stdint.h>

static_assert(CHAR_BIT == 8, "We assume that a byte is 8 bits");

typedef unsigned long long ullong;
typedef unsigned long ulong;
typedef unsigned int uint;
typedef unsigned short ushort;
typedef unsigned char uchar;

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t  i8,  s8;
typedef int16_t i16, s16;
typedef int32_t i32, s32;
typedef int64_t i64, s64;

typedef u64 size64;
typedef u32 size32;
typedef u16 size16;
typedef u8  size8;

typedef i64 isize64;
typedef i32 isize32;
typedef i16 isize16;
typedef i8  isize8;

typedef volatile u8  vu8;
typedef volatile u16 vu16;
typedef volatile u32 vu32;
typedef volatile u64 vu64;

typedef volatile i8  vi8,  vs8;
typedef volatile i16 vi16, vs16;
typedef volatile i32 vi32, vs32;
typedef volatile i64 vi64, vs64;

LC_DEPRECATED("ABI unstable") typedef intmax_t  intmax_t;
LC_DEPRECATED("ABI unstable") typedef uintmax_t uintmax_t;

typedef float  f32;
typedef double f64;

typedef volatile f32 vf32;
typedef volatile f64 vf64;

static_assert(sizeof(f32) == 4, " float should match IEEE-754 binary32 format");
static_assert(sizeof(f64) == 8, "double should match IEEE-754 binary64 format");

typedef ptrdiff_t ssize;


#ifndef SSIZE_MAX
#define SSIZE_MAX (ssize)(SIZE_MAX >> 1)
#endif
#define SSIZE_MIN ~(SSIZE_MAX)

// Constant literal macro
#define u8_c(x)  UINT8_C(x)
#define u16_c(x) UINT16_C(x)
#define u32_c(x) UINT32_C(x)
#define u64_c(x) UINT64_C(x)

#define i8_c(x)  INT8_C(x)
#define i16_c(x) INT16_C(x)
#define i32_c(x) INT32_C(x)
#define i64_c(x) INT64_C(x)

#define s8_c(x)  INT8_C(x)
#define s16_c(x) INT16_C(x)
#define s32_c(x) INT32_C(x)
#define s64_c(x) INT64_C(x)

#define f32_c(x) x##f
#define f64_c(x) x


#if __STDC_VERSION__ < LC_STDC_C23

#define INT16_WIDTH  16
#define INT32_WIDTH  32
#define INT64_WIDTH  64
#define INT8_WIDTH   8
#define UINT16_WIDTH 16
#define UINT32_WIDTH 32
#define UINT64_WIDTH 64
#define UINT8_WIDTH  8

#endif

////////////////////////////////////////////////////////////////////////////////
/// Platform code                                                   [LC_XPL] ///
////////////////////////////////////////////////////////////////////////////////

#ifndef lc_memset
#include <string.h>
#define lc_memset memset
#endif

#ifndef lc_memcpy
#include <string.h>
#define lc_memcpy memcpy
#endif

////////////////////////////////////////////////////////////////////////////////
/// Utility macro                                                   [LC_XUM] ///
////////////////////////////////////////////////////////////////////////////////

#ifndef LEN
#define LEN(X) (sizeof(X) / sizeof(X)[0])
#endif

#ifdef LC_HAVE_TYPEOF
#define typeof_member(type, member) typeof(((type *)0)->member)
#endif
#define sizeof_member(type, member) (sizeof(((type *)0)->member))

#define struct_size_t(type, member, count)                                                                   \
	(offsetof(type, member) + (sizeof_member(type, member[0]) * (count)))
#ifdef LC_HAVE_TYPEOF
#define struct_size(ptr, member, count) struct_size_t(typeof(*ptr), member, count)
#endif


#define KiB (UINTMAX_C(0x01) << 10)
#define MiB (UINTMAX_C(0x01) << 20)
#define GiB (UINTMAX_C(0x01) << 30)
#define TiB (UINTMAX_C(0x01) << 40)
#define PiB (UINTMAX_C(0x01) << 50)
#define EiB (UINTMAX_C(0x01) << 60)


#if LC_CC_GCC
#define LC_ELVIS(X, Y) __extension__((X) ?: (Y))
#else
#define LC_ELVIS(X, Y) ((X) ? (X) : (Y))
#endif


#if LC_STDC_C11 <= __STDC_VERSION__
#define type_equal(expr, type) _Generic(expr, type: true, default: false)
#else
#define type_equal(expr, type) true
#endif

#define type_assert(expr, type) static_assert(type_equal(expr, type), #expr " == " #type);

#if LC_STDC_C11 <= __STDC_VERSION__
#define type_check(expr, type) _Generic(expr, type: expr)
#else
#define type_check(expr, type) expr
#endif


#define container_of(type, member, ptr)                                                                      \
	((type *)((u8 *)(ptr) - offsetof(type, member)))

#define container_of_c(type, member, ptr)                                                                    \
	((type *)(((u8 *)type_check(ptr, typeof_member(type, member) *)) - offsetof(type, member)))


#if LC_CC_LLVM && __has_builtin(__builtin_debugtrap)
#define LC_DEBUGBREAK() __builtin_debugtrap()
#elif LC_CC_GCC
#ifdef LC_ARCH_AMD64
#define LC_DEBUGBREAK() __asm__ volatile("int3")
#else
#define LC_DEBUGBREAK() __builtin_trap()
#endif
#else
#define LC_DEBUGBREAK()
#endif


#if __STDC_VERSION__ < LC_STDC_C23
#if LC_CC_GNU
#define lc_unreachable __builtin_unreachable
#else
#define lc_unreachable() *((void *volatile *volatile)NULL)
#endif
#else
#define lc_unreachable unreachable
#endif

#define lc_unimplemented(msg) LC_DEBUGBREAK()
#define lc_todo(msg)          LC_DEBUGBREAK()


#define lc_assume(cond)                                                                                      \
	do {                                                                                                 \
		if (!(cond))                                                                                 \
			lc_unreachable();                                                                    \
	} while (false)


#define static_assert_size(type, size)                                                                       \
	static_assert(sizeof (type) == (size), "Unintended size of type " #type ", expected: " #size)


////////////////////////////////////////////////////////////////////////////////
/// Math                                                            [LC_XMT] ///
////////////////////////////////////////////////////////////////////////////////

#define MAX(X, Y)           ((X) < (Y) ? (Y) : (X))
#define MIN(X, Y)           ((X) < (Y) ? (X) : (Y))
#define IN_RANGE(X, LO, HI) (((LO) <= (X)) & ((X) <= (HI)))

#ifdef LC_HAVE_TYPEOF
#define ABS(X) (typeof(X))llabs(X)
#else
#define ABS(X) llabs(X)
#endif

#ifndef LC_STD_CXX
LC_NODISCARD
static inline f32
lerp(f32 a, f32 b, f32 t)
{
	return a + t * (b - a);
}

LC_UNSEQUENCED_FP_FUNC(lerp);
#endif

LC_NODISCARD
static inline f32
clamp(f32 x, f32 lo, f32 hi)
{
	return x < lo ? lo : (hi < x ? hi : x);
}

LC_UNSEQUENCED_FP_FUNC(clamp);

////////////////////////////////////////////////////////////////////////////////
/// Relative pointer                                                [LC_XRP] ///
////////////////////////////////////////////////////////////////////////////////

#if !defined(LC_STD_CXX) && defined(LC_PACKED) && defined(LC_HAVE_TYPEOF)


#define rel_ptr(type, rel_type)                                                                              \
	struct LC_PACKED /* Anonymous tag */ {                                                               \
		/* Relative offset to the thing pointing to */                                               \
		rel_type _offset;                                                                            \
		/* zero-sized field to carry type infomation of the thing pointing to */                     \
		__extension__ type _[0];                                                                     \
	}


#define rel_is_null(p)        (0 == (p)._offset)
#define rel_to_abs(p)         (typeof((p)._[0]))((intptr_t) & (p) + (p)._offset)
#define abs_to_rel(base, abs) ((typeof(base)){._offset = ((intptr_t)(abs) - (intptr_t) & (base))})

static_assert(sizeof(rel_ptr(const char *, i32)) == sizeof(i32), "Ensure the struct is packed");
static_assert(sizeof(rel_ptr(const void *, i8)) == sizeof(i8), "Ensure the struct is packed");

#else


#define rel_ptr(type, rel_type) rel_type

#define rel_is_null(p)        (0 == (p))
#define rel_to_abs(p)         ((intptr_t) & (p) + (p))
#define abs_to_rel(base, abs) ((intptr_t)(abs) - (intptr_t) & (base))


#endif

#define rel_ptr_i8(type) rel_ptr(type, i8)
#define rel_ptr_u8(type) rel_ptr(type, u8)

#define rel_ptr_i16(type) rel_ptr(type, i16)
#define rel_ptr_u16(type) rel_ptr(type, u16)

#define rel_ptr_i32(type) rel_ptr(type, i32)
#define rel_ptr_u32(type) rel_ptr(type, u32)

#define rel_ptr_i64(type) rel_ptr(type, i64)
#define rel_ptr_u64(type) rel_ptr(type, u64)


////////////////////////////////////////////////////////////////////////////////
/// Ordering                                                        [LC_XOD] ///
////////////////////////////////////////////////////////////////////////////////

// Does not make sense in cxx...
#ifndef LC_STD_CXX
#define LC_HAVE_ORDER

/**
 * @breif A strict total order.
 *
 * Used as a result of a three-way comparison.
 *
 * This enum is compatible with existing C API that use comparison agains zero
 * (eg. qsort). Any existing C API that returns an integer can be converted to
 * this enum by using \p lc_order_from_int.
 *
 * A strict total order satisfy the following for all \a a, \a b and \a c
 *
 *   -# a < a == false (irreflextive).
 *   -# if a < b == true then b < a == false (asymmetric).
 *   -# if a < b and b < c then a < c (transitive).
 *   -# if a != b then a < b or b < a (connected).
 *
 */
enum LC_NODISCARD lc_order
{
	LC_ORDER_LESS    = -1,
	LC_ORDER_EQUAL   = +0,
	LC_ORDER_GREATER = +1,
};

static inline enum lc_order lc_order_cmp_size_t(size_t lhs, size_t rhs);
static inline enum lc_order lc_order_cmp_u32(u32 lhs, u32 rhs);
static inline enum lc_order lc_order_cmp_i32(i32 lhs, i32 rhs);

static inline enum lc_order lc_order_from_i64(i64 order);
static inline enum lc_order lc_order_from_i32(i32 order);
static inline enum lc_order lc_order_from_i32(i32 order);
static inline enum lc_order lc_order_from_i16(i16 order);
static inline enum lc_order lc_order_from_i8(i8 order);

LC_UNSEQUENCED_FUNC(lc_order_cmp_size_t);
LC_UNSEQUENCED_FUNC(lc_order_cmp_u32);
LC_UNSEQUENCED_FUNC(lc_order_cmp_i32);
LC_UNSEQUENCED_FUNC(lc_order_from_i64);
LC_UNSEQUENCED_FUNC(lc_order_from_i32);
LC_UNSEQUENCED_FUNC(lc_order_from_i16);
LC_UNSEQUENCED_FUNC(lc_order_from_i8);

static inline enum lc_order
lc_order_cmp_size_t(size_t lhs, size_t rhs)
{
	return (lhs > rhs) - (lhs < rhs);
}

static inline enum lc_order
lc_order_cmp_u32(u32 lhs, u32 rhs)
{
	return (lhs > rhs) - (lhs < rhs);
}

static inline enum lc_order
lc_order_cmp_i32(i32 lhs, i32 rhs)
{
	return (lhs > rhs) - (lhs < rhs);
}

/**
 * @breif Converts an integer representing an order to an enumeration
 * representing the order.
 *
 * @param order The integer to convert.
 *
 * @returns LC_ORDER_LESS if \a order is below zero,
 *          LC_ORDER_EQUAL if \a order is zero,
 *          LC_ORDER_GREATER if \a order is above zero.
 */
static inline enum lc_order
lc_order_from_i64(i64 order)
{
	return (order > 0) - (order < 0);
}

/**
 * @breif Converts an integer representing an order to an enumeration
 * representing the order.
 *
 * @param order The integer to convert.
 *
 * @returns LC_ORDER_LESS if \a order is below zero,
 *          LC_ORDER_EQUAL if \a order is zero,
 *          LC_ORDER_GREATER if \a order is above zero.
 */
static inline enum lc_order
lc_order_from_i32(i32 order)
{
	return (order > 0) - (order < 0);
}

/**
 * @breif Converts an integer representing an order to an enumeration
 * representing the order.
 *
 * @param order The integer to convert.
 *
 * @returns LC_ORDER_LESS if \a order is below zero,
 *          LC_ORDER_EQUAL if \a order is zero,
 *          LC_ORDER_GREATER if \a order is above zero.
 */
static inline enum lc_order
lc_order_from_i16(i16 order)
{
	return (order > 0) - (order < 0);
}

/**
 * @breif Converts an integer representing an order to an enumeration
 * representing the order.
 *
 * @param order The integer to convert.
 *
 * @returns LC_ORDER_LESS if \a order is below zero,
 *          LC_ORDER_EQUAL if \a order is zero,
 *          LC_ORDER_GREATER if \a order is above zero.
 */
static inline enum lc_order
lc_order_from_i8(i8 order)
{
	return (order > 0) - (order < 0);
}

#if __STDC_VERSION__ < LC_STDC_C11
#define lc_order_from_int lc_order_from_i32
#else
#define _lc__oo(type) type: lc_order_from_##type

#define lc_order_from_int(order) _Generic(order, _lc__oo(i8), _lc__oo(i16), _lc__oo(i32), _lc__oo(i64))(order)

#endif
#endif


////////////////////////////////////////////////////////////////////////////////
/// Assert                                                          [LC_XAS] ///
////////////////////////////////////////////////////////////////////////////////

#define LC_ASSERT_(expr, msg)                                                                                \
	do {                                                                                                 \
		extern int puts(const char *);                                                               \
                                                                                                             \
		if (LC_EXPECT(!(expr), false)) {                                                             \
			puts(msg);                                                                           \
			LC_DEBUGBREAK();                                                                     \
			lc_unreachable();                                                                    \
		}                                                                                            \
	} while (false);

#ifdef LC_STD_CXX
#define LC_ASSERT__(expr, msg, src_loc)                                                                      \
	do {                                                                                                 \
		if (LC_EXPECT(!(expr), false)) {                                                             \
			_lc_log(ctx->logger,                                                                 \
			        LC_LOG_LEVEL_FATAL,                                                          \
			        src_loc,                                                                     \
			        "Assert failed on expression: %s message: %s",                               \
			        #expr,                                                                       \
			        msg);                                                                        \
                                                                                                             \
			/* We HAVE to save src_loc into a local variable to be compatible with C++ */        \
			size_t i = 0;                                                                        \
			for (const struct lc_source_loc s = src_loc, *src = &s; src->prev; src = src->prev)  \
				_lc_log(ctx->logger,                                                         \
				        LC_LOG_LEVEL_FATAL,                                                  \
				        src_loc,                                                             \
				        "%02zu: %s:%d %s()",                                                 \
				        ++i,                                                                 \
				        src->filename,                                                       \
				        src->line,                                                           \
				        src->func);                                                          \
			LC_DEBUGBREAK();                                                                     \
			lc_unreachable();                                                                    \
		}                                                                                            \
	} while (false)
#else
#define LC_ASSERT__(expr, msg, src_loc)                                                                      \
	do {                                                                                                 \
		if (LC_EXPECT(!(expr), false)) {                                                             \
			_lc_log(ctx->logger,                                                                 \
			        LC_LOG_LEVEL_FATAL,                                                          \
			        src_loc,                                                                     \
			        "Assert failed on expression: %s message: %s",                               \
			        #expr,                                                                       \
			        msg);                                                                        \
                                                                                                             \
			/* We HAVE to save src_loc into a local variable to be compatible with C++ */        \
			size_t i = 0;                                                                        \
			for (const struct lc_source_loc s = src_loc, *src = &s; src->prev; src = src->prev)  \
				_lc_log(ctx->logger,                                                         \
				        LC_LOG_LEVEL_FATAL,                                                  \
				        src_loc,                                                             \
				        "%02zu: %s:%d %s()",                                                 \
				        ++i,                                                                 \
				        src->data->filename,                                                 \
				        src->data->line,                                                     \
				        src->data->func);                                                    \
			LC_DEBUGBREAK();                                                                     \
			lc_unreachable();                                                                    \
		}                                                                                            \
	} while (false)
#endif

#define LC_ASSERT(expr, msg) LC_ASSERT__(expr, msg, lc_src_loc(&src_loc))

////////////////////////////////////////////////////////////////////////////////
/// Slice                                                           [LC_XSC] ///
////////////////////////////////////////////////////////////////////////////////

typedef u8 lc_byte;

// Const byte slice
struct lc_bslice {
	size_t size;
	const lc_byte *data;
};

// Mutable byte slice
struct lc_mut_bslice {
	size_t size;
	lc_byte *data;
};

#define slice(n, ptr)     ((struct lc_bslice){.size = n, .data = (const lc_byte *)ptr})
#define mut_slice(n, ptr) ((struct lc_mut_bslice){.size = n, .data = (lc_byte *)ptr})

#define slice_c(arr)     slice(sizeof arr, arr)
#define mut_slice_c(arr) mut_slice(sizeof arr, arr)

static inline struct lc_bslice
lc_bslice_from_mut(struct lc_mut_bslice mut_slice)
{
	return slice(mut_slice.size, mut_slice.data);
}

static inline struct lc_bslice
lc_bslice_subslice(struct lc_bslice slice, size_t idx, size_t size)
{
	/// Case 4. Length zero
	///
	/// idx: does not matter
	/// size: 0
	///
	/// Returns empty string
	if (0 == size)
		return slice(0, 0);

	/// Case 2. Index out of bound
	///
	/// idx: (sv.length; ]
	/// size: does not matter
	///
	/// Returns empty string
	if (slice.size < idx)
		return slice(0, 0);

	/// Case 3. Substring length truncate
	///
	/// idx: [0;   sv.length]
	/// size: (sv.length - idx; ]
	///
	/// Trucate substring
	if (slice.size < idx + size)
		size -= (idx + size) - slice.size;

	/// Case 1. Normal
	///
	/// idx: [0;   sv.length]
	/// size: [0; sv.length - idx]
	///
	/// Returns substring as expected
	return slice(size, slice.data + idx);
}

static inline struct lc_mut_bslice
lc_bslice_mut_subslice(struct lc_mut_bslice slice, size_t idx, size_t size)
{
	/// Case 4. Length zero
	///
	/// idx: does not matter
	/// size: 0
	///
	/// Returns empty string
	if (0 == size)
		return mut_slice(0, 0);

	/// Case 2. Index out of bound
	///
	/// idx: (sv.length; ]
	/// size: does not matter
	///
	/// Returns empty string
	if (slice.size < idx)
		return mut_slice(0, 0);

	/// Case 3. Substring length truncate
	///
	/// idx: [0;   sv.length]
	/// size: (sv.length - idx; ]
	///
	/// Trucate substring
	if (slice.size < idx + size)
		size -= (idx + size) - slice.size;

	/// Case 1. Normal
	///
	/// idx: [0;   sv.length]
	/// size: [0; sv.length - idx]
	///
	/// Returns substring as expected
	return mut_slice(size, slice.data + idx);
}

////////////////////////////////////////////////////////////////////////////////
/// Source location                                                 [LC_XSL] ///
////////////////////////////////////////////////////////////////////////////////

#ifdef LC_STD_CXX

struct lc_source_loc {
	const char *restrict filename;
	const char *restrict func;
	int line;
	const struct lc_source_loc *prev;
};

#define lc_src_loc(prev)                                                                                     \
	LC_WARN_PUSH                                                                                         \
	LC_WARN_IGNORE("-Wc99-extensions")                                                                   \
	((struct lc_source_loc){LC_FILE_NAME, __func__, __LINE__, prev}) LC_WARN_POP

#else

struct lc_source_loc_data {
	const char *restrict filename;
	const char *restrict func;
	int line;
};

struct lc_source_loc {
	const struct lc_source_loc_data *data;
	const struct lc_source_loc *prev;
};

#define lc_src_loc(prev)                                                                                     \
	((struct lc_source_loc){                                                                             \
	 .data = &(struct lc_source_loc_data){.filename = LC_FILE_NAME, .func = __func__, .line = __LINE__},                \
	 prev                                                                                                \
	 })
#endif



////////////////////////////////////////////////////////////////////////////////
/// Allocator                                                       [LC_XAC] ///
////////////////////////////////////////////////////////////////////////////////

#include <stdarg.h>
#include <string.h>

enum lc_allocator_cmd
{
	LC_ALLOCATOR_INIT,
	LC_ALLOCATOR_ALLOCATE,
	LC_ALLOCATOR_RESIZE,
	LC_ALLOCATOR_RESIZE_SIZED,
	LC_ALLOCATOR_FREE,
	LC_ALLOCATOR_FREE_SIZED,
	LC_ALLOCATOR_FREE_ALL,
};

enum lc_allocator_error
{
	LC_ALLOCATOR_ERROR_NONE,
	LC_ALLOCATOR_ERROR_OUT_OF_MEMORY,
	LC_ALLOCATOR_ERROR_INVALID_ARG,
	LC_ALLOCATOR_ERROR_UNKNOWN,
	LC_ALLOCATOR_ERROR_NOT_IMPLEMENTED,
};

#include <stdlib.h>

struct lc_allocator {
	void *(*proc)(
	        void *restrict data,
	        const struct lc_source_loc *restrict src_loc,
	        enum lc_allocator_error *restrict err,
	        enum lc_allocator_cmd cmd,
	        ...
	);
	void *data;
};

LC_NODISCARD
static inline void *
lc_mem_alloc__(
        struct lc_source_loc src_loc,
        struct lc_allocator allocator,
        enum lc_allocator_error *restrict err,
        size_t size
)
{
	LC_ASSERT_(allocator.proc, "");
	return allocator.proc(allocator.data, &src_loc, err, LC_ALLOCATOR_ALLOCATE, size);
}

LC_NODISCARD
static inline void *
lc_mem_alloc_zero__(
        struct lc_source_loc src_loc,
        struct lc_allocator allocator,
        enum lc_allocator_error *restrict err,
        size_t size
)
{
	LC_ASSERT_(allocator.proc, "");
	return lc_memset(allocator.proc(allocator.data, &src_loc, err, LC_ALLOCATOR_ALLOCATE, size), 0, size);
}

static inline void *
lc_mem_free__(
        struct lc_source_loc src_loc,
        struct lc_allocator allocator,
        enum lc_allocator_error *restrict err,
        void *restrict ptr
)
{
	LC_ASSERT_(allocator.proc, "");
	return allocator.proc(allocator.data, &src_loc, err, LC_ALLOCATOR_FREE, ptr);
}

static inline void *
lc_mem_free_sized__(
        struct lc_source_loc src_loc,
        struct lc_allocator allocator,
        enum lc_allocator_error *restrict err,
        void *restrict ptr,
        size_t size
)
{
	LC_ASSERT_(allocator.proc, "");
	void *ret = allocator.proc(allocator.data, &src_loc, err, LC_ALLOCATOR_FREE_SIZED, size, ptr);

	if (LC_ALLOCATOR_ERROR_NOT_IMPLEMENTED == *err)
		return lc_mem_free__(src_loc, allocator, err, ptr);

	return ret;
}

LC_NODISCARD
static inline void *
lc_mem_realloc__(
        struct lc_source_loc src_loc,
        struct lc_allocator allocator,
        enum lc_allocator_error *restrict err,
        void *restrict ptr,
        size_t size
)
{
	LC_ASSERT_(allocator.proc, "");
	if (NULL == ptr)
		return lc_mem_alloc__(src_loc, allocator, err, size);


	if (0 == size)
		return lc_mem_free__(src_loc, allocator, err, ptr);

	return allocator.proc(allocator.data, &src_loc, err, LC_ALLOCATOR_RESIZE, size, ptr);
}

LC_NODISCARD
static inline void *
lc_mem_realloc_sized__(
        struct lc_source_loc src_loc,
        struct lc_allocator allocator,
        enum lc_allocator_error *restrict err,
        void *restrict ptr,
        size_t old_size,
        size_t new_size
)
{
	LC_ASSERT_(allocator.proc, "");
	if (NULL == ptr)
		return lc_mem_alloc__(src_loc, allocator, err, new_size);

	if (0 == new_size)
		return lc_mem_free_sized__(src_loc, allocator, err, ptr, old_size);


	return allocator
	        .proc(allocator.data, &src_loc, err, LC_ALLOCATOR_RESIZE_SIZED, old_size, new_size, ptr);
}

static inline void
lc_mem_free_all__(
        struct lc_source_loc src_loc,
        struct lc_allocator allocator,
        enum lc_allocator_error *restrict err
)
{
	LC_ASSERT_(allocator.proc, "");
	(void)allocator.proc(allocator.data, &src_loc, err, LC_ALLOCATOR_FREE_ALL);
}

#define lc_mem_alloc_(...)         lc_mem_alloc__(lc_src_loc(&src_loc), __VA_ARGS__)
#define lc_mem_alloc_zero_(...)    lc_mem_alloc_zero__(lc_src_loc(&src_loc), __VA_ARGS__)
#define lc_mem_realloc_(...)       lc_mem_realloc__(lc_src_loc(&src_loc), __VA_ARGS__)
#define lc_mem_realloc_sized_(...) lc_mem_realloc_sized__(lc_src_loc(&src_loc), __VA_ARGS__)
#define lc_mem_free_(...)          lc_mem_free__(lc_src_loc(&src_loc), __VA_ARGS__)
#define lc_mem_free_sized_(...)    lc_mem_free_sized__(lc_src_loc(&src_loc), __VA_ARGS__)
#define lc_mem_free_all_(...)      lc_mem_free_all__(lc_src_loc(&src_loc), __VA_ARGS__)

#define lc_mem_alloc(...)         lc_mem_alloc_(ctx->allocator, __VA_ARGS__)
#define lc_mem_alloc_zero(...)    lc_mem_alloc_zero_(ctx->allocator, __VA_ARGS__)
#define lc_mem_realloc(...)       lc_mem_realloc_(ctx->allocator, __VA_ARGS__)
#define lc_mem_realloc_sized(...) lc_mem_realloc_sized_(ctx->allocator, __VA_ARGS__)
#define lc_mem_free(...)          lc_mem_free_(ctx->allocator, __VA_ARGS__)
#define lc_mem_free_sized(...)    lc_mem_free_sized_(ctx->allocator, __VA_ARGS__)
#define lc_mem_free_all(...)      lc_mem_free_all_(ctx->allocator, __VA_ARGS__)

#define lc_mem_alloc_t(...)         lc_mem_alloc_(ctx->temp_allocator, __VA_ARGS__)
#define lc_mem_alloc_zero_t(...)    lc_mem_alloc_zero_(ctx->temp_allocator, __VA_ARGS__)
#define lc_mem_realloc_t(...)       lc_mem_realloc_(ctx->temp_allocator, __VA_ARGS__)
#define lc_mem_realloc_sized_t(...) lc_mem_realloc_sized_(ctx->temp_allocator, __VA_ARGS__)
#define lc_mem_free_t(...)          lc_mem_free_(ctx->temp_allocator, __VA_ARGS__)
#define lc_mem_free_sized_t(...)    lc_mem_free_sized_(ctx->temp_allocator, __VA_ARGS__)
#define lc_mem_free_all_t(...)      lc_mem_free_all_(ctx->temp_allocator, __VA_ARGS__)

#include <errno.h>

#ifndef LC_NO_IMPLEMENTATION

extern void *_lc_default_allocator_proc(
        void *restrict data,
        const struct lc_source_loc *restrict src_loc,
        enum lc_allocator_error *restrict err,
        enum lc_allocator_cmd cmd,
        ...
)
#ifndef LC_IMPLEMENTATION
        ;
#else
{
	void *ret = NULL;
	va_list args;
	va_start(args, cmd);

	switch (cmd) {
	case LC_ALLOCATOR_INIT:
	{
		goto ret;
	}
	case LC_ALLOCATOR_ALLOCATE:
	{
		const size_t size = va_arg(args, size_t);

		void *p = malloc(size);
		if (NULL == p)
			goto err;

		ret = p;
		goto ret;
	}
	case LC_ALLOCATOR_FREE_SIZED: /* size = */ va_arg(args, size_t); LC_FALLTHROUGH;
	case LC_ALLOCATOR_FREE:
	{
		void *ptr = va_arg(args, void *);
		free(ptr);
		goto ret;
	}
	case LC_ALLOCATOR_FREE_ALL:
	{
		lc_unimplemented("");
		goto ret;
	}
	case LC_ALLOCATOR_RESIZE_SIZED: /* old_size = */ va_arg(args, size_t); LC_FALLTHROUGH;
	case LC_ALLOCATOR_RESIZE:
	{
		size_t size = va_arg(args, size_t);
		void *ptr = ret = va_arg(args, void *);

		void *p = realloc(ptr, size);
		if (NULL == p)
			goto err;


		ret = p;
		goto ret;
	}
	}


ret:
	*err = LC_ALLOCATOR_ERROR_NONE;

	va_end(args);
	return ret;

err:
	*err = LC_ALLOCATOR_ERROR_OUT_OF_MEMORY;

	va_end(args);
	return ret;
}
#endif

LC_NODISCARD
static inline struct lc_allocator
lc_std_allocator(void)
{
	return (struct lc_allocator){
	        .proc = _lc_default_allocator_proc,
	};
}

#endif

#ifndef LC_NO_IMPLEMENTATION

extern void *_lc_bump_allocator_proc(
        void *restrict data,
        const struct lc_source_loc *restrict src_loc,
        enum lc_allocator_error *restrict err,
        enum lc_allocator_cmd cmd,
        ...
)
#ifndef LC_IMPLEMENTATION
        ;
#else
{
	struct bump_allocator_data {
		size_t total_size;
		size_t allocated;
		u8 data[];
	} *b = data;

	void *ret = NULL;
	*err      = LC_ALLOCATOR_ERROR_NONE;

	LC_ASSERT_(b->allocated <= b->total_size, "We cannot have more memory allocate than available");

	va_list args;
	va_start(args, cmd);

	switch (cmd) {
	case LC_ALLOCATOR_INIT:
	{
		struct lc_mut_bslice bslice = va_arg(args, struct lc_mut_bslice);

		if (bslice.size <= offsetof(struct bump_allocator_data, data))
#if 1
			goto out_of_memory;
#else
			LC_ASSERT_(false, "Not enought memory to allocate bump allocator");
#endif

		*b = (struct bump_allocator_data){
		        .total_size = bslice.size - offsetof(struct bump_allocator_data, data),
		        .allocated  = 0,
		};

		ret = b;

		goto ret;
	}
	case LC_ALLOCATOR_ALLOCATE:
	{
		LC_ASSERT_(
		        b->allocated <= b->total_size,
		        "We cannot have more memory allocate than available"
		);

		size_t size = va_arg(args, size_t);

		if (b->total_size - b->allocated < size)
			goto out_of_memory;


		ret = &b->data[b->allocated];

		b->allocated += size;

		goto ret;
	}
	case LC_ALLOCATOR_RESIZE: goto not_implemented;
	case LC_ALLOCATOR_RESIZE_SIZED:
	{
		size_t old_size = va_arg(args, size_t);
		size_t new_size = va_arg(args, size_t);
		void *ptr = ret = va_arg(args, void *);


		if (b->allocated < old_size)
			goto invalid_arg;

		if (!IN_RANGE((uintptr_t)ptr, (uintptr_t)b->data, (uintptr_t)&b->data[b->allocated]))
			goto invalid_arg;

		LC_ASSERT_(old_size < SSIZE_MAX && new_size < SSIZE_MAX, "(Poor) Overflow check");

		bool is_end      = ptr == &b->data[b->allocated - old_size];
		bool is_grow     = old_size < new_size;
		size_t delta_size = ABS((ssize)new_size - (ssize)old_size);

		if (is_end) {

			if (is_grow && b->allocated + delta_size > b->total_size)
				goto out_of_memory;


			LC_ASSERT_(
			        is_grow || delta_size < b->allocated,
			        "Impossible to shrink into negative"
			);

			if (is_grow)
				b->allocated += delta_size;
			else
				b->allocated -= delta_size;

			goto ret;
		}


		if (is_grow) {
			if (b->total_size < b->allocated + new_size)
				goto out_of_memory;


			ret = &b->data[b->allocated];
			memcpy(ret, ptr, old_size);

			b->allocated += new_size;

			goto ret;
		}

		// Shrinking is a no-op
		goto ret;
	}
	case LC_ALLOCATOR_FREE:
	{
		goto ret; // Free is just a no-op
	}
	case LC_ALLOCATOR_FREE_SIZED:
	{
		size_t size = va_arg(args, size_t);
		void *ptr  = va_arg(args, void *);

		// NOTE: Because we never dereference ptr, we can skip NULL check.
		if (size <= b->allocated && &b->data[b->allocated - size] == ptr)
			b->allocated -= size;

		goto ret;
	}
	case LC_ALLOCATOR_FREE_ALL:
	{
		b->allocated = 0;
		goto ret;
	}
	}


ret:

	va_end(args);
	return ret;

out_of_memory:
	*err = LC_ALLOCATOR_ERROR_OUT_OF_MEMORY;

	va_end(args);
	return NULL;

invalid_arg:
	*err = LC_ALLOCATOR_ERROR_INVALID_ARG;

	va_end(args);
	return NULL;

not_implemented:
	*err = LC_ALLOCATOR_ERROR_NOT_IMPLEMENTED;

	va_end(args);
	return NULL;
}
#endif

LC_NODISCARD
static inline struct lc_allocator
lc_bump_allocator_(struct lc_mut_bslice slice, enum lc_allocator_error *err, struct lc_source_loc src_loc)
{
	return (struct lc_allocator){
	        .proc = _lc_bump_allocator_proc,
	        .data = _lc_bump_allocator_proc(slice.data, &src_loc, err, LC_ALLOCATOR_INIT, slice),
	};
}

#define lc_bump_allocator(slice, err) lc_bump_allocator_(slice, err, lc_src_loc(&src_loc))

#endif

////////////////////////////////////////////////////////////////////////////////
/// ANSI Escape Code                                                [LC_XAE] ///
////////////////////////////////////////////////////////////////////////////////

#ifdef LC_ENABLE_ANSI_ESC

#define CSI "\033["

#define VT_RESET          CSI "0m"
#define VT_BLACK          CSI "30m"
#define VT_RED            CSI "31m"
#define VT_GREEN          CSI "32m"
#define VT_YELLOW         CSI "33m"
#define VT_BLUE           CSI "34m"
#define VT_MAGENTA        CSI "35m"
#define VT_CYAN           CSI "36m"
#define VT_WHITE          CSI "37m"
#define VT_BRIGHT_BLACK   CSI "90m"
#define VT_BRIGHT_RED     CSI "91m"
#define VT_BRIGHT_GREEN   CSI "92m"
#define VT_BRIGHT_YELLOW  CSI "93m"
#define VT_BRIGHT_BLUE    CSI "94m"
#define VT_BRIGHT_MAGENTA CSI "95m"
#define VT_BRIGHT_CYAN    CSI "96m"
#define VT_BRIGHT_WHITE   CSI "97m"


#define VT_BLACK_BG          CSI "40m"
#define VT_RED_BG            CSI "41m"
#define VT_GREEN_BG          CSI "42m"
#define VT_YELLOW_BG         CSI "43m"
#define VT_BLUE_BG           CSI "44m"
#define VT_MAGENTA_BG        CSI "45m"
#define VT_CYAN_BG           CSI "46m"
#define VT_WHITE_BG          CSI "47m"
#define VT_BRIGHT_BLACK_BG   CSI "100m"
#define VT_BRIGHT_RED_BG     CSI "101m"
#define VT_BRIGHT_GREEN_BG   CSI "102m"
#define VT_BRIGHT_YELLOW_BG  CSI "103m"
#define VT_BRIGHT_BLUE_BG    CSI "104m"
#define VT_BRIGHT_MAGENTA_BG CSI "105m"
#define VT_BRIGHT_CYAN_BG    CSI "106m"
#define VT_BRIGHT_WHITE_BG   CSI "107m"

// clear(3) dumps this sequence on xterm
#define VT_CLEAR     CSI "H" CSI "J" CSI "3J"
#define VT_BOLD      CSI "1m"
#define VT_ITALIC    CSI "3m"
#define VT_UNDERLINE CSI "4m"
#define VT_BLINK     CSI "5m"
#define VT_STRIKE    CSI "9m"

#endif

////////////////////////////////////////////////////////////////////////////////
/// Time                                                            [LC_XCH] ///
////////////////////////////////////////////////////////////////////////////////

struct lc_clock {
	i64 sec;
	i64 nsec;
};

static_assert(sizeof(u64[2]) == sizeof(struct lc_clock), "");

#include <time.h>

LC_NODISCARD static LC_ALWAYS_INLINE struct lc_clock lc_clock_from_timespec(struct timespec ts);
LC_NODISCARD static inline struct lc_clock lc_clock_init(void);

LC_NODISCARD LC_CONST static inline f64 lc_clock_get_sec(struct lc_clock clock);
LC_NODISCARD LC_CONST static inline f64 lc_clock_get_ms(struct lc_clock clock);

static inline f64 lc_clock_reset(struct lc_clock *clock);
static inline f64 lc_clock_reset_ms(struct lc_clock *clock);

LC_NODISCARD static inline f64 lc_clock_since(struct lc_clock clock);
LC_NODISCARD static inline f64 lc_clock_since_ms(struct lc_clock clock);

LC_UNSEQUENCED_FUNC(lc_clock_from_timespec);

#ifdef LC_HAVE_ORDER
LC_NODISCARD static inline enum lc_order lc_clock_cmp(struct lc_clock lhs, struct lc_clock rhs);
LC_UNSEQUENCED_FUNC(lc_clock_cmp);
#endif


LC_NODISCARD
static LC_ALWAYS_INLINE struct lc_clock
lc_clock_from_timespec(struct timespec ts)
{
	return (struct lc_clock){
	        .sec  = ts.tv_sec,
	        .nsec = ts.tv_nsec,
	};
}

LC_NODISCARD
static inline struct lc_clock
lc_clock_init(void)
{
	struct timespec ts;
#if defined(LC_PLATFORM_POSIX)
#if defined(_POSIX_MONOTONIC_CLOCK)
	clock_gettime(CLOCK_MONOTONIC, &ts);
#else
	clock_gettime(CLOCK_REALTIME, &ts);
#endif
#elif LC_STDC_C11 <= __STDC_VERSION__ || defined(LC_STD_CXX)
	timespec_get(&ts, TIME_UTC);
#else
#error "FIX me"
#endif


	return lc_clock_from_timespec(ts);
}

LC_NODISCARD LC_CONST static inline f64
lc_clock_get_sec(struct lc_clock clock)
{
	return (f64)clock.sec + clock.nsec / 1e9;
}

LC_NODISCARD LC_CONST static inline f64
lc_clock_get_ms(struct lc_clock clock)
{
	return clock.sec * 1e3 + clock.nsec / 1e6;
}

static inline f64
lc_clock_reset(struct lc_clock *clock)
{
	const struct lc_clock ts = lc_clock_init();
	const f64 delta_sec      = lc_clock_get_sec(ts) - lc_clock_get_sec(*clock);

	*clock = ts;
	return delta_sec;
}

static inline f64
lc_clock_reset_ms(struct lc_clock *clock)
{
	const struct lc_clock ts = lc_clock_init();
	const f64 delta_sec      = lc_clock_get_ms(ts) - lc_clock_get_ms(*clock);

	*clock = ts;
	return delta_sec;
}

LC_NODISCARD
static inline f64
lc_clock_since(struct lc_clock clock)
{
	return lc_clock_get_sec(lc_clock_init()) - lc_clock_get_sec(clock);
}

LC_NODISCARD
static inline f64
lc_clock_since_ms(struct lc_clock clock)
{
	return lc_clock_get_ms(lc_clock_init()) - lc_clock_get_ms(clock);
}

#ifdef LC_HAVE_ORDER

LC_NODISCARD static inline enum lc_order
lc_clock_cmp(struct lc_clock lhs, struct lc_clock rhs)
{
#if 0
	// Simple, but very slow approach. The code is still here, but disabled
	// for understandability and future reference

	if (lhs.sec < rhs.sec)
		return LC_ORDER_LESS;
	if (lhs.sec > rhs.sec)
		return LC_ORDER_GREATER;
	if (lhs.nsec < rhs.nsec)
		return LC_ORDER_LESS;
	if (lhs.nsec > rhs.nsec)
		return LC_ORDER_GREATER;

	return LC_ORDER_EQUAL;
#else
#if defined(LC_CLOCK_NO_ASSUMPTION)
	// Slower compare, that works for all (even invalid) input.
	//
	// This is the approach you would expect from a performant branchless
	// approach because you have to workaround the corner case of
	// overflow/overflow.
	//
	// Clever, but idiomatic. It contains more conditional operations than
	// the naive approach however.
	int_fast8_t sec  = (rhs.sec  < lhs.sec)  - (rhs.sec  > lhs.sec);
	int_fast8_t nsec = (rhs.nsec < lhs.nsec) - (rhs.nsec > lhs.nsec);

	sec *= 8;
	sec += nsec;

	return (0 < sec) - (0 > sec);
#else
	// Naive subtraction based compare (kinda how strcmp works)
	// The reason it is naive and does not work for all cases is because of
	// overflow/underflow of extreme high/low values. BUT IT IS FAST!
	//
	// We can sidestep the problems with the following assumptions however:

	// Assumption 1:
	//
	// Negative seconds does not make sense. Most implementation uses
	// signed integer to represent seconds in timespec anyway.
	//
	// With this assumption, we are guaranteed to never underflow when we
	// do subtraction.
	//
	lc_assume(0 <= (i64)lhs.sec);
	lc_assume(0 <= (i64)rhs.sec);

	// Assumption 2:
	//
	// We assume that the .nsec is in range of nanoseconds [0, 10^9]
	// This is also mandated by POSIX and the C standard anyway.
	//
	// With this assumption, we are guaranteed to never underflow when we
	// do subtraction.
	//
	// For reference:
	//   milli 10^-3
	//   micro 10^-6
	//   nano  10^-9
	lc_assume(IN_RANGE((i32)lhs.nsec, 0, 999999999));
	lc_assume(IN_RANGE((i32)rhs.nsec, 0, 999999999));

	i64 sec  = (i64)lhs.sec  - (i64)rhs.sec;
	i32 nsec = (i32)lhs.nsec - (i32)rhs.nsec;

	sec = sec ? sec : nsec;

	return (0 < sec) - (0 > sec);
#endif
#endif
}

#endif

////////////////////////////////////////////////////////////////////////////////
/// String view                                                     [LC_XSV] ///
////////////////////////////////////////////////////////////////////////////////

#include <string.h>

struct lc_sv {
	size_t length;
	const char *s;
};

#define sv(...)       ((struct lc_sv){__VA_ARGS__})
#define sv_c(str)     sv(.length = LEN(str) - 1, .s = str)
#define sv_cstr(cstr) sv(strlen(LC_ELVIS(cstr, "")), cstr)

#define LC_PRI_SV ".*s"
#define LC_PRI_SV_ARGS(sv) (int)(sv).length, (sv).s

LC_NODISCARD LC_CONST static LC_ALWAYS_INLINE size_t lc_sv_len(struct lc_sv sv);
LC_NODISCARD LC_CONST static LC_ALWAYS_INLINE bool lc_sv_is_empty(struct lc_sv sv);
LC_NODISCARD LC_CONST static LC_ALWAYS_INLINE const char * lc_sv_begin(struct lc_sv sv);
LC_NODISCARD LC_CONST static LC_ALWAYS_INLINE const char * lc_sv_end(struct lc_sv sv);
LC_NODISCARD static inline bool lc_sv_is_equal(struct lc_sv lhs, struct lc_sv rhs);
LC_NODISCARD static inline bool lc_sv_is_prefix(struct lc_sv sv, struct lc_sv prefix);
LC_NODISCARD static inline bool lc_sv_is_suffix(struct lc_sv sv, struct lc_sv suffix);
LC_NODISCARD static inline struct lc_sv lc_sv_find(struct lc_sv sv, struct lc_sv substring);
LC_NODISCARD static inline bool lc_sv_is_substring(struct lc_sv sv, struct lc_sv substring);
LC_NODISCARD static inline struct lc_sv lc_sv_sub(struct lc_sv sv, size_t len, size_t idx);

static inline bool lc_sv_split(struct lc_sv *restrict iter, struct lc_sv delim, struct lc_sv *restrict value);
static inline bool lc_sv_split_c(struct lc_sv *restrict iter, char c, struct lc_sv *restrict value);


LC_UNSEQUENCED_FUNC(lc_sv_is_equal);
LC_UNSEQUENCED_FUNC(lc_sv_is_suffix);
LC_UNSEQUENCED_FUNC(lc_sv_find);
LC_UNSEQUENCED_FUNC(lc_sv_is_substring);
LC_UNSEQUENCED_FUNC(lc_sv_sub);
LC_UNSEQUENCED_FUNC(lc_sv_is_prefix);


#ifdef LC_HAVE_ORDER
LC_NODISCARD static inline enum lc_order lc_sv_cmp(struct lc_sv lhs, struct lc_sv rhs);
LC_UNSEQUENCED_FUNC(lc_sv_cmp);
#endif

LC_NODISCARD LC_CONST static LC_ALWAYS_INLINE size_t
lc_sv_len(struct lc_sv sv)
{
	return sv.length;
}

LC_NODISCARD LC_CONST static LC_ALWAYS_INLINE bool
lc_sv_is_empty(struct lc_sv sv)
{
	return 0 == sv.length;
}

LC_NODISCARD LC_CONST static LC_ALWAYS_INLINE const char *
lc_sv_begin(struct lc_sv sv)
{
	return sv.s;
}

LC_NODISCARD LC_CONST static LC_ALWAYS_INLINE const char *
lc_sv_end(struct lc_sv sv)
{
	if (sv.s)
		return lc_sv_begin(sv) + sv.length;

	return NULL;
}

LC_NODISCARD static inline bool
lc_sv_is_equal(struct lc_sv lhs, struct lc_sv rhs)
{
	return lhs.length == rhs.length && 0 == memcmp(lhs.s, rhs.s, lhs.length);
}

LC_NODISCARD static inline bool
lc_sv_is_prefix(struct lc_sv sv, struct lc_sv prefix)
{
	return prefix.length <= sv.length && 0 == memcmp(sv.s, prefix.s, prefix.length);
}

LC_NODISCARD static inline bool
lc_sv_is_suffix(struct lc_sv sv, struct lc_sv suffix)
{
	return suffix.length <= sv.length
	    && 0 == memcmp(sv.s + sv.length - suffix.length, suffix.s, suffix.length);
}

LC_NODISCARD static inline struct lc_sv
lc_sv_find(struct lc_sv sv, struct lc_sv substring)
{
	if (0 == substring.length)
		return sv;
	if (sv.length < substring.length)
		return sv(0);

#if 0
	// TODO(larry): Performance test between the nested for loop vs memcmp
	for (size_t i = 0; i <= sv.length - substring.length; i++) {
		for (size_t j = 0; j < substring.length; j++)
			if (sv.s[i + j] != substring.s[j])
				goto next;

		return sv(substring.length, sv.s + i);
next:;
	}


#else
	// TODO(larry): Performance test between the nested for loop vs memcmp
	for (size_t i = 0; i <= sv.length - substring.length; i++)
		if (0 == memcmp(sv.s + i, substring.s, substring.length))
			return sv(substring.length, sv.s + i);

#endif


	return sv(0);
}

LC_NODISCARD static inline bool
lc_sv_is_substring(struct lc_sv sv, struct lc_sv substring)
{
	return !lc_sv_is_empty(lc_sv_find(sv, substring));
}

#ifdef LC_HAVE_ORDER

LC_NODISCARD static inline enum lc_order
lc_sv_cmp(struct lc_sv lhs, struct lc_sv rhs)
{
	const size_t len = MAX(lhs.length, rhs.length);
	return lc_order_from_int(memcmp(lhs.s, rhs.s, len));
}
#endif

LC_NODISCARD static inline struct lc_sv
lc_sv_sub(struct lc_sv sv, size_t len, size_t idx)
{
	/// Case 4. Length zero
	///
	/// idx: does not matter
	/// len: 0
	///
	/// Returns empty string
	if (0 == len)
		return sv(0);

	/// Case 2. Index out of bound
	///
	/// idx: [sv.length; ]
	/// len: does not matter
	///
	/// Returns empty string
	if (sv.length <= idx)
		return sv(0);

	/// Case 3. Substring length truncate
	///
	/// idx: [0;   sv.length)
	/// len: (sv.length - idx; ]
	///
	/// Trucate substring
	if (sv.length < idx + len)
		len -= (idx + len) - sv.length;

	/// Case 1. Normal
	///
	/// idx: [0;   sv.length]
	/// len: [0; sv.length - idx]
	///
	/// Returns substring as expected
	return sv(.length = len, .s = sv.s + idx);
}

///
/// @breif Split iterator based on delimiter
///
/// @param[in,out] iter  The iterator
/// @param[in]     delim The delimiter
/// @param[in,out] value The return value
///
/// @returns false when the iterator has hit the end
///
/// @code{.c}
/// struct lc_sv line = sv_c("1200/1246/1200");
///
/// for (struct lc_sv iter = line, value; lc_sv_split(&iter, sv_c("/"), &value);)
///         printf("num %.*s\n", (int)value.length, value.s);
/// @endcode
///
///
static inline bool
lc_sv_split(struct lc_sv *restrict iter, struct lc_sv delim, struct lc_sv *restrict value)
{
	if (0 == iter->length) {
		*value = sv(0);

		return false;
	}


	if (lc_sv_is_empty(delim)) {
		*value = sv(1, iter->s);
		iter->s++, iter->length--;

		return true;
	}

	struct lc_sv needle = lc_sv_find(*iter, delim);

	if (lc_sv_is_empty(needle)) {
		*value = *iter;
		*iter  = sv(0);

		return true;
	}

	*value = lc_sv_sub(*iter, needle.s - iter->s, 0);

	iter->length -= value->length + needle.length;
	iter->s      += value->length + needle.length;

	return true;
}

///
/// @breif Split iterator based on delimiter
///
/// @param[in,out] iter  The iterator
/// @param[in]     delim The delimiter
/// @param[in,out] value The return value
///
/// @returns false when the iterator has hit the end
///
/// @code{.c}
/// struct lc_sv line = sv_c("1201/1249/1201");
///
/// for (struct lc_sv iter = line, value; lc_sv_split_c(&iter, '/', &value);)
///         printf("num %.*s\n", (int)value.length, value.s);
/// @endcode
///
///
static inline bool
lc_sv_split_c(struct lc_sv *restrict iter, char c, struct lc_sv *restrict value)
{
	if (0 == iter->length) {
		*value = sv(0);
		return false;
	}


	for (struct lc_sv it = *iter; it.length; it.length--, it.s++) {
		if (c == it.s[0]) {
			*value = sv(.length = it.s - iter->s, .s = iter->s);

			iter->length -= value->length + 1;
			iter->s      += value->length + 1;
			return true;
		}
	}

	*value = *iter;
	*iter  = sv(0);

	return true;
}

////////////////////////////////////////////////////////////////////////////////
/// Logger                                                          [LC_XLG] ///
////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <string.h>

enum lc_log_level
{
	LC_LOG_LEVEL_TODO,
	LC_LOG_LEVEL_DEBUG,
	LC_LOG_LEVEL_TRACE,
	LC_LOG_LEVEL_INFO,
	LC_LOG_LEVEL_WARN,
	LC_LOG_LEVEL_ERROR,
	LC_LOG_LEVEL_FATAL,

	LC_LOG_LEVEL_DEFAULT = (1 << LC_LOG_LEVEL_TODO)
		             | (1 << LC_LOG_LEVEL_INFO)
			     | (1 << LC_LOG_LEVEL_WARN)
	                     | (1 << LC_LOG_LEVEL_ERROR)
			     | (1 << LC_LOG_LEVEL_FATAL)
};

enum LC_FLAG_ENUM lc_log_option
{
	LC_LOG_ENABLE_TIMESTAMP = 1 << 0,
	LC_LOG_ENABLE_LOG_LEVEL = 1 << 1,
	LC_LOG_ENABLE_FILENAME  = 1 << 2,
	LC_LOG_ENABLE_CALLSITE  = 1 << 3,
	LC_LOG_ENABLE_COLOR     = 1 << 4,

	LC_LOG_ENABLE_DEFAULT = LC_LOG_ENABLE_TIMESTAMP | LC_LOG_ENABLE_LOG_LEVEL | LC_LOG_ENABLE_FILENAME
	                      | LC_LOG_ENABLE_CALLSITE | LC_LOG_ENABLE_COLOR,
};

#ifndef __cplusplus

LC_NODISCARD
static inline enum lc_log_level
lc_log_level_from_env(const char *env)
{
	const char *log_level = getenv(LC_ELVIS(env, "LC_LOG_LEVEL"));

	enum lc_log_level level_mask = 0;

	if (NULL == log_level)
		return level_mask;

	if (strstr(log_level, "default"))
		level_mask |= LC_LOG_LEVEL_DEFAULT;

	if (strstr(log_level, "todo"))
		level_mask |= 1 << LC_LOG_LEVEL_TODO;

	if (strstr(log_level, "debug"))
		level_mask |= 1 << LC_LOG_LEVEL_DEBUG;

	if (strstr(log_level, "trace"))
		level_mask |= 1 << LC_LOG_LEVEL_TRACE;

	if (strstr(log_level, "info"))
		level_mask |= 1 << LC_LOG_LEVEL_INFO;

	if (strstr(log_level, "warn"))
		level_mask |= 1 << LC_LOG_LEVEL_WARN;

	if (strstr(log_level, "error"))
		level_mask |= 1 << LC_LOG_LEVEL_ERROR;

	if (strstr(log_level, "fatal"))
		level_mask |= 1 << LC_LOG_LEVEL_FATAL;

	return level_mask;
}

#endif

struct lc_ctx;

struct lc_logger {
	void (*proc)(
	        void *data,
	        enum lc_log_option options,
	        enum lc_log_level level,
	        struct lc_source_loc src_loc,
	        size_t n,
	        const char msg[n]
	);
	void *data;
	enum lc_log_level level_mask;
	enum lc_log_option options;
};

LC_PRINTF_LIKE(4, 5)
static inline void
_lc_log(struct lc_logger logger,
        enum lc_log_level level,
        struct lc_source_loc src_loc,
        const char *format,
        ...)

{
	if (0 == (logger.level_mask & (1 << level)))
		return;

	char buffer[8192];
	buffer[0] = 0;

	va_list args;
	va_start(args, format);
	size_t written = (size_t)vsnprintf(buffer, sizeof buffer, format, args);
	va_end(args);

	buffer[written] = 0;

	logger.proc(logger.data, logger.options, level, src_loc, written, buffer);
}

static inline void
_lc_log_v(struct lc_logger logger,
        enum lc_log_level level,
        struct lc_source_loc src_loc,
        const char *format,
        va_list args)

{
	if (0 == (logger.level_mask & (1 << level)))
		return;

	char buffer[8192];
	buffer[0] = 0;
	size_t written = (size_t)vsnprintf(buffer, sizeof buffer, format, args);
	buffer[written] = 0;

	logger.proc(logger.data, logger.options, level, src_loc, written, buffer);
}

#ifndef NDEBUG
#define lc_log_todo(...) _lc_log(ctx->logger, LC_LOG_LEVEL_TODO, lc_src_loc(0), __VA_ARGS__)
#else
#define lc_log_todo(...) (void)(__VA_ARGS__) // the arguments may have side effects!
#endif
#define lc_log_debug(...) _lc_log(ctx->logger, LC_LOG_LEVEL_DEBUG, lc_src_loc(0), __VA_ARGS__)
#define lc_log_info(...)  _lc_log(ctx->logger, LC_LOG_LEVEL_INFO,  lc_src_loc(0), __VA_ARGS__)
#define lc_log_warn(...)  _lc_log(ctx->logger, LC_LOG_LEVEL_WARN,  lc_src_loc(0), __VA_ARGS__)
#define lc_log_error(...) _lc_log(ctx->logger, LC_LOG_LEVEL_ERROR, lc_src_loc(0), __VA_ARGS__)
#define lc_log_fatal(...) _lc_log(ctx->logger, LC_LOG_LEVEL_FATAL, lc_src_loc(0), __VA_ARGS__)

#ifndef NDEBUG
#define lc_log_todo_once(...)                                                                                \
	do {                                                                                                 \
		/* We introduce a new scope as __VA_ARGS__ might contain printed */                          \
		{                                                                                            \
			static bool printed = false;                                                         \
			if (printed)                                                                         \
				break;                                                                       \
                                                                                                             \
			printed = true;                                                                      \
		}                                                                                            \
		lc_log_todo(__VA_ARGS__);                                                                    \
	} while (false)
#else
#define lc_log_todo_once(...) (void)(__VA_ARGS__)
#endif

#ifdef LC_IMPLEMENTATION
static inline size_t
_lc_print_log_level_raw(enum lc_log_level level, size_t n, char buffer[n])
{
	static const char *log_level_str[] = {
	        [LC_LOG_LEVEL_TODO]  = "TODO",
	        [LC_LOG_LEVEL_DEBUG] = "DEBUG",
	        [LC_LOG_LEVEL_TRACE] = "TRACE",
	        [LC_LOG_LEVEL_INFO]  = "INFO",
	        [LC_LOG_LEVEL_WARN]  = "WARN",
	        [LC_LOG_LEVEL_ERROR] = "ERROR",
	        [LC_LOG_LEVEL_FATAL] = "FATAL",
	};
	return (size_t)snprintf(buffer, n, "[%s] ", log_level_str[level]);
}

static inline size_t
_lc_print_log_level_color(enum lc_log_level level, size_t n, char buffer[n])
{
	static const char *log_level_str[] = {
	        [LC_LOG_LEVEL_TODO]  = VT_CYAN "TODO",
	        [LC_LOG_LEVEL_DEBUG] = VT_CYAN "DEBUG",
	        [LC_LOG_LEVEL_TRACE] = VT_BRIGHT_BLACK "TRACE",
	        [LC_LOG_LEVEL_INFO]  = VT_BRIGHT_GREEN "INFO",
	        [LC_LOG_LEVEL_WARN]  = VT_BRIGHT_YELLOW "WARN",
	        [LC_LOG_LEVEL_ERROR] = VT_RED "ERROR",
	        [LC_LOG_LEVEL_FATAL] = VT_WHITE VT_RED_BG VT_BLINK "FATAL",
	};
	return (size_t)snprintf(buffer, n, "[%s" VT_RESET "] ", log_level_str[level]);
}

static inline size_t
_lc_print_log_level(enum lc_log_level level, size_t n, char buffer[n], bool colored)
{

	if (colored)
		return _lc_print_log_level_color(level, n, buffer);

	return _lc_print_log_level_raw(level, n, buffer);
}

static inline size_t
_lc_print_timestamp(size_t n, char buffer[n])
{
	size_t written = 0;

	struct tm tm          = {0};
	struct lc_clock clock = lc_clock_init();

	// TODO: Portability with gmtime_r(3)
	gmtime_r((time_t *)&clock.sec, &tm);

	written += strftime(buffer, n, "[%Y-%m-%d %H:%M:%S", &tm);
	written += (size_t)snprintf(buffer + written, n - written, ".%03"PRIu64"] ", clock.nsec / 1000000);

	return written;
}

static inline size_t
_lc_print_filename(const char *restrict filename, int line, size_t n, char buffer[n])
{
	return (size_t)snprintf(
	        buffer,
	        n,
	        VT_BRIGHT_YELLOW "%s" VT_RESET ":" VT_BRIGHT_BLUE "%03d" VT_RESET,
	        filename,
	        line
	);
}
#endif

#ifndef LC_NO_IMPLEMENTATION

extern void _lc_log_console_format(
        void *_,
        enum lc_log_option options,
        enum lc_log_level level,
        struct lc_source_loc src_loc,
        size_t n,
        const char msg[n]
)
#ifndef LC_IMPLEMENTATION
        ;
#else
{
	char buffer[4096];
	size_t written = 0;
	buffer[0]     = 0;

	if (options & LC_LOG_ENABLE_TIMESTAMP)
		written += _lc_print_timestamp(sizeof buffer - written, buffer + written);
	if (options & LC_LOG_ENABLE_LOG_LEVEL)
		written += _lc_print_log_level(
		        level,
		        sizeof buffer - written,
		        buffer + written,
		        options & LC_LOG_ENABLE_COLOR
		);
	if (options & LC_LOG_ENABLE_FILENAME)
		written += _lc_print_filename(
		        src_loc.data->filename,
		        src_loc.data->line,
		        sizeof buffer - written,
		        buffer + written
		);
	if (options & LC_LOG_ENABLE_CALLSITE)
		written += snprintf(
		        buffer + written,
		        sizeof buffer - written,
		        ":" VT_GREEN "%s" VT_RESET "()",
		        src_loc.data->func
		);

	if (written)
		buffer[written++] = ' ';
	buffer[written] = 0;

	FILE *file = stdout;

	fwrite(buffer, 1, written, file);
	fwrite(msg, 1, n, file);
	fputc('\n', file);

	return;
}
#endif

LC_NODISCARD
static inline struct lc_logger
lc_log_console_create(enum lc_log_level level_mask, enum lc_log_option options)
{
	return (struct lc_logger){
	        .proc       = _lc_log_console_format,
	        .data       = NULL,
	        .level_mask = level_mask,
	        .options    = options,
	};
}

#endif

#ifndef LC_NO_IMPLEMENTATION

extern void _lc_log_tee(
        void *data,
        enum lc_log_option options,
        enum lc_log_level level,
        struct lc_source_loc src_loc,
        size_t n,
        const char msg[n]
)
#ifndef LC_IMPLEMENTATION
        ;
#else
{
	for (const struct lc_logger **l = data; *l; l++) {
		const struct lc_logger *logger = *l;

		if (0 == (logger->level_mask & (1 << level)))
			continue;

		logger->proc(logger->data, logger->options, level, src_loc, n, msg);
	}
}
#endif

LC_NODISCARD
static inline struct lc_logger
_lc_log_tee_create(struct lc_logger *loggers[])
{
	return (struct lc_logger){
	        .proc       = _lc_log_tee,
	        .data       = loggers,
	        .level_mask = (enum lc_log_level) ~0,
	        .options    = (enum lc_log_option) ~0,
	};
}

#define lc_log_tee_create(...) _lc_log_tee_create((struct lc_logger *[]){__VA_ARGS__, NULL})

#endif

////////////////////////////////////////////////////////////////////////////////
/// Context                                                         [LC_XCT] ///
////////////////////////////////////////////////////////////////////////////////

struct lc_ctx {

	/// NOTE(larry): Not sure if I like user_ptr in pratice, where is the
	/// coordination of ownership and data definition? Only usefull for end-user
	/// programmer... Use case for user_ptr:
	///	- per thread context pointer
	///	- per module context pointer
	// void *user_ptr;

	struct lc_allocator allocator;
	struct lc_allocator temp_allocator;

	struct lc_logger logger;
};

#define lc_context lc_ctx

#define LC_ARGS_WARP(func, ...) func(LC_ARGS_CALL, __VA_ARGS__)
#define LC_ARGS_DECL            struct lc_ctx *ctx, struct lc_source_loc src_loc
#define LC_ARGS_CNT             2
#define LC_ARGS_CALL            ctx, lc_src_loc(&src_loc)
#define LC_ARGS_PASS            ctx, src_loc
#define LC_ARGS(ctx, src_loc)   ctx, src_loc

////////////////////////////////////////////////////////////////////////////////
/// Input/Output                                                    [LC_XIO] ///
////////////////////////////////////////////////////////////////////////////////

#ifdef LC_ENABLE_IO

/// TODO(larry): Rethink type for position, length (written)
typedef u64 lc_io_pos;
typedef i64 lc_io_off;
typedef size_t lc_io_len;

enum LC_NODISCARD lc_io_error
{
	LC_IO_ERR_NONE,
	LC_IO_ERR_NOT_IMPLEMENTED,
	LC_IO_ERR_EOF,
	LC_IO_ERR_INVALID_ARG,
	LC_IO_ERR_UNKNOWN,
};

enum lc_io_seek_mode
{
	LC_IO_SEEK_SET = SEEK_SET,
	LC_IO_SEEK_CUR = SEEK_CUR,
	LC_IO_SEEK_END = SEEK_END,
};

enum lc_io_mode
{
	LC_IO_MODE_INIT,
	LC_IO_MODE_CLOSE,
	LC_IO_MODE_FLUSH,
	LC_IO_MODE_READ,
	LC_IO_MODE_READ_AT,
	LC_IO_MODE_WRITE,
	LC_IO_MODE_WRITE_AT,
	LC_IO_MODE_SEEK,
	LC_IO_MODE_TELL,
	LC_IO_MODE_SIZE,
	LC_IO_MODE_DESTROY,
	LC_IO_MODE_QUERY,
};

struct lc_io {
	enum lc_io_error (*proc)(LC_ARGS_DECL, void *data, enum lc_io_mode mode, ...);
	void *data;
};

static inline enum lc_io_error
lc_io_close_(LC_ARGS_DECL, struct lc_io io)
{
	return io.proc(LC_ARGS_PASS, io.data, LC_IO_MODE_CLOSE);
}

static inline enum lc_io_error
lc_io_flush_(LC_ARGS_DECL, struct lc_io io)
{
	return io.proc(LC_ARGS_PASS, io.data, LC_IO_MODE_FLUSH);
}

static inline enum lc_io_error
lc_io_read_(LC_ARGS_DECL, struct lc_io io, size_t n, void *buffer, lc_io_len *written)
{
	return io.proc(LC_ARGS_PASS, io.data, LC_IO_MODE_READ, n, buffer, written);
}

static inline enum lc_io_error
lc_io_read_slice_(LC_ARGS_DECL, struct lc_io io, struct lc_mut_bslice slice, lc_io_len *written)
{
	return io.proc(LC_ARGS_PASS, io.data, LC_IO_MODE_READ, slice.size, slice.data, written);
}

static inline enum lc_io_error
lc_io_read_at_(
        LC_ARGS_DECL,
        struct lc_io io,
        size_t n,
        void *restrict buffer,
        lc_io_off offset,
        lc_io_len *written
)
{
	enum lc_io_error err = io.proc(LC_ARGS_PASS, io.data, LC_IO_MODE_READ_AT, n, buffer, offset, written);

	if (LC_IO_ERR_NOT_IMPLEMENTED == err) {
		lc_io_pos old_pos = 0;
		lc_io_len len     = 0;

		err = io.proc(LC_ARGS_PASS, io.data, LC_IO_MODE_SEEK, 0, LC_IO_SEEK_CUR, &old_pos);
		if (LC_IO_ERR_NOT_IMPLEMENTED == err)
			return err;

		err = io.proc(LC_ARGS_PASS, io.data, LC_IO_MODE_SEEK, offset, LC_IO_SEEK_SET, &len);
		if (LC_IO_ERR_NONE != err)
			return err;

		err = io.proc(LC_ARGS_PASS, io.data, LC_IO_MODE_READ, n, buffer, written);
		if (LC_IO_ERR_NONE != err)
			return err;

		err = io.proc(LC_ARGS_PASS, io.data, LC_IO_MODE_SEEK, old_pos, LC_IO_SEEK_SET, &len);
	}

	return err;
}

static inline enum lc_io_error
lc_io_read_slice_at_(
        LC_ARGS_DECL,
        struct lc_io io,
        struct lc_mut_bslice slice,
        lc_io_off offset,
        lc_io_len *written
)
{
	enum lc_io_error err;
	err = io.proc(LC_ARGS_PASS, io.data, LC_IO_MODE_READ_AT, slice.size, slice.data, offset, written);

	if (LC_IO_ERR_NOT_IMPLEMENTED == err) {
		lc_io_pos old_pos = 0;
		lc_io_len len     = 0;

		err = io.proc(LC_ARGS_PASS, io.data, LC_IO_MODE_SEEK, 0, LC_IO_SEEK_CUR, &old_pos);
		if (LC_IO_ERR_NOT_IMPLEMENTED == err)
			return err;

		err = io.proc(LC_ARGS_PASS, io.data, LC_IO_MODE_SEEK, offset, LC_IO_SEEK_SET, &len);
		if (LC_IO_ERR_NONE != err)
			return err;

		err = io.proc(LC_ARGS_PASS, io.data, LC_IO_MODE_READ, slice.size, slice.data, written);
		if (LC_IO_ERR_NONE != err)
			return err;

		err = io.proc(LC_ARGS_PASS, io.data, LC_IO_MODE_SEEK, old_pos, LC_IO_SEEK_SET, &len);
	}

	return err;
}

LC_NODISCARD
static inline u8
lc_io_read_u8_(LC_ARGS_DECL, struct lc_io io, enum lc_io_error *err)
{
	u8 c          = 0;
	lc_io_len len = 0;
	*err          = io.proc(LC_ARGS_PASS, io.data, LC_IO_MODE_READ, sizeof c, &c, &len);
	return c;
}

LC_NODISCARD
static inline struct lc_sv
lc_io_read_line_(LC_ARGS_DECL, struct lc_io io, size_t n, char buffer[restrict n], enum lc_io_error *err)
{
	size_t i;

	for (i = 0; i < n; i++) {
		const char c = lc_io_read_u8_(LC_ARGS_CALL, io, err);
		switch (c) {
		// case 0x00: goto done;
		case 0x0A: goto done;
		default:
			if (LC_IO_ERR_EOF == *err)
				goto done;

			buffer[i] = c;
		}
	}
done:

	if (i < n)
		buffer[i] = 0;

	return sv(i, buffer);
}

static inline enum lc_io_error
lc_io_write_(LC_ARGS_DECL, struct lc_io io, size_t n, const void *buffer, lc_io_len *written)
{
	return io.proc(LC_ARGS_PASS, io.data, LC_IO_MODE_WRITE, n, buffer, written);
}

static inline enum lc_io_error
lc_io_write_slice_(LC_ARGS_DECL, struct lc_io io, struct lc_bslice slice, lc_io_len *written)
{
	return io.proc(LC_ARGS_PASS, io.data, LC_IO_MODE_WRITE, slice.size, slice.data, written);
}

static inline enum lc_io_error
lc_io_write_at_(
        LC_ARGS_DECL,
        struct lc_io io,
        size_t n,
        const void *restrict buffer,
        lc_io_off offset,
        lc_io_len *written
)
{
	enum lc_io_error err =
	        io.proc(LC_ARGS_PASS, io.data, LC_IO_MODE_WRITE_AT, n, buffer, offset, written);

	if (LC_IO_ERR_NOT_IMPLEMENTED == err) {
		lc_io_pos old_pos = 0;
		lc_io_len len     = 0;

		err = io.proc(LC_ARGS_PASS, io.data, LC_IO_MODE_SEEK, 0, LC_IO_SEEK_CUR, &old_pos);
		if (LC_IO_ERR_NONE != err)
			return err;


		err = io.proc(LC_ARGS_PASS, io.data, LC_IO_MODE_SEEK, offset, LC_IO_SEEK_SET, &len);
		if (LC_IO_ERR_NONE != err)
			return err;

		err = io.proc(LC_ARGS_PASS, io.data, LC_IO_MODE_WRITE, n, buffer, written);
		if (LC_IO_ERR_NONE != err)
			return err;

		err = io.proc(LC_ARGS_PASS, io.data, LC_IO_MODE_SEEK, old_pos, LC_IO_SEEK_SET, &len);
		if (LC_IO_ERR_NONE != err)
			return err;
	}

	return err;
}

static inline enum lc_io_error
lc_io_write_u8_(LC_ARGS_DECL, struct lc_io io, u8 byte)
{
	lc_io_len len = 0;
	return io.proc(LC_ARGS_PASS, io.data, LC_IO_MODE_WRITE, sizeof byte, &byte, &len);
}

static inline enum lc_io_error
lc_io_seek_(LC_ARGS_DECL, struct lc_io io, lc_io_off offset, enum lc_io_seek_mode whence, lc_io_pos *pos)
{
	return io.proc(LC_ARGS_PASS, io.data, LC_IO_MODE_SEEK, offset, whence, pos);
}

static inline enum lc_io_error
lc_io_tell_(LC_ARGS_DECL, struct lc_io io, lc_io_pos *pos)
{
	return io.proc(LC_ARGS_PASS, io.data, LC_IO_MODE_TELL, pos);
}

static inline enum lc_io_error
lc_io_size_(LC_ARGS_DECL, struct lc_io io, size_t *size)
{
	return io.proc(LC_ARGS_PASS, io.data, LC_IO_MODE_SIZE, size);
}

static inline enum lc_io_error
lc_io_destroy_(LC_ARGS_DECL, struct lc_io io)
{
	return io.proc(LC_ARGS_PASS, io.data, LC_IO_MODE_DESTROY);
}

#define lc_io_close(...)         LC_ARGS_WARP(lc_io_close_, __VA_ARGS__)
#define lc_io_flush(...)         LC_ARGS_WARP(lc_io_flush_, __VA_ARGS__)
#define lc_io_read(...)          LC_ARGS_WARP(lc_io_read_, __VA_ARGS__)
#define lc_io_read_slice(...)    LC_ARGS_WARP(lc_io_read_slice_, __VA_ARGS__)
#define lc_io_read_at(...)       LC_ARGS_WARP(lc_io_read_at_, __VA_ARGS__)
#define lc_io_read_slice_at(...) LC_ARGS_WARP(lc_io_read_slice_at_, __VA_ARGS__)
#define lc_io_read_u8(...)       LC_ARGS_WARP(lc_io_read_u8_, __VA_ARGS__)
#define lc_io_read_line(...)     LC_ARGS_WARP(lc_io_read_line_, __VA_ARGS__)
#define lc_io_write(...)         LC_ARGS_WARP(lc_io_write_, __VA_ARGS__)
#define lc_io_write_slice(...)   LC_ARGS_WARP(lc_io_write_slice_, __VA_ARGS__)
#define lc_io_write_at(...)      LC_ARGS_WARP(lc_io_write_at_, __VA_ARGS__)
#define lc_io_write_u8(...)      LC_ARGS_WARP(lc_io_write_u8_, __VA_ARGS__)
#define lc_io_seek(...)          LC_ARGS_WARP(lc_io_seek_, __VA_ARGS__)
#define lc_io_tell(...)          LC_ARGS_WARP(lc_io_tell_, __VA_ARGS__)
#define lc_io_size(...)          LC_ARGS_WARP(lc_io_size_, __VA_ARGS__)
#define lc_io_destroy(...)       LC_ARGS_WARP(lc_io_destroy_, __VA_ARGS__)

////////////////////////////////////////////////////////////////////////////////
/// Input/Output implementation                                     [LC_XIH] ///
////////////////////////////////////////////////////////////////////////////////


#define LC_IO_IMPL_CLOSE_BEGIN(args) case LC_IO_MODE_CLOSE:
#define LC_IO_IMPL_CLOSE_END

#define LC_IO_IMPL_FLUSH_BEGIN(args) case LC_IO_MODE_FLUSH:
#define LC_IO_IMPL_FLUSH_END

#define LC_IO_IMPL_READ_BEGIN(args, n, buffer, written)                                                      \
	case LC_IO_MODE_READ:                                                                                \
		do {                                                                                         \
			size_t n            = va_arg(args, size_t);                                            \
			void *buffer       = va_arg(args, void *);                                           \
			lc_io_len *written = va_arg(args, lc_io_len *);
#define LC_IO_IMPL_READ_END                                                                                  \
	}                                                                                                    \
	while (false)                                                                                        \
		;                                                                                            \
	break

#define LC_IO_IMPL_READ_AT_BEGIN(args, n, buffer, offset, written)                                           \
	case LC_IO_MODE_READ_AT:                                                                             \
		do {                                                                                         \
			size_t n            = va_arg(args, size_t);                                            \
			void *buffer       = va_arg(args, void *);                                           \
			lc_io_off offset   = va_arg(args, lc_io_off);                                        \
			lc_io_len *written = va_arg(args, lc_io_len *);

#define LC_IO_IMPL_READ_AT_END                                                                               \
	}                                                                                                    \
	while (false)                                                                                        \
		;                                                                                            \
	break;

#define LC_IO_IMPL_WRITE_BEGIN(args, n, buffer, written)                                                     \
	case LC_IO_MODE_WRITE:                                                                               \
		do {                                                                                         \
			size_t n            = va_arg(args, size_t);                                            \
			void *buffer       = va_arg(args, void *);                                           \
			lc_io_len *written = va_arg(args, lc_io_len *);
#define LC_IO_IMPL_WRITE_END                                                                                 \
	}                                                                                                    \
	while (false)                                                                                        \
		;                                                                                            \
	break

#define LC_IO_IMPL_WRITE_AT_BEGIN(args, n, buffer, offset, written)                                          \
	case LC_IO_MODE_WRITE_AT:                                                                            \
		do {                                                                                         \
			size_t n            = va_arg(args, size_t);                                            \
			void *buffer       = va_arg(args, void *);                                           \
			lc_io_off offset   = va_arg(args, lc_io_off);                                        \
			lc_io_len *written = va_arg(args, lc_io_len *);
#define LC_IO_IMPL_WRITE_AT_END                                                                              \
	}                                                                                                    \
	while (false)                                                                                        \
		;                                                                                            \
	break

#define LC_IO_IMPL_SEEK_BEGIN(args, offset, origin, pos)                                                     \
	case LC_IO_MODE_SEEK:                                                                                \
		do {                                                                                         \
			lc_io_off offset            = va_arg(args, lc_io_off);                               \
			enum lc_io_seek_mode origin = va_arg(args, enum lc_io_seek_mode);                    \
			lc_io_pos *pos              = va_arg(args, lc_io_pos *);

#define LC_IO_IMPL_SEEK_END                                                                                  \
	}                                                                                                    \
	while (false)                                                                                        \
		;                                                                                            \
	break

#define LC_IO_IMPL_TELL_BEGIN(args, pos)                                                                     \
	case LC_IO_MODE_TELL: do { lc_io_pos *pos = va_arg(args, lc_io_pos *);
#define LC_IO_IMPL_TELL_END                                                                                  \
	}                                                                                                    \
	while (false)                                                                                        \
		;                                                                                            \
	break

#define LC_IO_IMPL_SIZE_BEGIN(args, size)                                                                    \
	case LC_IO_MODE_SIZE: do { size_t *size = va_arg(args, size_t *);
#define LC_IO_IMPL_SIZE_END                                                                                  \
	}                                                                                                    \
	while (false)                                                                                        \
		;                                                                                            \
	break

#define LC_IO_IMPL_QUERY_BEGIN(args, modes)                                                                  \
	case LC_IO_MODE_QUERY: do { enum lc_io_mode *modes = va_arg(args, enum lc_io_mode *);
#define LC_IO_IMPL_QUERY_END                                                                                 \
	}                                                                                                    \
	while (false)                                                                                        \
		;                                                                                            \
	break

#ifndef LC_NO_IMPLEMENTATION

extern enum lc_io_error lc_io_stdfile_impl(LC_ARGS_DECL, void *data, enum lc_io_mode mode, ...)
#ifndef LC_IMPLEMENTATION
        ;
#else
{
	type_assert(lc_io_stdfile_impl, typeof_member(struct lc_io, proc));

	FILE *file = data;

	va_list args;
	va_start(args, mode);

	switch (mode) {
	case LC_IO_MODE_INIT:  break;
	case LC_IO_MODE_CLOSE: fclose(file); break;
	case LC_IO_MODE_FLUSH: fflush(file); break;
	LC_IO_IMPL_READ_BEGIN(args, n, buffer, written)
	{
		if (n == (*written = fread(buffer, 1, n, file)))
			break;

		if (feof(file))
			goto eof;

		goto err;
	}
	LC_IO_IMPL_READ_END;
	case LC_IO_MODE_READ_AT: goto not_implemented;
	LC_IO_IMPL_WRITE_BEGIN(args, n, buffer, written)
	{
		*written = fwrite(buffer, 1, n, file);
		break;
	}
	LC_IO_IMPL_WRITE_END;
	case LC_IO_MODE_WRITE_AT: goto not_implemented;
	LC_IO_IMPL_SEEK_BEGIN(args, offset, origin, pos)
	{
		lc_log_debug("seek: off %ld whence %d", offset, origin);
		*pos = ftell(file);
		if (fseek(file, offset, origin))
			goto err;
		*pos = ftell(file);
		break;
	}
	LC_IO_IMPL_SEEK_END;
	LC_IO_IMPL_TELL_BEGIN(args, pos)
	{
		*pos = ftell(file);
		break;
	}
	LC_IO_IMPL_TELL_END;
	LC_IO_IMPL_SIZE_BEGIN(args, size)
	{
		fpos_t pos;
		fgetpos(file, &pos);
		fseek(file, 0, SEEK_END);
		*size = ftell(file);
		fsetpos(file, &pos);
		break;
	}
	LC_IO_IMPL_SIZE_END;
	case LC_IO_MODE_DESTROY: goto not_implemented;

	LC_IO_IMPL_QUERY_BEGIN(args, modes)
	{

		*modes  = 0;
		*modes |= 1 << LC_IO_MODE_CLOSE;
		*modes |= 1 << LC_IO_MODE_FLUSH;
		*modes |= 1 << LC_IO_MODE_READ;
		// *modes |= 1 << LC_IO_MODE_READ_AT;
		*modes |= 1 << LC_IO_MODE_WRITE;
		// *modes |= 1 << LC_IO_MODE_WRITE_AT;
		*modes |= 1 << LC_IO_MODE_SEEK;
		*modes |= 1 << LC_IO_MODE_TELL;
		*modes |= 1 << LC_IO_MODE_SIZE;
		// *modes |= 1 << LC_IO_MODE_DESTROY;
		*modes |= 1 << LC_IO_MODE_QUERY;

		break;
	}
	LC_IO_IMPL_QUERY_END;
	}

	va_end(args);

	return LC_IO_ERR_NONE;

not_implemented:

	va_end(args);

	return LC_IO_ERR_NOT_IMPLEMENTED;

eof:
	va_end(args);
	return LC_IO_ERR_EOF;

err:
	va_end(args);

	switch (errno) {
	case EINVAL: return LC_IO_ERR_INVALID_ARG;

	default:     return LC_IO_ERR_UNKNOWN;
	}

	lc_unreachable();
}
#endif


LC_NODISCARD
static inline struct lc_io
lc_io_from_stdfile(FILE *file)
{
	return (struct lc_io){
	        .proc = lc_io_stdfile_impl,
	        .data = file,
	};
}

#endif

#define lc_io_cursor(type)                                                                                   \
	struct {                                                                                             \
		lc_io_pos offset;                                                                            \
		type data;                                                                                   \
	}

#define lc_io_cursor_c(type, ...) ((lc_io_cursor(type)){__VA_ARGS__})

#ifdef LC_IMPLEMENTATION

#define LC_IO_IMPL_SLICE_READ(slice, cursor, err_eof)                                                        \
	LC_IO_IMPL_READ_BEGIN(args, n, buffer, written)                                                      \
	{                                                                                                    \
		static_assert(                                                                               \
		        type_equal(*slice, struct lc_mut_bslice) | type_equal(*slice, struct lc_bslice),     \
		        "slice must be a pointer to a byte slice"                                            \
		);                                                                                           \
		if (1 == n && cursor->offset < slice->size) { \
			*(u8 *)buffer = slice->data[cursor->offset]; \
			cursor->offset += *written = 1; \
			break; \
		} \
                                                                                                             \
		const struct lc_bslice read_slice =                                                          \
		        lc_bslice_subslice(*(struct lc_bslice *)slice, cursor->offset, n);                   \
		const size_t to_write = MIN(n, read_slice.size);                                              \
                                                                                                             \
		if (0 == to_write)                                                                           \
			goto err_eof;                                                                        \
		else if (1 == to_write)                                                                      \
			*(u8 *)buffer = *(u8 *)read_slice.data;                                              \
		else                                                                                         \
			memcpy(buffer, read_slice.data, to_write);                                           \
		cursor->offset += *written = to_write;                                                       \
                                                                                                             \
		break;                                                                                       \
	}                                                                                                    \
	LC_IO_IMPL_READ_END

#define LC_IO_IMPL_SLICE_READ_AT(slice, cursor, err_eof)                                                     \
	LC_IO_IMPL_READ_AT_BEGIN(args, n, buffer, offset, written)                                           \
	{                                                                                                    \
		static_assert(                                                                               \
		        type_equal(*slice, struct lc_mut_bslice) | type_equal(*slice, struct lc_bslice),     \
		        "slice must be a pointer to a byte slice"                                            \
		);                                                                                           \
		if (1 == n && cursor->offset < slice->size) { \
			*(u8 *)buffer = slice->data[cursor->offset]; \
			cursor->offset += *written = 1; \
			break; \
		} \
                                                                                                             \
		const struct lc_bslice read_slice =                                                          \
		        lc_bslice_subslice((*(struct lc_bslice *)slice), offset, n);                         \
		const size_t to_write = MIN(n, read_slice.size);                                              \
                                                                                                             \
		if (0 == to_write)                                                                           \
			goto err_eof;                                                                        \
		else if (1 == to_write)                                                                      \
			*(u8 *)buffer = *(u8 *)read_slice.data;                                              \
		else                                                                                         \
			memcpy(buffer, read_slice.data, to_write);                                           \
		*written = to_write;                                                                         \
                                                                                                             \
		break;                                                                                       \
	}                                                                                                    \
	LC_IO_IMPL_READ_AT_END

#define LC_IO_IMPL_CURSOR_SEEK(cursor, bound)                                                                \
	LC_IO_IMPL_SEEK_BEGIN(args, offset, origin, pos)                                                     \
	{                                                                                                    \
		switch (origin) {                                                                            \
		case LC_IO_SEEK_SET:                                                                         \
		{                                                                                            \
			lc_log_todo_once("Handle on out of bound...");                                       \
                                                                                                             \
			*pos = cursor->offset = offset;                                                      \
			break;                                                                               \
		}                                                                                            \
		case LC_IO_SEEK_CUR:                                                                         \
		{                                                                                            \
			lc_log_todo_once("Handle on out of bound...");                                       \
                                                                                                             \
			*pos = cursor->offset += offset;                                                     \
			break;                                                                               \
		}                                                                                            \
		case LC_IO_SEEK_END:                                                                         \
		{                                                                                            \
			lc_log_todo_once("Handle on out of bound...");                                       \
                                                                                                             \
			*pos = cursor->offset = bound - offset;                                              \
			break;                                                                               \
		}                                                                                            \
		default: lc_unreachable();                                                                   \
		}                                                                                            \
                                                                                                             \
		break;                                                                                       \
	}                                                                                                    \
	LC_IO_IMPL_SEEK_END


#endif

#define LC_IO_IMPL_EXPR_TELL(offset)                                                                         \
	LC_IO_IMPL_TELL_BEGIN(args, pos)                                                                     \
	{                                                                                                    \
		*pos = offset;                                                                               \
		break;                                                                                       \
	}                                                                                                    \
	LC_IO_IMPL_TELL_END

#define LC_IO_IMPL_EXPR_SIZE(bound)                                                                          \
	LC_IO_IMPL_SIZE_BEGIN(args, size)                                                                    \
	{                                                                                                    \
		*size = bound;                                                                               \
		break;                                                                                       \
	}                                                                                                    \
	LC_IO_IMPL_SIZE_END


#ifndef LC_NO_IMPLEMENTATION

extern enum lc_io_error lc_io_slice_impl(LC_ARGS_DECL, void *data, enum lc_io_mode mode, ...)
#ifndef LC_IMPLEMENTATION
        ;
#else
{
	type_assert(lc_io_slice_impl, typeof_member(struct lc_io, proc));

	lc_io_cursor(struct lc_bslice) *cursor = data;

	const struct lc_bslice *slice = &cursor->data;

	lc_log_todo_once("pos parameter should be i64");

	va_list args;
	va_start(args, mode);

	switch (mode) {
	case LC_IO_MODE_INIT:
	{
		struct lc_bslice slice = va_arg(args, struct lc_bslice);
		enum lc_allocator_error err;
		void **ret = data;


		cursor = *ret  = lc_mem_alloc(&err, sizeof *cursor);
		cursor->offset = 0;
		cursor->data   = slice;

		break;
	}

	case LC_IO_MODE_CLOSE:
	{
		enum lc_allocator_error err;
		lc_mem_free_sized(&err, cursor, sizeof *cursor);
		break;
	}
	case LC_IO_MODE_FLUSH:    break;

	LC_IO_IMPL_SLICE_READ(slice, cursor, err_eof);
	LC_IO_IMPL_SLICE_READ_AT(slice, cursor, err_eof);

	case LC_IO_MODE_WRITE:    goto not_implemented;
	case LC_IO_MODE_WRITE_AT: goto not_implemented;

	LC_IO_IMPL_CURSOR_SEEK(cursor, slice->size);

	LC_IO_IMPL_EXPR_TELL(cursor->offset);
	LC_IO_IMPL_EXPR_SIZE(slice->size);

	case LC_IO_MODE_DESTROY:  goto not_implemented;

	LC_IO_IMPL_QUERY_BEGIN(args, modes)
	{

		*modes  = 0;
		*modes |= 1 << LC_IO_MODE_CLOSE;
		*modes |= 1 << LC_IO_MODE_FLUSH;
		*modes |= 1 << LC_IO_MODE_READ;
		*modes |= 1 << LC_IO_MODE_READ_AT;
		// *modes |= 1 << LC_IO_MODE_WRITE;
		// *modes |= 1 << LC_IO_MODE_WRITE_AT;
		*modes |= 1 << LC_IO_MODE_SEEK;
		*modes |= 1 << LC_IO_MODE_TELL;
		*modes |= 1 << LC_IO_MODE_SIZE;
		// *modes |= 1 << LC_IO_MODE_DESTROY;
		*modes |= 1 << LC_IO_MODE_QUERY;

		break;
	}
	LC_IO_IMPL_QUERY_END;
	}

	va_end(args);
	return LC_IO_ERR_NONE;

err_eof:

	va_end(args);
	return LC_IO_ERR_EOF;

not_implemented:

	va_end(args);
	return LC_IO_ERR_NOT_IMPLEMENTED;
}
#endif

#endif

#ifndef LC_NO_IMPLEMENTATION

extern enum lc_io_error lc_io_mut_slice_impl(LC_ARGS_DECL, void *data, enum lc_io_mode mode, ...)
#ifndef LC_IMPLEMENTATION
        ;
#else
{
	type_assert(lc_io_mut_slice_impl, typeof_member(struct lc_io, proc));

	lc_io_cursor(struct lc_mut_bslice) *cursor = data;

	const struct lc_mut_bslice *slice = &cursor->data;

	lc_log_todo_once("pos parameter should be i64");

	va_list args;
	va_start(args, mode);

	switch (mode) {
	case LC_IO_MODE_INIT:  lc_todo("");
	case LC_IO_MODE_CLOSE: goto not_implemented;
	case LC_IO_MODE_FLUSH: break;

	LC_IO_IMPL_SLICE_READ(slice, cursor, err_eof);
	LC_IO_IMPL_SLICE_READ_AT(slice, cursor, err_eof);


	LC_IO_IMPL_WRITE_BEGIN(args, n, buffer, written)
	{
		const struct lc_mut_bslice write_slice = lc_bslice_mut_subslice(*slice, cursor->offset, n);
		const size_t to_write                   = MIN(n, write_slice.size);

		if (0 == to_write)
			goto err_eof;
		else if (1 == to_write)
			*(u8 *)write_slice.data = *(u8 *)buffer;
		else
			memcpy(write_slice.data, buffer, to_write);
		cursor->offset += *written = to_write;

		break;
	}
	LC_IO_IMPL_WRITE_END;
	LC_IO_IMPL_WRITE_AT_BEGIN(args, n, buffer, offset, written)
	{

		const struct lc_mut_bslice write_slice = lc_bslice_mut_subslice(*slice, offset, n);
		const size_t to_write                   = MIN(n, write_slice.size);

		if (0 == to_write)
			goto err_eof;
		else if (1 == to_write)
			*(u8 *)write_slice.data = *(u8 *)buffer;
		else
			memcpy(write_slice.data, buffer, to_write);
		*written = to_write;

		break;
	}
	LC_IO_IMPL_WRITE_AT_END;

	LC_IO_IMPL_CURSOR_SEEK(cursor, slice->size);

	LC_IO_IMPL_EXPR_TELL(cursor->offset);
	LC_IO_IMPL_EXPR_SIZE(slice->size);

	case LC_IO_MODE_DESTROY: goto not_implemented;
	case LC_IO_MODE_QUERY:
	{
		enum lc_io_mode *modes = va_arg(args, enum lc_io_mode *);

		*modes  = 0;
		*modes |= 1 << LC_IO_MODE_CLOSE;
		*modes |= 1 << LC_IO_MODE_FLUSH;
		*modes |= 1 << LC_IO_MODE_READ;
		*modes |= 1 << LC_IO_MODE_READ_AT;
		*modes |= 1 << LC_IO_MODE_WRITE;
		*modes |= 1 << LC_IO_MODE_WRITE_AT;
		*modes |= 1 << LC_IO_MODE_SEEK;
		*modes |= 1 << LC_IO_MODE_TELL;
		*modes |= 1 << LC_IO_MODE_SIZE;
		// *modes |= 1 << LC_IO_MODE_DESTROY;
		*modes |= 1 << LC_IO_MODE_QUERY;

		break;
	}
		lc_unreachable();
	}

	va_end(args);
	return LC_IO_ERR_NONE;

err_eof:

	va_end(args);
	return LC_IO_ERR_EOF;

not_implemented:

	va_end(args);
	return LC_IO_ERR_NOT_IMPLEMENTED;
}
#endif

#endif

#if defined(LC_PLATFORM_UNIX) && defined(LC_PLATFORM_POSIX)
#include <sys/stat.h>
#include <unistd.h>

#ifndef LC_NO_IMPLEMENTATION

extern enum lc_io_error lc_io_unistd_impl(LC_ARGS_DECL, void *data, enum lc_io_mode mode, ...)
#ifndef LC_IMPLEMENTATION
        ;
#else
{
	type_assert(lc_io_unistd_impl, typeof_member(struct lc_io, proc));

	const int fd = (intptr_t)data;

	va_list args;
	va_start(args, mode);


	switch (mode) {
	case LC_IO_MODE_INIT: goto not_implemented;
	case LC_IO_MODE_CLOSE:
		if (close(fd))
			goto err;
		break;
	case LC_IO_MODE_FLUSH:
		if (fsync(fd))
			goto err;
		break;
	LC_IO_IMPL_READ_BEGIN(args, n, buffer, written)
	{
		*written = read(fd, buffer, n);
		break;
	}
	LC_IO_IMPL_READ_END;
	LC_IO_IMPL_READ_AT_BEGIN(args, n, buffer, offset, written)
	{
		*written = pread(fd, buffer, n, offset);
		break;
	}
	LC_IO_IMPL_READ_AT_END;
	LC_IO_IMPL_WRITE_BEGIN(args, n, buffer, written)
	{
		*written = write(fd, buffer, n);
		break;
	}
	LC_IO_IMPL_WRITE_END;
	LC_IO_IMPL_WRITE_AT_BEGIN(args, n, buffer, offset, written)
	{
		*written = pwrite(fd, buffer, n, offset);
		break;
	}
	LC_IO_IMPL_WRITE_AT_END;
	LC_IO_IMPL_SEEK_BEGIN(args, offset, origin, pos)
	{
		*pos = lseek(fd, offset, origin);
		break;
	}
	LC_IO_IMPL_SEEK_END;
	LC_IO_IMPL_TELL_BEGIN(args, pos)
	{
		*pos = lseek(fd, 0, LC_IO_SEEK_CUR);
		break;
	}
	LC_IO_IMPL_TELL_END;
	LC_IO_IMPL_SIZE_BEGIN(args, size)
	{
		struct stat st;

		if (fstat(fd, &st))
			goto err;

		*size = st.st_size;
		break;
	}
	LC_IO_IMPL_SIZE_END;
	case LC_IO_MODE_DESTROY: goto not_implemented;
	case LC_IO_MODE_QUERY:
	{
		enum lc_io_mode *modes = va_arg(args, enum lc_io_mode *);

		*modes  = 0;
		*modes |= 1 << LC_IO_MODE_CLOSE;
		*modes |= 1 << LC_IO_MODE_FLUSH;
		*modes |= 1 << LC_IO_MODE_READ;
		*modes |= 1 << LC_IO_MODE_READ_AT;
		*modes |= 1 << LC_IO_MODE_WRITE;
		*modes |= 1 << LC_IO_MODE_WRITE_AT;
		*modes |= 1 << LC_IO_MODE_SEEK;
		*modes |= 1 << LC_IO_MODE_TELL;
		*modes |= 1 << LC_IO_MODE_SIZE;
		// *modes |= 1 << LC_IO_MODE_DESTROY;
		*modes |= 1 << LC_IO_MODE_QUERY;

		break;
	}
	}

	va_end(args);

	return LC_IO_ERR_NONE;

not_implemented:
	va_end(args);

	return LC_IO_ERR_NOT_IMPLEMENTED;

err:
	va_end(args);

	return LC_IO_ERR_UNKNOWN;
}
#endif


LC_NODISCARD
static inline struct lc_io
lc_io_from_unistd(int fd)
{
	return (struct lc_io){
	        .proc = lc_io_unistd_impl,
	        .data = (void *)(intptr_t)fd,
	};
}
#endif

#endif

#ifndef LC_NO_IMPLEMENTATION

extern enum lc_io_error lc_io_ring_buffer_impl(LC_ARGS_DECL, void *restrict data, enum lc_io_mode cmd, ...)
#ifndef LC_IMPLEMENTATION
        ;
#else
{
	type_assert(lc_io_ring_buffer_impl, typeof_member(struct lc_io, proc));

	lc_io_cursor(struct lc_mut_bslice) *cursor = data;

	const struct lc_mut_bslice *slice = &cursor->data;

	va_list args;
	va_start(args, cmd);


	switch (cmd) {
	case LC_IO_MODE_INIT:  lc_todo("");
	case LC_IO_MODE_CLOSE: goto not_implemented;
	case LC_IO_MODE_FLUSH: goto not_implemented;

	LC_IO_IMPL_READ_BEGIN(args, n, buffer, written)
	{
		u8 *out          = buffer;
		const size_t len = MIN(n, slice->size - cursor->offset);
		if (1 == len) {
			*out = slice->data[cursor->offset];
		} else {
			memcpy(out, &slice->data[cursor->offset], len);
		}

		out            += len;
		cursor->offset += len;
		n              -= len;

		if (cursor->offset == slice->size) {
			if (1 == n) {
				*out = slice->data[0];
			} else {
				memcpy(out, &slice->data[0], n);
			}
			out            += n;
			cursor->offset  = n;
		}

		*written = out - (u8 *)buffer;
		goto ret;
	}
	LC_IO_IMPL_READ_END;

	LC_IO_IMPL_READ_AT_BEGIN(args, n, buffer, offset, written)
	{
		lc_log_todo_once("Use memcpy instead");

		u8 *out = buffer;
		for (size_t i = 0; i < n; i++)
			*out++ = slice->data[offset + i % slice->size];

		*written = out - (u8 *)buffer;
		goto ret;
	}
	LC_IO_IMPL_READ_AT_END;

	LC_IO_IMPL_WRITE_BEGIN(args, n, buffer, written)
	{
		const u8 *in = buffer;

		const size_t len = MIN(n, slice->size - cursor->offset);
		if (1 == len) {
			slice->data[cursor->offset] = *in;
		} else {
			memcpy(&slice->data[cursor->offset], in, len);
		}

		in             += len;
		cursor->offset += len;
		n              -= len;


		if (cursor->offset == slice->size) {
			memcpy(&slice->data[0], in, n);
			in             += n;
			cursor->offset  = n;
		}
		*written = in - (u8 *)buffer;
		goto ret;
	}
	LC_IO_IMPL_WRITE_END;


	LC_IO_IMPL_WRITE_AT_BEGIN(args, n, buffer, offset, written)
	{
		lc_log_todo_once("Use memcpy instead");

		const u8 *in = buffer;

		for (size_t i = 0; i < n; i++)
			slice->data[offset + i % slice->size] = *in++;

		*written = in - (u8 *)buffer;
		goto ret;
	}
	LC_IO_IMPL_WRITE_AT_END;

	LC_IO_IMPL_SEEK_BEGIN(args, offset, origin, pos)
	{
		lc_log_todo_once("Seeking is kinda buggy (I think)");
		lc_log_todo_once("It was written sloppy/rushed so it was not proven correct");

		switch (origin) {
		default:             lc_unreachable();
		case LC_IO_SEEK_SET: *pos = cursor->offset = offset % slice->size; break;
		case LC_IO_SEEK_CUR:
			cursor->offset += slice->size + offset % slice->size;
			*pos = cursor->offset %= slice->size;
			break;
		case LC_IO_SEEK_END:

			if ((void)"offset does not make sense", 0 < offset)
				goto invalid_arg;

			offset = MIN(offset, (i64)slice->size);
			*pos = cursor->offset = slice->size + offset;
			break;
		}

		goto ret;
	}
	LC_IO_IMPL_SEEK_END;

	LC_IO_IMPL_EXPR_TELL(cursor->offset);
	LC_IO_IMPL_EXPR_SIZE(slice->size);

	case LC_IO_MODE_DESTROY: goto not_implemented;
	LC_IO_IMPL_QUERY_BEGIN(args, modes)
	{
		*modes = 0;
		// *modes |= 1 << LC_IO_MODE_CLOSE;
		// *modes |= 1 << LC_IO_MODE_FLUSH;
		*modes |= 1 << LC_IO_MODE_READ;
		*modes |= 1 << LC_IO_MODE_READ_AT;
		*modes |= 1 << LC_IO_MODE_WRITE;
		*modes |= 1 << LC_IO_MODE_WRITE_AT;
		*modes |= 1 << LC_IO_MODE_SEEK;
		*modes |= 1 << LC_IO_MODE_TELL;
		*modes |= 1 << LC_IO_MODE_SIZE;
		// *modes |= 1 << LC_IO_MODE_DESTROY;
		*modes |= 1 << LC_IO_MODE_QUERY;
	}
	LC_IO_IMPL_QUERY_END;
	}


ret:
	va_end(args);

	return LC_IO_ERR_NONE;

invalid_arg:
	va_end(args);

	return LC_IO_ERR_INVALID_ARG;

not_implemented:
	va_end(args);

	return LC_IO_ERR_NOT_IMPLEMENTED;
}
#endif

#endif

#ifndef LC_NO_IMPLEMENTATION

extern enum lc_io_error _lc_io_null_impl(LC_ARGS_DECL, void *restrict data, enum lc_io_mode cmd, ...)
#ifndef LC_IMPLEMENTATION
        ;
#else
{
	type_assert(_lc_io_null_impl, typeof_member(struct lc_io, proc));

	va_list args;
	va_start(args, cmd);


	switch (cmd) {
	case LC_IO_MODE_INIT:  goto not_implemented;
	case LC_IO_MODE_CLOSE: goto not_implemented;
	case LC_IO_MODE_FLUSH: goto not_implemented;

	LC_IO_IMPL_READ_BEGIN(args, n, buffer, written)
	{
		lc_memset(buffer, 0, *written = n);
		goto ret;
	}
	LC_IO_IMPL_READ_END;

	LC_IO_IMPL_READ_AT_BEGIN(args, n, buffer, offset LC_UNUSED, written)
	{
		lc_memset(buffer, 0, *written = n);
		goto ret;
	}
	LC_IO_IMPL_READ_AT_END;

	LC_IO_IMPL_WRITE_BEGIN(args, n, buffer LC_UNUSED, written)
	{
		*written = n;
		goto ret;
	}
	LC_IO_IMPL_WRITE_END;


	LC_IO_IMPL_WRITE_AT_BEGIN(args, n, buffer LC_UNUSED, offset LC_UNUSED, written)
	{
		*written = n;
		goto ret;
	}
	LC_IO_IMPL_WRITE_AT_END;

	LC_IO_IMPL_SEEK_BEGIN(args, offset LC_UNUSED, origin LC_UNUSED, pos)
	{
		*pos = 0;
		goto ret;
	}
	LC_IO_IMPL_SEEK_END;

	LC_IO_IMPL_EXPR_TELL(0);
	LC_IO_IMPL_EXPR_SIZE(0);

	case LC_IO_MODE_DESTROY: goto not_implemented;
	LC_IO_IMPL_QUERY_BEGIN(args, modes)
	{
		*modes = 0;
		// *modes |= 1 << LC_IO_MODE_CLOSE;
		// *modes |= 1 << LC_IO_MODE_FLUSH;
		*modes |= 1 << LC_IO_MODE_READ;
		*modes |= 1 << LC_IO_MODE_READ_AT;
		*modes |= 1 << LC_IO_MODE_WRITE;
		*modes |= 1 << LC_IO_MODE_WRITE_AT;
		*modes |= 1 << LC_IO_MODE_SEEK;
		*modes |= 1 << LC_IO_MODE_TELL;
		*modes |= 1 << LC_IO_MODE_SIZE;
		// *modes |= 1 << LC_IO_MODE_DESTROY;
		*modes |= 1 << LC_IO_MODE_QUERY;
	}
	LC_IO_IMPL_QUERY_END;
	}


ret:
	va_end(args);

	return LC_IO_ERR_NONE;

not_implemented:
	va_end(args);

	return LC_IO_ERR_NOT_IMPLEMENTED;
}
#endif


LC_NODISCARD
static inline struct lc_io
lc_io_null(void)
{
	return (struct lc_io){
	        .proc = _lc_io_null_impl,
	};
}

#endif

#ifdef _GNU_SOURCE
#ifndef LC_NO_IMPLEMENTATION
#define src_loc lc_src_loc(0)

extern ssize_t _lc_io_stdfile_read(void *restrict cookie, char *restrict buf, size_t size)
#ifndef LC_IMPLEMENTATION
        ;
#else
{
	// type_assert(&_lc_io_stdfile_read, typeof_member);

	struct io_context {
		struct lc_ctx *ctx;

		struct lc_io *io;
	} *io_ctx          = cookie;
	struct lc_ctx *ctx = io_ctx->ctx;

	lc_io_len written = 0;
	(void)lc_io_read(*io_ctx->io, size, buf, &written);
	return written;
}
#endif

extern ssize_t _lc_io_stdfile_write(void *restrict cookie, const char *restrict buf, size_t size)
#ifndef LC_IMPLEMENTATION
        ;
#else
{
	// type_assert(_lc_io_stdfile_write, cookie_write_function_t);

	struct io_context {
		struct lc_ctx *ctx;

		struct lc_io *io;
	} *io_ctx          = cookie;
	struct lc_ctx *ctx = io_ctx->ctx;

	lc_io_len written = 0;
	(void)lc_io_write(*io_ctx->io, size, buf, &written);
	return written;
}
#endif

extern int _lc_io_stdfile_seek(void *restrict cookie, off_t *restrict offset, int whence)
#ifndef LC_IMPLEMENTATION
        ;
#else
{
	// type_assert(_lc_io_stdfile_seek, cookie_seek_function_t);

	struct io_context {
		struct lc_ctx *ctx;

		struct lc_io *io;
	} *io_ctx = cookie;

	struct lc_ctx *ctx = io_ctx->ctx;

	return lc_io_seek(*io_ctx->io, *offset, whence, (lc_io_pos *)offset);
}
#endif

extern int _lc_io_stdfile_close(void *cookie)
#ifndef LC_IMPLEMENTATION
        ;
#else
{
	struct io_context {
		struct lc_ctx *ctx;
		struct lc_io *io;
	} *io_ctx          = cookie;
	struct lc_ctx *ctx = io_ctx->ctx;

	(void)lc_io_close(*io_ctx->io);

	enum lc_allocator_error err = 0;
	lc_mem_free_(io_ctx->ctx->allocator, &err, io_ctx);

	if (0 != err)
		return EOF;

	return 0;
}
#endif

#undef src_loc
extern FILE *lc_io_to_stdfile(LC_ARGS_DECL, struct lc_io *io, const char *restrict modes)
#ifndef LC_IMPLEMENTATION
        ;
#else
{
	enum lc_allocator_error err;

	struct io_context {
		struct lc_ctx *ctx;
		struct lc_io *io;
	} *io_ctx = lc_mem_alloc(&err, sizeof(struct io_context));

	if (LC_ALLOCATOR_ERROR_NONE != err)
		return NULL;

	*io_ctx = (struct io_context){
	        .io  = io,
	        .ctx = ctx,
	};

	static const cookie_io_functions_t cb = {
	        .read  = _lc_io_stdfile_read,
	        .write = _lc_io_stdfile_write,
	        .seek  = _lc_io_stdfile_seek,
	        .close = _lc_io_stdfile_close,
	};

	return fopencookie(io_ctx, modes, cb);
}
#endif

#endif

#endif
#endif

////////////////////////////////////////////////////////////////////////////////
/// Vector                                                          [LC_XVC] ///
////////////////////////////////////////////////////////////////////////////////

struct lc_vector {
	struct lc_allocator allocator;
	size_t len;
	size_t cap;
	void *data;
};

LC_NODISCARD
static inline struct lc_vector
lc_vector_create_(struct lc_allocator allocator, size_t capacity, size_t item_size)
{
	capacity  = capacity ? capacity : 16;
	item_size = item_size ? item_size : 1;

	enum lc_allocator_error err = LC_ALLOCATOR_ERROR_NONE;
	return (struct lc_vector){
	        .allocator = allocator,
	        .cap       = capacity * item_size,
	        .data      = lc_mem_alloc__(lc_src_loc(0), allocator, &err, capacity * item_size),
	};
}

static inline void
lc_vector_destroy(struct lc_vector vector)
{
	enum lc_allocator_error err = LC_ALLOCATOR_ERROR_NONE;
	lc_mem_free_sized__(lc_src_loc(0), vector.allocator, &err, vector.data, vector.cap);
}

#define lc_vector_create(allocator, capacity, type) lc_vector_create_(allocator, capacity, sizeof(type))

static inline bool
lc_vector_append_(struct lc_vector *vector, enum lc_allocator_error *err, size_t item_size, const void *item)
{
	if (false == (vector->len + item_size < vector->cap)) {
		vector->data = lc_mem_realloc_sized__(
		        lc_src_loc(0),
		        vector->allocator,
		        err,
		        vector->data,
		        vector->cap,
		        vector->cap * 2
		);

		if (LC_ALLOCATOR_ERROR_NONE != *err)
			return false;

		vector->cap *= 2;
	}

	memcpy((u8 *)vector->data + vector->len, item, item_size);
	vector->len += item_size;

	return true;
}

LC_NODISCARD
static inline size_t
lc_vector_length_(const struct lc_vector *vector, size_t item_size)
{
	return vector->len / item_size;
}

#define lc_vector_append(vector, err, item) lc_vector_append_(vector, err, sizeof(item), &item)
#define lc_vector_length(vector, type)      lc_vector_length_(vector, sizeof(type))

LC_NODISCARD
static inline bool
lc_vector_is_empty(struct lc_vector vector)
{
	return 0 == vector.len;
}

////////////////////////////////////////////////////////////////////////////////
/// Strings                                                         [LC_XSS] ///
////////////////////////////////////////////////////////////////////////////////

struct lc_string {
	size_t len;
	const char *s;
};

#define string(...) ((struct lc_string){__VA_ARGS__})

LC_NODISCARD
static inline const char *
lc_string_end(struct lc_string str)
{
	return str.s + str.len;
}

#include <string.h>

LC_NODISCARD
static inline struct lc_string
lc_string_join_(
        LC_ARGS_DECL,
        struct lc_allocator allocator,
        char sep,
        size_t n,
        const struct lc_sv string_views[n]
)
{
	// Bytes to allocate
	size_t total_len = 0;
	for (size_t i = 0; i < n; i++)
		total_len += string_views[i].length + !!string_views[i].length;

	if (0 == total_len)
		return string(0);

	LC_ASSERT(
	        n,
	        "At this point, there should be at least one string because we "
	        "have to allocate"
	);

	enum lc_allocator_error err;

	char *const str = (char *)lc_mem_alloc_(allocator, &err, total_len);
	char *s         = str;
	for (size_t i = 0; i < n; i++) {
		if (lc_sv_is_empty(string_views[i]))
			continue;

		if (str != s)
			*s++ = sep;

		memcpy(s, string_views[i].s, string_views[i].length);
		s += string_views[i].length;
	}

	const size_t len = s - str;
	LC_ASSERT(len + 1 == total_len, "Length is always one short of allocation (nul byte)");

	str[len] = 0;
	return string(len, str);
}

#define lc_string_join(sep, ...)                                                                             \
	lc_string_join_(                                                                                     \
	        ctx,                                                                                         \
	        ctx->allocator,                                                                              \
	        sep,                                                                                         \
	        LEN(((struct lc_sv[]){__VA_ARGS__})),                                                        \
	        (struct lc_sv[]){__VA_ARGS__}                                                                \
	)

#define lc_string_join_t(sep, ...)                                                                           \
	lc_string_join_(                                                                                     \
	        ctx,                                                                                         \
	        ctx->temp_allocator,                                                                         \
	        sep,                                                                                         \
	        LEN(((struct lc_sv[]){__VA_ARGS__})),                                                        \
	        (struct lc_sv[]){__VA_ARGS__}                                                                \
	)

LC_NODISCARD
static inline struct lc_string
lc_string_clone_from_sv__(struct lc_allocator allocator, struct lc_source_loc src_loc, struct lc_sv sv)
{
	enum lc_allocator_error err = LC_ALLOCATOR_ERROR_NONE;
	return (struct lc_string){
	        .len = sv.length,
	        .s   = (const char *)
	                memcpy(lc_mem_alloc_zero__(src_loc, allocator, &err, sv.length + 1),
	                       (const void *)sv.s,
	                       sv.length),
	};
}

static inline void
lc_string_free__(struct lc_allocator allocator, struct lc_source_loc src_loc, struct lc_string str)
{
	enum lc_allocator_error err = LC_ALLOCATOR_ERROR_NONE;
	lc_mem_free_sized__(src_loc, allocator, &err, (void *)str.s, str.len + 1);
}

LC_NODISCARD
static inline struct lc_sv
lc_string_to_sv(struct lc_string str)
{
	return sv(str.len, str.s);
}

#define lc_string_clone_from_sv_(allocator, sv) lc_string_clone_from_sv__(allocator, lc_src_loc(0), sv)
#define lc_string_free_(allocator, str)         lc_string_free__(allocator, lc_src_loc(0), str)

#define lc_string_free(str)           lc_string_free_(ctx->allocator, str)
#define lc_string_free_t(str)         lc_string_free_(ctx->temp_allocator, str)
#define lc_string_clone_from_sv(sv)   lc_string_clone_from_sv_(ctx->allocator, sv)
#define lc_string_clone_from_sv_t(sv) lc_string_clone_from_sv_(ctx->temp_allocator, sv)

////////////////////////////////////////////////////////////////////////////////
/// Path                                                            [LC_XPH] ///
////////////////////////////////////////////////////////////////////////////////

#ifndef PATH_MAX
#define PATH_MAX 4096
#endif

LC_NODISCARD
static inline size_t
lc_path_join_(LC_ARGS_DECL, char path[restrict PATH_MAX], size_t n, const struct lc_sv string_views[n])
{
	// NOTE(larry): I'm not particularly proud of this function, maybe it can be simplified...

#define SEP '/'

	// Bytes to write (worst case)
	size_t total_len = 0;
	for (size_t i = 0; i < n; i++)
		total_len += string_views[i].length + !!string_views[i].length;

	if (0 == total_len) {
		path[0] = '.';
		path[1] = 0;
		return 1;
	}

	LC_ASSERT(
	        n,
	        "At this point, there should be at least one string because we "
	        "have to write"
	);
	LC_ASSERT(total_len < PATH_MAX, "C'mon this should never happen!");

	goto joining;
joining:;

	char *s = path;
	for (size_t i = 0; i < n; i++) {
		if (lc_sv_is_empty(string_views[i]))
			continue;

		if (path != s)
			*s++ = SEP;

		memcpy(s, string_views[i].s, string_views[i].length);
		s += string_views[i].length;
	}

	const size_t len = s - path;
	LC_ASSERT(
	        len + 1 == total_len,
	        "(At this point) Length is always one short of allocation (nul byte)"
	);
	path[len] = 0;

	goto normalize;
normalize:;

	const char *r = path;
	char *w       = path;

	const bool is_root = SEP == *r;
	char *const begin  = is_root ? ++r, ++w : w;

	enum
	{
		NORMAL,
		SEPERATOR,
		DOT_1,
		DOT_2,
	} state = is_root ? SEPERATOR : NORMAL;

	for (size_t i = is_root; i < len; i++) {
		const char c = *w = *r++;
		lc_log_debug("is_root %d state = %d, c = '%c', len = %ju", is_root, state, c, w - path);

		switch (c) {
		default:
		{
			switch (state) {
			case NORMAL:    state = NORMAL; break;
			case SEPERATOR: state = NORMAL; break;
			case DOT_1:     state = NORMAL; break;
			case DOT_2:     state = NORMAL; break;
			}
			w++;
			break;
		}
		case '.':
		{
			switch (state) {
			case NORMAL:    state = NORMAL; break;
			case SEPERATOR: state = DOT_1; break;
			case DOT_1:     state = DOT_2; break;
			case DOT_2:     state = NORMAL; break;
			}
			w++;
			break;
		}
		case SEP:
		{
			switch (state) {
			case NORMAL:
				state = SEPERATOR;
				w++;
				break;
			case SEPERATOR: state = SEPERATOR; break;
			case DOT_1:
			{
				w--;
				state = SEPERATOR;
				break;
			}
			case DOT_2:
			{
				const size_t len = w - begin;
				if (len < LEN("..")) {
					w = begin;
					break;
				}
				w -= LEN("../") - 1;
				for (size_t i = 2; i < len; i++)
					if (SEP == *--w)
						break;

				LC_ASSERT(begin <= w, "");

				state = SEPERATOR;
				w++;
				break;
			}
			}
			break;
		}
		case 0: goto done;
		}
	}

done:
	switch (state) {
	default:        break;
	case SEPERATOR: w--; break;
	case DOT_1:
	{
		w--, w--;
		break;
	}
	case DOT_2:
	{
		const size_t len = w - begin;
		if (len < LEN("..")) {
			w  = begin;
			*w = 0;
			break;
		}
		w -= LEN("../") - 1;
		for (size_t i = 2; i < len; i++)
			if (SEP == *--w)
				break;
		break;
	}
	}
	*w = 0;

	const size_t written = w - path;

	if (0 == written) {

		if (is_root) {
			path[0] = '/';
			path[1] = 0;
			return 1;
		} else {
			path[0] = '.';
			path[1] = 0;
			return 1;
		}
	}

	return written;

#undef SEP
}

#define lc_path_join(path, ...)                                                                              \
	lc_path_join_(LC_ARGS_CALL, path, LEN(((struct lc_sv[]){__VA_ARGS__})), (struct lc_sv[]){__VA_ARGS__})

////////////////////////////////////////////////////////////////////////////////
/// Dynamic library                                                 [LC_XDL] ///
////////////////////////////////////////////////////////////////////////////////

#ifdef LC_ENABLE_DLOPEN
#if !defined(LC_PLATFORM_FREESTANDING)

#if defined(LC_PLATFORM_POSIX)
#include <dlfcn.h>

#define LC_DL_OPEN(shared_object) dlopen(shared_object, RTLD_NOW)
#define LC_DL_CLOSE(handle)       dlclose(handle)
#define LC_DL_SYM(handle, symbol) dlsym(handle, symbol)

#elif defined(LC_PLATFORM_WIN)

#define LC_DL_OPEN(shared_object)
#define LC_DL_CLOSE(handle)
#define LC_DL_SYM(handle, symbol)
#error "Not implemnted yet..."

#endif

#ifdef LC_HAVE_TYPEOF
#define LC_DL_EXPORT(symbol)         typeof(symbol) *symbol
#define LC_DL_IMPORT(handle, symbol) __extension__(typeof(symbol) *)LC_DL_SYM(handle, #symbol)
#else
#define LC_DL_EXPORT(symbol)         void *symbol
#define LC_DL_IMPORT(handle, symbol) LC_DL_SYM(handle, #symbol)
#endif

#endif
#endif

////////////////////////////////////////////////////////////////////////////////
/// Bit stream                                                      [LC_XBS] ///
////////////////////////////////////////////////////////////////////////////////

#ifdef LC_ENABLE_IO

#define bitstream(tagname)                                                                                   \
	struct tagname {                                                                                     \
		u32 buffer;                                                                                  \
		u8 remaining;                                                                                \
	}

bitstream(msb_stream);
bitstream(lsb_stream);

LC_NODISCARD
static inline u32
msb_stream_take_io_(LC_ARGS_DECL, struct msb_stream *stream, struct lc_io io, size_t count)
{
	LC_ASSERT(count <= UINT32_WIDTH - UINT8_WIDTH, "Cannot take more than 24-bits");

	while (stream->remaining < count) {
		u8 byte = 0;

		lc_io_len len = 0;
		if (LC_IO_ERR_NONE == lc_io_read(io, 1, &byte, &len)) {
			stream->buffer    <<= UINT8_WIDTH;
			stream->buffer     |= byte;
			stream->remaining  += UINT8_WIDTH;
		} else {
			stream->buffer    <<= count - stream->remaining;
			stream->remaining  += count - stream->remaining;
		}
	}

	u32 mask           = (1u << count) - 1;
	stream->remaining -= count;

	return (stream->buffer >> stream->remaining) & mask;
}

LC_NODISCARD
static inline u32
msb_stream_take_ptr_(LC_ARGS_DECL, struct msb_stream *stream, const u8 *restrict *io, size_t count)
{
	// LC_ASSERT(count <= UINT32_WIDTH - UINT8_WIDTH, "Cannot take more than 24-bits");

	while (stream->remaining < count) {
		stream->buffer    <<= UINT8_WIDTH;
		stream->buffer     |= **io;
		stream->remaining  += UINT8_WIDTH;
		(*io)++;
	}

	u32 mask           = (1u << count) - 1;
	stream->remaining -= count;

	return (stream->buffer >> stream->remaining) & mask;
}

LC_NODISCARD
static inline u32
msb_stream_peek_(LC_ARGS_DECL, struct msb_stream stream, struct lc_io io, size_t count)
{
	return msb_stream_take_io_(LC_ARGS_PASS, &stream, io, count);
}

LC_NODISCARD
static inline u32
lsb_stream_take_slice_(LC_ARGS_DECL, struct lsb_stream *stream, struct lc_bslice *io, size_t count)
{
	// LC_ASSERT(count <= UINT32_WIDTH - UINT8_WIDTH, "Cannot take more than 24-bits");

	while (stream->remaining < count) {
		if (io->size) {
			stream->buffer    |= ((u32)io->data[0] << stream->remaining);
			stream->remaining += UINT8_WIDTH;
			io->data++;
			io->size--;
		} else {
			stream->remaining += count - stream->remaining;
		}
	}

	const u32 mask = (1u << count) - 1;
	const u32 out  = stream->buffer & mask;

	stream->remaining  -= count;
	stream->buffer    >>= count;

	return out;
}

LC_NODISCARD
static inline u32
lsb_stream_take_mut_slice_(LC_ARGS_DECL, struct lsb_stream *stream, struct lc_bslice *io, size_t count)
{
	return lsb_stream_take_slice_(LC_ARGS_PASS, stream, (void *)io, count);
}


LC_NODISCARD
static inline u32
lsb_stream_take_ptr_(LC_ARGS_DECL, struct lsb_stream *stream, const u8 *restrict *io, size_t count)
{
	// LC_ASSERT(count <= UINT32_WIDTH - UINT8_WIDTH, "Cannot take more than 24-bits");

	while (stream->remaining < count) {
		stream->buffer    |= ((u32)**io << stream->remaining);
		stream->remaining += UINT8_WIDTH;
		(*io)++;
	}

	const u32 mask = (1u << count) - 1;
	const u32 out  = stream->buffer & mask;

	stream->remaining  -= count;
	stream->buffer    >>= count;

	return out;
}

LC_NODISCARD
static inline u32
lsb_stream_take_io_(LC_ARGS_DECL, struct lsb_stream *stream, struct lc_io io, size_t count)
{
	// LC_ASSERT(count <= UINT32_WIDTH - UINT8_WIDTH, "Cannot take more than 24-bits");

	while (stream->remaining < count) {
		u8 byte = 0;

		lc_io_len len = 0;
		if (LC_IO_ERR_NONE == lc_io_read(io, 1, &byte, &len)) {
			stream->buffer    |= ((u32)byte << stream->remaining);
			stream->remaining += UINT8_WIDTH;
		} else {
			stream->remaining += count - stream->remaining;
		}
	}

	const u32 mask = (1u << count) - 1;
	const u32 out  = stream->buffer & mask;

	stream->remaining  -= count;
	stream->buffer    >>= count;

	return out;
}

LC_NODISCARD
static inline u32
lsb_stream_take_file_(LC_ARGS_DECL, struct lsb_stream *stream, FILE *io, size_t count)
{
	// LC_ASSERT(count <= UINT32_WIDTH - UINT8_WIDTH, "Cannot take more than 24-bits");

	while (stream->remaining < count) {
		int c = fgetc(io);

		if (EOF != c) {
			stream->buffer    |= ((u32)c << stream->remaining);
			stream->remaining += UINT8_WIDTH;
		} else {
			stream->remaining += count - stream->remaining;
		}
	}

	const u32 mask = (1u << count) - 1;
	const u32 out  = stream->buffer & mask;

	stream->remaining  -= count;
	stream->buffer    >>= count;

	return out;
}

LC_NODISCARD
static inline u32
lsb_stream_take_io_ptr_(LC_ARGS_DECL, struct lsb_stream *stream, struct lc_io *io, size_t count)
{
	return lsb_stream_take_io_(LC_ARGS_PASS, stream, *io, count);
}

LC_NODISCARD
static inline u32
lsb_stream_take_file_ptr_(LC_ARGS_DECL, struct lsb_stream *stream, FILE **io, size_t count)
{
	return lsb_stream_take_file_(LC_ARGS_PASS, stream, *io, count);
}


LC_NODISCARD
static inline u32
lsb_stream_peek_io_(LC_ARGS_DECL, struct lsb_stream stream, struct lc_io io, size_t count)
{
	return lsb_stream_take_io_(LC_ARGS_PASS, &stream, io, count);
}

#define lsb_stream_take(stream, io, count)                                                                   \
	_Generic(*(typeof_unqual(io) *)0,                                                                    \
		struct lc_io:           lsb_stream_take_io_,                                                 \
		struct lc_io *:         lsb_stream_take_io_ptr_,                                             \
		const u8 **:            lsb_stream_take_ptr_,                                                \
		const u8 *restrict *:   lsb_stream_take_ptr_,                                                \
		const i8 **:            lsb_stream_take_ptr_,                                                \
		const i8 *restrict *:   lsb_stream_take_ptr_,                                                \
		const char **:          lsb_stream_take_ptr_,                                                \
		const char *restrict *: lsb_stream_take_ptr_,                                                \
		struct lc_bslice *:     lsb_stream_take_slice_,                                              \
		struct lc_mut_bslice *: lsb_stream_take_mut_slice_,                                          \
		FILE *:                 lsb_stream_take_file_,                                               \
		FILE **:                lsb_stream_take_file_ptr_                                            \
	)(LC_ARGS_PASS, stream, io, count)


#define lsb_stream_peek(stream, io, count) lsb_stream_peek_(LC_ARGS_PASS, stream, io, count)

#define msb_stream_take(stream, io, count)                                                                   \
	_Generic(*(typeof_unqual(io) *)0,                                                                    \
		struct lc_io:           msb_stream_take_io_,                                                 \
		struct lc_io *:         msb_stream_take_io_ptr_,                                             \
		const u8 **:            msb_stream_take_ptr_,                                                \
		const u8 *restrict *:   msb_stream_take_ptr_,                                                \
		const i8 **:            msb_stream_take_ptr_,                                                \
		const i8 *restrict *:   msb_stream_take_ptr_,                                                \
		const char **:          msb_stream_take_ptr_,                                                \
		const char *restrict *: msb_stream_take_ptr_,                                                \
		struct lc_bslice *:     msb_stream_take_slice_,                                              \
		struct lc_mut_bslice *: msb_stream_take_mut_slice_,                                          \
		FILE *:                 msb_stream_take_file_,                                               \
		FILE **:                msb_stream_take_file_ptr_                                            \
	)(LC_ARGS_PASS, stream, io, count)

#define msb_stream_peek(stream, io, count) msb_stream_peek_(LC_ARGS_PASS, stream, io, count)

#endif

////////////////////////////////////////////////////////////////////////////////
/// Endian                                                          [LC_XED] ///
////////////////////////////////////////////////////////////////////////////////

#define typedef_be(type)                                                                                     \
	typedef struct {                                                                                     \
		type _be;                                                                                    \
	} type##be

#define typedef_le(type)                                                                                     \
	typedef struct {                                                                                     \
		type _le;                                                                                    \
	} type##le

typedef_le(u16);
typedef_be(u16);
typedef_le(u32);
typedef_be(u32);
typedef_le(u64);
typedef_be(u64);

typedef_le(i16);
typedef_be(i16);
typedef_le(i32);
typedef_be(i32);
typedef_le(i64);
typedef_be(i64);

#undef typedef_le
#undef typedef_be

#define lc_bswap16_c(x)                                                                                      \
	( ((x & 0xFF00) >> 0x08)                                                                             \
	| ((x & 0x00FF) << 0x08)                                                                             \
	)

#define lc_bswap32_c(x)                                                                                      \
	( ((x & 0xFF000000) >> 0x18)                                                                         \
	| ((x & 0x00FF0000) >> 0x08)                                                                         \
	| ((x & 0x0000FF00) << 0x08)                                                                         \
	| ((x & 0x000000FF) << 0x18)                                                                         \
	)

#define lc_bswap64_c(x)                                                                                      \
	( ((x & 0xFF00000000000000) >> 0x38)                                                                 \
	| ((x & 0x00FF000000000000) >> 0x28)                                                                 \
	| ((x & 0x0000FF0000000000) >> 0x18)                                                                 \
	| ((x & 0x000000FF00000000) >> 0x08)                                                                 \
	| ((x & 0x00000000FF000000) << 0x08)                                                                 \
	| ((x & 0x0000000000FF0000) << 0x18)                                                                 \
	| ((x & 0x000000000000FF00) << 0x28)                                                                 \
	| ((x & 0x00000000000000FF) << 0x38)                                                                 \
	)

#define lc_bswap16(x) __builtin_bswap16(x)
#define lc_bswap32(x) __builtin_bswap32(x)
#define lc_bswap64(x) __builtin_bswap64(x)

#if __BYTE_ORDER == __LITTLE_ENDIAN
#define lc_be16toh(x) lc_bswap16(x._be)
#define lc_le16toh(x) x._le
#define lc_htobe16(x) ((u16be){lc_bswap16(x)})
#define lc_htole16(x) ((u16le){x})

#define lc_be32toh(x) lc_bswap32(x._be)
#define lc_le32toh(x) x._le
#define lc_htobe32(x) ((u32be){lc_bswap32(x)})
#define lc_htole32(x) ((u32le){x})

#define lc_be64toh(x) lc_bswap64(x._be)
#define lc_le64toh(x) x._le
#define lc_htobe64(x) ((u64be){lc_bswap64(x)})
#define lc_htole64(x) ((u64le){x})

#elif __BYTE_ORDER == __BIG_ENDIAN
#define lc_be16toh(x) x._be
#define lc_le16toh(x) lc_bswap16(x._le)
#define lc_htobe16(x) ((u16be){x})
#define lc_htole16(x) ((u16le){lc_bswap16(x)})

#define lc_be32toh(x) x._be
#define lc_le32toh(x) lc_bswap32(x._le)
#define lc_htobe32(x) ((u32be){x})
#define lc_htole32(x) ((u32le){lc_bswap32(x)})

#define lc_be64toh(x) x._be
#define lc_le64toh(x) lc_bswap64(x._le)
#define lc_htobe64(x) ((u64be){x})
#define lc_htole64(x) ((u64le){lc_bswap64(x)})
#endif

////////////////////////////////////////////////////////////////////////////////
/// Compression                                                     [LC_XCM] ///
////////////////////////////////////////////////////////////////////////////////

#ifdef LC_ENABLE_IO
#ifdef LC_ENABLE_DEFLATE
#include "lc_deflate.h"
#endif
#endif


////////////////////////////////////////////////////////////////////////////////
/// Base64                                                          [LC_XBC] ///
////////////////////////////////////////////////////////////////////////////////

static inline size_t
lc_base64_encode(size_t n, const u8 in[restrict n], size_t m, char out[restrict m])
{
	struct msb_stream stream = {0};
	const u8 *in_p   = in;
	const u8 *in_end = in + n;

	char *out_p   = (char *)out;
	char *out_end = (char *)out + n;

	static const char decode_tbl[64] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

	while (in_p < in_end && out_p < out_end) {
		u32 idx = msb_stream_take_ptr_(LC_ARGS(NULL, lc_src_loc(0)), &stream, &in_p, 6);
		*out_p++ = decode_tbl[idx];
	}

	return out_p - out;
}

static inline size_t
lc_base64_decode(size_t n, const char in[restrict n], size_t m, u8 out[restrict m])
{
	u32 buffer    = 0;
	u32 remaining = 0;

	const char *in_p   = in;
	const char *in_end = in + n;

	u8 *out_p   = (u8 *)out;
	u8 *out_end = (u8 *)out + m;

	while (in_p < in_end && out_p < out_end) {
		u8 bits = 0;

		if (0) {
		} else if (IN_RANGE(*in_p, 'A', 'Z')) {
			bits = 00 + *in_p++ - 'A';
		} else if (IN_RANGE(*in_p, 'a', 'z')) {
			bits = 26 + *in_p++ - 'A';
		} else if (IN_RANGE(*in_p, '0', '9')) {
			bits = 52 + *in_p++ - 'A';
		} else switch (*in_p) {
		case '+': bits = 62 + *in_p++ - '+'; break;
		case '/': bits = 63 + *in_p++ - '/'; break;
		case '=': bits = 00 + *in_p++ - '='; break;
		default: lc_unreachable();
		}

		enum
		{
			MASK = (1 << 6) - 1,
		};

		buffer    <<= 6;
		buffer     |= MASK & bits;
		remaining  += 6;
		if (remaining >= UINT8_WIDTH) {
			remaining -= UINT8_WIDTH;
			*out_p++   = buffer >> remaining;
		}
	}

	return out_p - out;
}

#ifdef LC_STD_CXX
};
#endif

#ifdef LC_STD_CXX
LC_WARN_POP
#endif

#endif
