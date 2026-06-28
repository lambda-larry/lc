#ifndef LC_DEFLATE_INCLUDE_H
#define LC_DEFLATE_INCLUDE_H

#ifndef LC_INCLUDE_H
#include "lc.h"
#endif

/// Table of content
///
/// @lc_toc_begin
/// - Deflate .....................................................735. [LC_XDF]
/// - ZLIB [TODO] ..................................................67. [LC_XGZ]
/// - GZIP [TODO] ...................................................4. [LC_XGZ]
/// - BZIP [TODO] ..................................................40. [LC_XBZ]
/// @lc_toc_end


////////////////////////////////////////////////////////////////////////////////
/// Deflate                                                         [LC_XDF] ///
////////////////////////////////////////////////////////////////////////////////

enum
{
	LC_DEFLATE_MAXBITS   = 15,  // maximum bits in a code
	LC_DEFLATE_MAXLCODES = 286, // maximum number of literal/length codes
	LC_DEFLATE_MAXDCODES = 30,  // maximum number of distance codes
	LC_DEFLATE_MAXCODES  = (LC_DEFLATE_MAXLCODES + LC_DEFLATE_MAXDCODES), // maximum codes lengths to read
	LC_DEFLATE_FIXLCODES = 288,    // number of fixed literal/length codes
	LC_DEFLATE_MAXDIST   = 0x8000, // maximum match distance (32k)
};


struct _lc_huffman {
	u16 count[LC_DEFLATE_MAXBITS + 1];
	u16 symbol[LC_DEFLATE_FIXLCODES];
};

#ifdef LC_DEV
#undef LC_ASSERT
#define LC_ASSERT(...) (void)0
#endif

struct lc_deflate_ctx {
	struct lsb_stream stream;
	lc_io_pos stream_pos;
	bool bfinal;
	int state;

	union {
		struct {
			u16 len;
		} stored;

		struct {
			struct _lc_huffman len_codes, dist_codes;

			u32 token;
			u32 length;
			u32 distance;
		} decoding;
	} d;

	struct {
		u8 data[LC_DEFLATE_MAXDIST];
		i64 offset;
	} sliding_window;
};


#define LC_HUFFMAN_DECODE_IMPLEMENTATION(name, type)                                                         \
	LC_NODISCARD                                                                                         \
	static inline u32                                                                                    \
	name(LC_ARGS_DECL, const struct _lc_huffman *h, struct lsb_stream *stream, type *io)                 \
	{                                                                                                    \
		const u16 *next = h->count + 1;                                                              \
                                                                                                             \
		u32 code  = 0;                                                                               \
		u32 first = 0;                                                                               \
		u32 index = 0;                                                                               \
                                                                                                             \
		for (;;) {                                                                                   \
			LC_ASSERT(next < h->count + LEN(h->count), "");                                      \
			u32 count = *next++;                                                                 \
                                                                                                             \
			code = (code << 1) | lsb_stream_take(stream, io, 1);                                 \
                                                                                                             \
			if (code < first + count)                                                            \
				return h->symbol[index + (code - first)];                                    \
                                                                                                             \
			index += count;                                                                      \
			first += count;                                                                      \
                                                                                                             \
			first <<= 1;                                                                         \
		}                                                                                            \
                                                                                                             \
		lc_unreachable();                                                                            \
	}                                                                                                    \
	LC_NODISCARD                                                                                         \
	static inline u32 name(LC_ARGS_DECL, const struct _lc_huffman *h, struct lsb_stream *stream, type *io)


LC_HUFFMAN_DECODE_IMPLEMENTATION(_lc_huffman_decode_io,    struct lc_io);
LC_HUFFMAN_DECODE_IMPLEMENTATION(_lc_huffman_decode_slice, struct lc_bslice);
LC_HUFFMAN_DECODE_IMPLEMENTATION(_lc_huffman_decode_file, FILE *);

#undef LC_HUFFMAN_DECODE_IMPLEMENTATION


static inline void
_lc_huffman_construct(struct _lc_huffman *h, size_t n, const u32 lengths[n])
{
	memset(h->count, 0, sizeof h->count);

	/// 1)  Count the number of codes for each code length.
	/// Let bl_count[N] be the number of codes of length N, N >= 1.
	for (size_t symbol = 0; symbol < n; symbol++)
		h->count[lengths[symbol]]++;

	u32 offsets[LEN(h->count)] = {0};
	for (size_t len = 1; len < LEN(offsets) - 1; len++)
		offsets[len + 1] = offsets[len] + h->count[len];

	for (size_t symbol = 0; symbol < n; symbol++)
		if (lengths[symbol])
			h->symbol[offsets[lengths[symbol]]++] = symbol;
}

#define LC_DEFLATE_INFLATE_IMPLEMENTATION_PROTOTYPE(name, io_type)                                           \
	LC_NODISCARD                                                                                         \
	extern size_t lc_deflate_inflate_##name(                                                             \
	        LC_ARGS_DECL,                                                                                \
	        struct lc_deflate_ctx *dctx,                                                                 \
	        io_type io,                                                                                  \
	        size_t n,                                                                                    \
	        u8 out[restrict n]                                                                           \
	)

#define LC_DEFLATE_INFLATE_IMPLEMENTATION(                                                                     \
        name,                                                                                                  \
        io_type,                                                                                               \
        HUFFMAN_DECODE,                                                                                        \
        STORED_2,                                                                                              \
        HUFFMAN_DECODE_1,                                                                                      \
        HUFFMAN_DECODE_3,                                                                                      \
        SAVE_STREAM_POS                                                                                        \
)                                                                                                              \
	LC_DEFLATE_INFLATE_IMPLEMENTATION_PROTOTYPE(name, io_type)                                             \
	{                                                                                                      \
		struct lsb_stream *stream = &dctx->stream;                                                     \
                                                                                                               \
		enum                                                                                           \
		{                                                                                              \
			DEFLATE_END_OF_BLOCK = 256,                                                            \
                                                                                                               \
			DEFLATE_CM_STORED          = 0x00,                                                     \
			DEFLATE_CM_FIXED_HUFFMAN   = 0x01,                                                     \
			DEFLATE_CM_DYNAMIC_HUFFMAN = 0x02,                                                     \
			DEFLATE_CM_RESERVED        = 0x03,                                                     \
		};                                                                                             \
                                                                                                               \
		size_t written = 0;                                                                            \
                                                                                                               \
		for (;;) {                                                                                     \
			const enum                                                                             \
			{                                                                                      \
				S_NEW_BLOCK = 0,                                                               \
				S_END_BLOCK,                                                                   \
				S_STORED_1,                                                                    \
				S_STORED_2,                                                                    \
				S_HUFFMAN_STATIC,                                                              \
				S_HUFFMAN_DYNAMIC,                                                             \
				S_HUFFMAN_DECODE_1,                                                            \
				S_HUFFMAN_DECODE_2,                                                            \
				S_HUFFMAN_DECODE_3,                                                            \
				S_EOF,                                                                         \
				S_ERR_1,                                                                       \
				S_ERR_2,                                                                       \
			} register state = dctx->state;                                                        \
                                                                                                               \
			switch (state) {                                                                       \
			case S_NEW_BLOCK:                                                                      \
			{                                                                                      \
                                                                                                               \
				dctx->bfinal   = lsb_stream_take(stream, &io, 1);                              \
				const u8 btype = lsb_stream_take(stream, &io, 2);                              \
                                                                                                               \
                                                                                                               \
				static const int state_transition[4] = {                                       \
				        [DEFLATE_CM_RESERVED]        = S_ERR_1,                                \
				        [DEFLATE_CM_STORED]          = S_STORED_1,                             \
				        [DEFLATE_CM_FIXED_HUFFMAN]   = S_HUFFMAN_STATIC,                       \
				        [DEFLATE_CM_DYNAMIC_HUFFMAN] = S_HUFFMAN_DYNAMIC,                      \
				};                                                                             \
				LC_ASSERT(btype < LEN(state_transition), "");                                  \
				lc_assume(btype < LEN(state_transition));                                      \
                                                                                                               \
				dctx->state = state_transition[btype];                                         \
				continue;                                                                      \
			}                                                                                      \
			case S_END_BLOCK:                                                                      \
                                                                                                               \
				if (dctx->bfinal) {                                                            \
					dctx->state = S_EOF;                                                   \
					continue;                                                              \
				}                                                                              \
                                                                                                               \
				dctx->state = S_NEW_BLOCK;                                                     \
				continue;                                                                      \
			case S_STORED_1:                                                                       \
			{                                                                                      \
				/**                                                                            \
				 * Any bits of input up to the next byte boundary are ignored.                 \
				 * The rest of the block consists of the following information:                \
				 */                                                                            \
				(void)lsb_stream_take(stream, &io, stream->remaining % UINT8_WIDTH);           \
                                                                                                               \
				dctx->d.stored.len = lsb_stream_take(stream, &io, UINT16_WIDTH);               \
                                                                                                               \
				const u16 nlen = lsb_stream_take(stream, &io, UINT16_WIDTH);                   \
                                                                                                               \
				/* NOTE(larry): Ummm, not sure if that is to be understood from the RFC...     \
				 */                                                                            \
				LC_ASSERT(dctx->d.stored.len == (nlen ^ 0xFFFF), "");                          \
				(void)nlen;                                                                    \
                                                                                                               \
				dctx->state = S_STORED_2;                                                      \
				LC_FALLTHROUGH;                                                                \
			}                                                                                      \
			case S_STORED_2:                                                                       \
			{                                                                                      \
				const u16 len = MIN(dctx->d.stored.len, n);                                    \
                                                                                                               \
				STORED_2(                                                                      \
				        io,                                                                    \
				        len,                                                                   \
				        dctx->sliding_window.offset,                                           \
				        dctx->sliding_window.data,                                             \
				        out                                                                    \
				);                                                                             \
                                                                                                               \
				dctx->d.stored.len -= len;                                                     \
				written            += len;                                                     \
				n                  -= len;                                                     \
				out                += len;                                                     \
                                                                                                               \
				if (0 == dctx->d.stored.len)                                                   \
					dctx->state = S_END_BLOCK;                                             \
                                                                                                               \
				if (0 == n)                                                                    \
					goto ret;                                                              \
                                                                                                               \
				continue;                                                                      \
			} break;                                                                               \
			case S_HUFFMAN_STATIC:                                                                 \
			{                                                                                      \
                                                                                                               \
				/**                                                                            \
				 *  Lit Value    Bits    Codes                                                 \
				 *  ---------    ----    -----                                                 \
				 *    0 - 143     8       00110000 through 10111111                            \
				 *  144 - 255     9      110010000 through 111111111                           \
				 *  256 - 279     7        0000000 through 0010111                             \
				 *  280 - 287     8       11000000 through 11000111                            \
				 */                                                                            \
                                                                                                               \
				/* Pre-computed huffman tree */                                                \
				static const struct _lc_huffman lencode = {                                    \
				        .count[7] = 24,                                                        \
				        .count[8] = 152,                                                       \
				        .count[9] = 112,                                                       \
				        .symbol =                                                              \
				                {                                                              \
				                        [0] = 256,   [1] = 257,   [2] = 258,   [3] = 259,      \
				                        [4] = 260,   [5] = 261,   [6] = 262,   [7] = 263,      \
				                        [8] = 264,   [9] = 265,   [10] = 266,  [11] = 267,     \
				                        [12] = 268,  [13] = 269,  [14] = 270,  [15] = 271,     \
				                        [16] = 272,  [17] = 273,  [18] = 274,  [19] = 275,     \
				                        [20] = 276,  [21] = 277,  [22] = 278,  [23] = 279,     \
				                        [24] = 0,    [25] = 1,    [26] = 2,    [27] = 3,       \
				                        [28] = 4,    [29] = 5,    [30] = 6,    [31] = 7,       \
				                        [32] = 8,    [33] = 9,    [34] = 10,   [35] = 11,      \
				                        [36] = 12,   [37] = 13,   [38] = 14,   [39] = 15,      \
				                        [40] = 16,   [41] = 17,   [42] = 18,   [43] = 19,      \
				                        [44] = 20,   [45] = 21,   [46] = 22,   [47] = 23,      \
				                        [48] = 24,   [49] = 25,   [50] = 26,   [51] = 27,      \
				                        [52] = 28,   [53] = 29,   [54] = 30,   [55] = 31,      \
				                        [56] = 32,   [57] = 33,   [58] = 34,   [59] = 35,      \
				                        [60] = 36,   [61] = 37,   [62] = 38,   [63] = 39,      \
				                        [64] = 40,   [65] = 41,   [66] = 42,   [67] = 43,      \
				                        [68] = 44,   [69] = 45,   [70] = 46,   [71] = 47,      \
				                        [72] = 48,   [73] = 49,   [74] = 50,   [75] = 51,      \
				                        [76] = 52,   [77] = 53,   [78] = 54,   [79] = 55,      \
				                        [80] = 56,   [81] = 57,   [82] = 58,   [83] = 59,      \
				                        [84] = 60,   [85] = 61,   [86] = 62,   [87] = 63,      \
				                        [88] = 64,   [89] = 65,   [90] = 66,   [91] = 67,      \
				                        [92] = 68,   [93] = 69,   [94] = 70,   [95] = 71,      \
				                        [96] = 72,   [97] = 73,   [98] = 74,   [99] = 75,      \
				                        [100] = 76,  [101] = 77,  [102] = 78,  [103] = 79,     \
				                        [104] = 80,  [105] = 81,  [106] = 82,  [107] = 83,     \
				                        [108] = 84,  [109] = 85,  [110] = 86,  [111] = 87,     \
				                        [112] = 88,  [113] = 89,  [114] = 90,  [115] = 91,     \
				                        [116] = 92,  [117] = 93,  [118] = 94,  [119] = 95,     \
				                        [120] = 96,  [121] = 97,  [122] = 98,  [123] = 99,     \
				                        [124] = 100, [125] = 101, [126] = 102, [127] = 103,    \
				                        [128] = 104, [129] = 105, [130] = 106, [131] = 107,    \
				                        [132] = 108, [133] = 109, [134] = 110, [135] = 111,    \
				                        [136] = 112, [137] = 113, [138] = 114, [139] = 115,    \
				                        [140] = 116, [141] = 117, [142] = 118, [143] = 119,    \
				                        [144] = 120, [145] = 121, [146] = 122, [147] = 123,    \
				                        [148] = 124, [149] = 125, [150] = 126, [151] = 127,    \
				                        [152] = 128, [153] = 129, [154] = 130, [155] = 131,    \
				                        [156] = 132, [157] = 133, [158] = 134, [159] = 135,    \
				                        [160] = 136, [161] = 137, [162] = 138, [163] = 139,    \
				                        [164] = 140, [165] = 141, [166] = 142, [167] = 143,    \
				                        [168] = 280, [169] = 281, [170] = 282, [171] = 283,    \
				                        [172] = 284, [173] = 285, [174] = 286, [175] = 287,    \
				                        [176] = 144, [177] = 145, [178] = 146, [179] = 147,    \
				                        [180] = 148, [181] = 149, [182] = 150, [183] = 151,    \
				                        [184] = 152, [185] = 153, [186] = 154, [187] = 155,    \
				                        [188] = 156, [189] = 157, [190] = 158, [191] = 159,    \
				                        [192] = 160, [193] = 161, [194] = 162, [195] = 163,    \
				                        [196] = 164, [197] = 165, [198] = 166, [199] = 167,    \
				                        [200] = 168, [201] = 169, [202] = 170, [203] = 171,    \
				                        [204] = 172, [205] = 173, [206] = 174, [207] = 175,    \
				                        [208] = 176, [209] = 177, [210] = 178, [211] = 179,    \
				                        [212] = 180, [213] = 181, [214] = 182, [215] = 183,    \
				                        [216] = 184, [217] = 185, [218] = 186, [219] = 187,    \
				                        [220] = 188, [221] = 189, [222] = 190, [223] = 191,    \
				                        [224] = 192, [225] = 193, [226] = 194, [227] = 195,    \
				                        [228] = 196, [229] = 197, [230] = 198, [231] = 199,    \
				                        [232] = 200, [233] = 201, [234] = 202, [235] = 203,    \
				                        [236] = 204, [237] = 205, [238] = 206, [239] = 207,    \
				                        [240] = 208, [241] = 209, [242] = 210, [243] = 211,    \
				                        [244] = 212, [245] = 213, [246] = 214, [247] = 215,    \
				                        [248] = 216, [249] = 217, [250] = 218, [251] = 219,    \
				                        [252] = 220, [253] = 221, [254] = 222, [255] = 223,    \
				                        [256] = 224, [257] = 225, [258] = 226, [259] = 227,    \
				                        [260] = 228, [261] = 229, [262] = 230, [263] = 231,    \
				                        [264] = 232, [265] = 233, [266] = 234, [267] = 235,    \
				                        [268] = 236, [269] = 237, [270] = 238, [271] = 239,    \
				                        [272] = 240, [273] = 241, [274] = 242, [275] = 243,    \
				                        [276] = 244, [277] = 245, [278] = 246, [279] = 247,    \
				                        [280] = 248, [281] = 249, [282] = 250, [283] = 251,    \
				                        [284] = 252, [285] = 253, [286] = 254, [287] = 255,    \
				                },                                                             \
				};                                                                             \
                                                                                                               \
                                                                                                               \
				/* Pre-computed huffman tree */                                                \
				/* clang-format off */                                                       \
				static const struct _lc_huffman distcode = {                                 \
					.count[5] = 30,                                                      \
					.symbol = {                                                          \
						[0] = 0, [10] = 10, [20] = 20,                               \
						[1] = 1, [11] = 11, [21] = 21,                               \
						[2] = 2, [12] = 12, [22] = 22,                               \
						[3] = 3, [13] = 13, [23] = 23,                               \
						[4] = 4, [14] = 14, [24] = 24,                               \
						[5] = 5, [15] = 15, [25] = 25,                               \
						[6] = 6, [16] = 16, [26] = 26,                               \
						[7] = 7, [17] = 17, [27] = 27,                               \
						[8] = 8, [18] = 18, [28] = 28,                               \
						[9] = 9, [19] = 19, [29] = 29,                               \
					},                                                                   \
				}; \
				/* clang-format on */                                                          \
                                                                                                               \
				dctx->d.decoding.len_codes  = lencode;                                         \
				dctx->d.decoding.dist_codes = distcode;                                        \
                                                                                                               \
				dctx->state = S_HUFFMAN_DECODE_1;                                              \
				continue;                                                                      \
			}                                                                                      \
			case S_HUFFMAN_DYNAMIC:                                                                \
			{                                                                                      \
				static const u8 pre_code_length_idx[] = {                                      \
				        16, 17, 18, 0, 8, 7, 9, 6, 10, 5, 11, 4, 12, 3, 13, 2, 14, 1, 15,      \
				};                                                                             \
                                                                                                               \
				const size_t hlit =                                                            \
				        lsb_stream_take(stream, &io, 5) + 257; /* Literal/Lenght code  */      \
				const size_t hdist =                                                           \
				        lsb_stream_take(stream, &io, 5) + 1;   /* Distance/Offset code */      \
				const size_t hclen =                                                           \
				        lsb_stream_take(stream, &io, 4) + 4;   /* Code Lenght code     */      \
                                                                                                               \
				u32 pre_code_length[LEN(pre_code_length_idx)] = {0};                           \
				static_assert(LEN(pre_code_length) == 19, "");                                 \
				for (size_t i = 0; i < hclen; i++)                                             \
					pre_code_length[pre_code_length_idx[i]] =                              \
					        lsb_stream_take(stream, &io, 3);                               \
                                                                                                               \
				struct _lc_huffman *len_codes = &dctx->d.decoding.len_codes;                   \
				_lc_huffman_construct(len_codes, LEN(pre_code_length), pre_code_length);       \
                                                                                                               \
				u32 lengths[LC_DEFLATE_MAXCODES] = {0};                                        \
				size_t index                     = 0;                                          \
				while (index < hlit + hdist) {                                                 \
                                                                                                               \
					u32 symbol = HUFFMAN_DECODE(LC_ARGS_CALL, len_codes, stream, &io);     \
                                                                                                               \
					u32 sym       = 0;                                                     \
					size_t repeat = 1;                                                     \
                                                                                                               \
					switch (symbol) {                                                      \
						/* clang-format off */ \
					case 0x00: case 0x01: case 0x02: case 0x03:                            \
					case 0x04: case 0x05: case 0x06: case 0x07:                            \
					case 0x08: case 0x09: case 0x0A: case 0x0B:                            \
					case 0x0C: case 0x0D: case 0x0E: case 0x0F:                            \
						/* clang-format on */                                          \
						sym = symbol;                                                  \
						break;                                                         \
					case 16:                                                               \
						LC_ASSERT(index != 0, "No last length available");             \
                                                                                                               \
						repeat = 3 + lsb_stream_take(stream, &io, 2);                  \
						sym    = lengths[index - 1];                                   \
						break;                                                         \
					case 17: repeat = 03 + lsb_stream_take(stream, &io, 3); break;         \
					case 18: repeat = 11 + lsb_stream_take(stream, &io, 7); break;         \
					default: lc_unreachable();                                             \
					}                                                                      \
                                                                                                               \
					for (size_t i = 0; i < repeat; i++)                                    \
						lengths[index++] = sym;                                        \
				}                                                                              \
                                                                                                               \
				LC_ASSERT(lengths[DEFLATE_END_OF_BLOCK], "Check for end-of-block");            \
                                                                                                               \
				_lc_huffman_construct(len_codes, hlit, lengths);                               \
                                                                                                               \
				struct _lc_huffman *dist_code = &dctx->d.decoding.dist_codes;                  \
				_lc_huffman_construct(dist_code, hdist, lengths + hlit);                       \
                                                                                                               \
				dctx->state = S_HUFFMAN_DECODE_1;                                              \
				LC_FALLTHROUGH;                                                                \
			}                                                                                      \
			case S_HUFFMAN_DECODE_1:                                                               \
			{                                                                                      \
				const struct _lc_huffman *len_codes = &dctx->d.decoding.len_codes;             \
                                                                                                               \
				while (n) {                                                                    \
					const u32 value =                                                      \
					        HUFFMAN_DECODE(LC_ARGS_CALL, len_codes, stream, &io);          \
                                                                                                               \
					dctx->d.decoding.token = value;                                        \
                                                                                                               \
					switch (lc_order_cmp_u32(value, DEFLATE_END_OF_BLOCK)) {               \
					default: lc_unreachable(); break;                                      \
					case LC_ORDER_LESS:                                                    \
					{                                                                      \
						HUFFMAN_DECODE_1(                                              \
						        dctx->sliding_window.offset,                           \
						        dctx->sliding_window.data,                             \
						        value,                                                 \
						        out                                                    \
						);                                                             \
						n--, written++;                                                \
						break;                                                         \
					}                                                                      \
                                                                                                               \
					case LC_ORDER_EQUAL:   goto end;                                       \
					case LC_ORDER_GREATER: goto decompress;                                \
					}                                                                      \
				}                                                                              \
                                                                                                               \
				if (0 == n)                                                                    \
					goto ret;                                                              \
                                                                                                               \
end:                                                                                                           \
				if (0 == n)                                                                    \
					goto ret;                                                              \
                                                                                                               \
				dctx->state = S_END_BLOCK;                                                     \
				continue;                                                                      \
                                                                                                               \
decompress:                                                                                                    \
				dctx->state = S_HUFFMAN_DECODE_2;                                              \
				LC_FALLTHROUGH;                                                                \
			}                                                                                      \
			case S_HUFFMAN_DECODE_2:                                                               \
			{                                                                                      \
				/**                                                                            \
				 *        Extra               Extra               Extra                        \
				 *   Code Bits Length(s) Code Bits Lengths   Code Bits Length(s)               \
				 *   ---- ---- ------     ---- ---- -------   ---- ---- -------                \
				 *    257   0     3       267   1   15,16     277   4   67-82                  \
				 *    258   0     4       268   1   17,18     278   4   83-98                  \
				 *    259   0     5       269   2   19-22     279   4   99-114                 \
				 *    260   0     6       270   2   23-26     280   4  115-130                 \
				 *    261   0     7       271   2   27-30     281   5  131-162                 \
				 *    262   0     8       272   2   31-34     282   5  163-194                 \
				 *    263   0     9       273   3   35-42     283   5  195-226                 \
				 *    264   0    10       274   3   43-50     284   5  227-257                 \
				 *    265   1  11,12      275   3   51-58     285   0    258                   \
				 *    266   1  13,14      276   3   59-66                                      \
				 */                                                                            \
				/* clang-format off */ \
				static const struct {                                                        \
					u16 ext;                                                             \
					u16 len;                                                             \
				} len_lut[] = {                                                              \
					[257] = {0,  3},  [267] = {1, 15},  [277] = {4,  67},                \
					[258] = {0,  4},  [268] = {1, 17},  [278] = {4,  83},                \
					[259] = {0,  5},  [269] = {2, 19},  [279] = {4,  99},                \
					[260] = {0,  6},  [270] = {2, 23},  [280] = {4, 115},                \
					[261] = {0,  7},  [271] = {2, 27},  [281] = {5, 131},                \
					[262] = {0,  8},  [272] = {2, 31},  [282] = {5, 163},                \
					[263] = {0,  9},  [273] = {3, 35},  [283] = {5, 195},                \
					[264] = {0, 10},  [274] = {3, 43},  [284] = {5, 227},                \
					[265] = {1, 11},  [275] = {3, 51},  [285] = {0, 258},                \
					[266] = {1, 13},  [276] = {3, 59},                                   \
				};                                                       \
				/* clang-format on */                                                          \
                                                                                                               \
				/**                                                                            \
				 *       Extra           Extra               Extra                             \
				 *  Code Bits Dist  Code Bits   Dist     Code Bits Distance                    \
				 *  ---- ---- ----  ---- ----  ------    ---- ---- --------                    \
				 *    0   0    1     10   4     33-48    20    9   1025-1536                   \
				 *    1   0    2     11   4     49-64    21    9   1537-2048                   \
				 *    2   0    3     12   5     65-96    22   10   2049-3072                   \
				 *    3   0    4     13   5     97-128   23   10   3073-4096                   \
				 *    4   1   5,6    14   6    129-192   24   11   4097-6144                   \
				 *    5   1   7,8    15   6    193-256   25   11   6145-8192                   \
				 *    6   2   9-12   16   7    257-384   26   12  8193-12288                   \
				 *    7   2  13-16   17   7    385-512   27   12 12289-16384                   \
				 *    8   3  17-24   18   8    513-768   28   13 16385-24576                   \
				 *    9   3  25-32   19   8   769-1024   29   13 24577-32768                   \
				 */                                                                            \
				/* clang-format off */                                                 \
				static const struct {                                                          \
					u16 ext;                                                               \
					u16 len;                                                               \
				} dist_lut[] = {                                                               \
					[0] = {0,  1},  [10] = {4,  33}, [20] = { 9,  1025},                   \
					[1] = {0,  2},  [11] = {4,  49}, [21] = { 9,  1537},                   \
					[2] = {0,  3},  [12] = {5,  65}, [22] = {10,  2049},                   \
					[3] = {0,  4},  [13] = {5,  97}, [23] = {10,  3073},                   \
					[4] = {1,  5},  [14] = {6, 129}, [24] = {11,  4097},                   \
					[5] = {1,  7},  [15] = {6, 193}, [25] = {11,  6145},                   \
					[6] = {2,  9},  [16] = {7, 257}, [26] = {12,  8193},                   \
					[7] = {2, 13},  [17] = {7, 385}, [27] = {12, 12289},                   \
					[8] = {3, 17},  [18] = {8, 513}, [28] = {13, 16385},                   \
					[9] = {3, 25},  [19] = {8, 769}, [29] = {13, 24577},                   \
				};       \
				/* clang-format on */                                                          \
                                                                                                               \
				const u32 value = dctx->d.decoding.token;                                      \
				lc_assume(IN_RANGE(value, DEFLATE_END_OF_BLOCK + 1, LEN(len_lut) - 1));        \
                                                                                                               \
				const struct _lc_huffman *dist_codes = &dctx->d.decoding.dist_codes;           \
                                                                                                               \
				const u32 length = len_lut[value].len                                          \
				                 + lsb_stream_take(stream, &io, len_lut[value].ext);           \
				const u32 dsym     = HUFFMAN_DECODE(LC_ARGS_CALL, dist_codes, stream, &io);    \
				const u32 distance = dist_lut[dsym].len                                        \
				                   + lsb_stream_take(stream, &io, dist_lut[dsym].ext);         \
                                                                                                               \
				dctx->d.decoding.length   = length;                                            \
				dctx->d.decoding.distance = distance;                                          \
                                                                                                               \
				dctx->state = S_HUFFMAN_DECODE_3;                                              \
				LC_FALLTHROUGH;                                                                \
			}                                                                                      \
			case S_HUFFMAN_DECODE_3:                                                               \
			{                                                                                      \
				const u32 len = MIN(dctx->d.decoding.length, n);                               \
                                                                                                               \
				HUFFMAN_DECODE_3(                                                              \
				        len,                                                                   \
				        dctx->sliding_window.offset,                                           \
				        dctx->sliding_window.data,                                             \
				        dctx->d.decoding.distance,                                             \
				        out                                                                    \
				);                                                                             \
                                                                                                               \
				dctx->d.decoding.length -= len;                                                \
                                                                                                               \
				n       -= len;                                                                \
				written += len;                                                                \
                                                                                                               \
				if (0 == dctx->d.decoding.length)                                              \
					dctx->state = S_HUFFMAN_DECODE_1;                                      \
                                                                                                               \
				if (0 == n)                                                                    \
					goto ret;                                                              \
                                                                                                               \
				continue;                                                                      \
			}                                                                                      \
                                                                                                               \
			case S_EOF: return written;                                                            \
                                                                                                               \
			case S_ERR_1:                                                                          \
				dctx->state = S_ERR_2;                                                         \
				/* (void)lc_io_tell(io, &dctx->stream_pos); */                                 \
				SAVE_STREAM_POS(io, dctx->stream_pos);                                         \
				LC_FALLTHROUGH;                                                                \
			case S_ERR_2: return written;                                                          \
			}                                                                                      \
		}                                                                                              \
                                                                                                               \
                                                                                                               \
ret:                                                                                                           \
		/* (void)lc_io_tell(io, &dctx->stream_pos); */                                                 \
		SAVE_STREAM_POS(io, dctx->stream_pos);                                                         \
		return written;                                                                                \
	}                                                                                                      \
	LC_DEFLATE_INFLATE_IMPLEMENTATION_PROTOTYPE(name, io_type)

#define _lc_deflate_save_stream(...) (void)0



#define _lc_deflate_io_stored2(io, len, offset, window, out)                                                 \
	do {                                                                                                 \
		(void)lc_io_read(io, len, out, &(lc_io_len){0});                                             \
                                                                                                             \
		for (u32 i = 0; i < len; i++) {                                                              \
			window[offset++] = out[i];                                                           \
                                                                                                             \
			offset %= LC_DEFLATE_MAXDIST;                                                        \
		}                                                                                            \
	} while (false)

#define _lc_deflate_io_huffman_decode_1(offset, window, value, out)                                          \
	do {                                                                                                 \
		window[offset++] = *out++ = value;                                                           \
                                                                                                             \
		offset %= LC_DEFLATE_MAXDIST;                                                                \
	} while (false)

#define _lc_deflate_io_huffman_decode_3(len, offset, window, distance, out)                                  \
	do {                                                                                                 \
		size_t writer = offset;                                                                      \
		size_t reader = offset;                                                                      \
                                                                                                             \
		reader += LC_DEFLATE_MAXDIST;                                                                \
		reader -= distance;                                                                          \
		reader %= LC_DEFLATE_MAXDIST;                                                                \
                                                                                                             \
		if (1 == distance) {                                                                         \
			u8 b = window[reader];                                                               \
			for (size_t i = 0; i < len; i++)                                                     \
				window[writer++ % LC_DEFLATE_MAXDIST] = *out++ = b;                          \
                                                                                                             \
		} else {                                                                                     \
			for (size_t i = 0; i < len; i++)                                                     \
				window[writer++ % LC_DEFLATE_MAXDIST] = *out++ =                             \
				        window[reader++ % LC_DEFLATE_MAXDIST];                               \
		}                                                                                            \
                                                                                                             \
		offset = writer % LC_DEFLATE_MAXDIST;                                                        \
	} while (false)

#ifndef LC_IMPLEMENTATION
LC_DEFLATE_INFLATE_IMPLEMENTATION_PROTOTYPE(io, struct lc_io);
#else
LC_DEFLATE_INFLATE_IMPLEMENTATION(
        io,
        struct lc_io,
        _lc_huffman_decode_io,
        _lc_deflate_io_stored2,
        _lc_deflate_io_huffman_decode_1,
        _lc_deflate_io_huffman_decode_3,
        _lc_deflate_save_stream
);
#endif

#define _lc_deflate_slice_stored2(io, len, offset, window, out)                                              \
	do {                                                                                                 \
		memcpy(out, io.data, len);                                                                   \
		io.data += len;                                                                              \
		io.size -= len;                                                                              \
                                                                                                             \
		for (u32 i = 0; i < len; i++) {                                                              \
			window[offset++] = out[i];                                                           \
                                                                                                             \
			offset %= LC_DEFLATE_MAXDIST;                                                        \
		}                                                                                            \
	} while (false)

#ifndef LC_IMPLEMENTATION
LC_DEFLATE_INFLATE_IMPLEMENTATION_PROTOTYPE(slice, struct lc_bslice);
#else
LC_DEFLATE_INFLATE_IMPLEMENTATION(
        slice,
        struct lc_bslice,
        _lc_huffman_decode_slice,
        _lc_deflate_slice_stored2,
        _lc_deflate_io_huffman_decode_1,
        _lc_deflate_io_huffman_decode_3,
        _lc_deflate_save_stream
);
#endif

#define _lc_deflate_file_stored2(io, len, offset, window, out)                                               \
	do {                                                                                                 \
                fread(out, len, 1, io);                                                                      \
                                                                                                             \
		for (u32 i = 0; i < len; i++) {                                                              \
			window[offset++] = out[i];                                                           \
                                                                                                             \
			offset %= LC_DEFLATE_MAXDIST;                                                        \
		}                                                                                            \
	} while (false)

#define _lc_deflate_file_huffman_decode_1(offset, window, value, out)                                        \
	do {                                                                                                 \
		window[offset++] = *out++ = value;                                                           \
                                                                                                             \
		offset %= LC_DEFLATE_MAXDIST;                                                                \
	} while (false)

#define _lc_deflate_file_huffman_decode_3(len, offset, window, distance, out)                                \
	do {                                                                                                 \
		size_t writer = offset;                                                                      \
		size_t reader = offset;                                                                      \
                                                                                                             \
		reader += LC_DEFLATE_MAXDIST;                                                                \
		reader -= distance;                                                                          \
		reader %= LC_DEFLATE_MAXDIST;                                                                \
                                                                                                             \
		if (1 == distance) {                                                                         \
			u8 b = window[reader];                                                               \
			for (size_t i = 0; i < len; i++)                                                     \
				window[writer++ % LC_DEFLATE_MAXDIST] = *out++ = b;                          \
                                                                                                             \
		} else {                                                                                     \
			for (size_t i = 0; i < len; i++)                                                     \
				window[writer++ % LC_DEFLATE_MAXDIST] = *out++ =                             \
				        window[reader++ % LC_DEFLATE_MAXDIST];                               \
		}                                                                                            \
                                                                                                             \
		offset = writer % LC_DEFLATE_MAXDIST;                                                        \
	} while (false)

#ifndef LC_IMPLEMENTATION
LC_DEFLATE_INFLATE_IMPLEMENTATION_PROTOTYPE(file, FILE *);
#else
LC_DEFLATE_INFLATE_IMPLEMENTATION(
        file,
        FILE *,
        _lc_huffman_decode_file,
        _lc_deflate_file_stored2,
        _lc_deflate_file_huffman_decode_1,
        _lc_deflate_file_huffman_decode_3,
        _lc_deflate_save_stream
);
#endif

////////////////////////////////////////////////////////////////////////////////
/// ZLIB [TODO]                                                     [LC_XGZ] ///
////////////////////////////////////////////////////////////////////////////////

struct lc_zlib_ctx {
	int state;
	struct lc_deflate_ctx dctx;
};

extern size_t
lc_zlib_inflate_slice(LC_ARGS_DECL, struct lc_zlib_ctx *zctx, struct lc_bslice slice, size_t n, u8 out[restrict n])
#ifndef LC_IMPLEMENTATION
	;
#else
{
	union zlib_header {
		struct {
			u8 cm:    4;
			u8 cinfo: 3;
			u8: 1;
			u8 fcheck: 5;
			u8 fdict:  1;
			u8 flevel: 2;
		};
		struct {
			u8 cmf;
			u8 flg;
		};
	};
	static_assert(sizeof (union zlib_header) == 2);


	switch (zctx->state) {
	default: lc_unreachable();
	case 0:
		const union zlib_header *zlib_header = (void *)slice.data;
		LC_ASSERT(8 == zlib_header->cm,    "Compression method must be DEFLATE");
		LC_ASSERT(7 >= zlib_header->cinfo, "Values of CINFO above 7 are not allowed in this version of the specification");
		LC_ASSERT(0 == zlib_header->fdict, "Preset dictionary not allowed for now");
		LC_ASSERT(
			0 == (zlib_header->cmf * 256 + zlib_header->flg) % 31,
			"fcheck failed, corrupt data"
		);

		slice.data += sizeof *zlib_header;
		slice.size -= sizeof *zlib_header;

		zctx->state = 1;
		LC_FALLTHROUGH;
	case 1:
		return lc_deflate_inflate_slice(
			LC_ARGS_CALL,
			&zctx->dctx,
			slice,
			n,
			out
		);

	}



}
#endif



////////////////////////////////////////////////////////////////////////////////
/// GZIP [TODO]                                                     [LC_XGZ] ///
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/// BZIP [TODO]                                                     [LC_XBZ] ///
////////////////////////////////////////////////////////////////////////////////

#if 0

#include <unistd.h>
#include <sys/mman.h>

extern u8 *
lc_magic_ring_buffer_create(int *fd)
{

	enum
	{
		BUFFER_SIZE = LC_DEFLATE_MAXDIST,
	};

	*fd = memfd_create("lc_magic_ring", MFD_CLOEXEC);
	ftruncate(*fd, BUFFER_SIZE);

	u8 *buffer = mmap(nullptr, 3 * BUFFER_SIZE, PROT_NONE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

	mmap(buffer + 0 * BUFFER_SIZE, BUFFER_SIZE, PROT_READ | PROT_WRITE, MAP_FIXED | MAP_SHARED, *fd, 0);
	mmap(buffer + 1 * BUFFER_SIZE, BUFFER_SIZE, PROT_READ | PROT_WRITE, MAP_FIXED | MAP_SHARED, *fd, 0);
	mmap(buffer + 2 * BUFFER_SIZE, BUFFER_SIZE, PROT_READ | PROT_WRITE, MAP_FIXED | MAP_SHARED, *fd, 0);

	return buffer + 1 * BUFFER_SIZE;
}

extern void
lc_magic_ring_buffer_destroy(int fd, u8 *buffer)
{
	munmap(buffer - LC_DEFLATE_MAXDIST, 3 * LC_DEFLATE_MAXDIST);
	close(fd);
	return;
}

#endif

#endif
