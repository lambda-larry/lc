#ifndef LC_LEX_C_H
#define LC_LEX_C_H

#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-label"
#endif

#include <stdbool.h>
#include <stdint.h>

#ifndef LC_HAVE_SV
#define LC_HAVE_SV
#include <stddef.h>

struct lc_sv {
	size_t length;
	const char *s;
};

#define sv_c(str)    (struct lc_sv){.length = LEN(str) - 1, .s = str}
#endif


#ifndef LEN
#define LEN(X) (sizeof (X) / sizeof (X)[0])
#endif

#ifndef lc_memcmp
extern int memcmp(const void *restrict, const void *restrict, size_t);
#define lc_memcmp memcmp
#endif

enum lc_c_token_type
{
	TOK_END_OF_FILE = 0,
	TOK_LIT_NUM,
	TOK_LIT_STR,
	TOK_LIT_CHR,
	TOK_SYM,
	TOK_COLON,
	TOK_SEMICOLON,
	TOK_DOT,
	TOK_COMMA,
	TOK_LBRACE,
	TOK_RBRACE,
	TOK_LBRACK,
	TOK_RBRACK,
	TOK_LPAREN,
	TOK_RPAREN,
	TOK_ELIPSE,
	TOK_QMARK,
	TOK_SCOPE,
	TOK_ARROW,
	TOK_ADD,
	TOK_SUB,
	TOK_MUL,
	TOK_DIV,
	TOK_MOD,
	TOK_ASS,
	TOK_BANG,
	TOK_AMP,
	TOK_NEQ,
	TOK_EQ,
	TOK_LT,
	TOK_GT,
	TOK_LE,
	TOK_GE,
	TOK_CMOD,
	TOK_AND,
	TOK_CAND,
	TOK_CMUL,
	TOK_INC,
	TOK_CADD,
	TOK_DEC,
	TOK_CSUB,
	TOK_CDIV,
	TOK_XOR,
	TOK_CXOR,
	TOK_BOR,
	TOK_COR,
	TOK_OR,
	TOK_BNOT,
	TOK_KW_ALIGNAS,
	TOK_KW_ALIGNOF,
	TOK_KW_ASM,
	TOK_KW_ATOMIC,
	TOK_KW_ATTRIBUTE,
	TOK_KW_AUTO,
	TOK_KW_BITINT,
	TOK_KW_BREAK,
	TOK_KW_CASE,
	TOK_KW_CONST,
	TOK_KW_CONSTEXPR,
	TOK_KW_CONTINUE,
	TOK_KW_DEFAULT,
	TOK_KW_DO,
	TOK_KW_ELSE,
	TOK_KW_ENUM,
	TOK_KW_EXTERN,
	TOK_KW_FALSE,
	TOK_KW_FOR,
	TOK_KW_GENERIC,
	TOK_KW_GOTO,
	TOK_KW_IF,
	TOK_KW_IMAGINARY,
	TOK_KW_INLINE,
	TOK_KW_NORETURN,
	TOK_KW_NULLPTR,
	TOK_KW_REGISTER,
	TOK_KW_RESTRICT,
	TOK_KW_RETURN,
	TOK_KW_SIZEOF,
	TOK_KW_STATIC,
	TOK_KW_STATIC_ASSERT,
	TOK_KW_STRUCT,
	TOK_KW_SWITCH,
	TOK_KW_THREAD_LOCAL,
	TOK_KW_TRUE,
	TOK_KW_TYPEDEF,
	TOK_KW_TYPEOF,
	TOK_KW_TYPEOF_UNQUAL,
	TOK_KW_UNION,
	TOK_KW_VOLATILE,
	TOK_KW_WHILE,
	TOK_UNKNOWN,
};

static inline const char *
lc_token_type_str(enum lc_c_token_type type)
{
	static const char *token_type_str[] = {
		[TOK_END_OF_FILE]      = "TOK_END_OF_FILE",
		[TOK_LIT_NUM]          = "TOK_LIT_NUM",
		[TOK_LIT_STR]          = "TOK_LIT_STR",
		[TOK_LIT_CHR]          = "TOK_LIT_CHR",
		[TOK_SYM]              = "TOK_SYM",
		[TOK_COLON]            = "TOK_COLON",
		[TOK_SEMICOLON]        = "TOK_SEMICOLON",
		[TOK_DOT]              = "TOK_DOT",
		[TOK_COMMA]            = "TOK_COMMA",
		[TOK_LBRACE]           = "TOK_LBRACE",
		[TOK_RBRACE]           = "TOK_RBRACE",
		[TOK_LBRACK]           = "TOK_LBRACK",
		[TOK_RBRACK]           = "TOK_RBRACK",
		[TOK_LPAREN]           = "TOK_LPAREN",
		[TOK_RPAREN]           = "TOK_RPAREN",
		[TOK_ELIPSE]           = "TOK_ELIPSE",
		[TOK_QMARK]            = "TOK_QMARK",
		[TOK_SCOPE]            = "TOK_SCOPE",
		[TOK_ARROW]            = "TOK_ARROW",
		[TOK_ADD]              = "TOK_ADD",
		[TOK_SUB]              = "TOK_SUB",
		[TOK_MUL]              = "TOK_MUL",
		[TOK_DIV]              = "TOK_DIV",
		[TOK_MOD]              = "TOK_MOD",
		[TOK_ASS]              = "TOK_ASS",
		[TOK_BANG]             = "TOK_BANG",
		[TOK_AMP]              = "TOK_AMP",
		[TOK_NEQ]              = "TOK_NEQ",
		[TOK_EQ]               = "TOK_EQ",
		[TOK_LT]               = "TOK_LT",
		[TOK_GT]               = "TOK_GT",
		[TOK_LE]               = "TOK_LE",
		[TOK_GE]               = "TOK_GE",
		[TOK_CMOD]             = "TOK_CMOD",
		[TOK_AND]              = "TOK_AND",
		[TOK_CAND]             = "TOK_CAND",
		[TOK_CMUL]             = "TOK_CMUL",
		[TOK_INC]              = "TOK_INC",
		[TOK_CADD]             = "TOK_CADD",
		[TOK_DEC]              = "TOK_DEC",
		[TOK_CSUB]             = "TOK_CSUB",
		[TOK_CDIV]             = "TOK_CDIV",
		[TOK_XOR]              = "TOK_XOR",
		[TOK_CXOR]             = "TOK_CXOR",
		[TOK_BOR]              = "TOK_BOR",
		[TOK_COR]              = "TOK_COR",
		[TOK_OR]               = "TOK_OR",
		[TOK_BNOT]             = "TOK_BNOT",
		[TOK_KW_ALIGNAS]       = "TOK_KW_ALIGNAS",
		[TOK_KW_ALIGNOF]       = "TOK_KW_ALIGNOF",
		[TOK_KW_ASM]           = "TOK_KW_ASM",
		[TOK_KW_ATOMIC]        = "TOK_KW_ATOMIC",
		[TOK_KW_ATTRIBUTE]     = "TOK_KW_ATTRIBUTE",
		[TOK_KW_AUTO]          = "TOK_KW_AUTO",
		[TOK_KW_BITINT]        = "TOK_KW_BITINT",
		[TOK_KW_BREAK]         = "TOK_KW_BREAK",
		[TOK_KW_CASE]          = "TOK_KW_CASE",
		[TOK_KW_CONST]         = "TOK_KW_CONST",
		[TOK_KW_CONSTEXPR]     = "TOK_KW_CONSTEXPR",
		[TOK_KW_CONTINUE]      = "TOK_KW_CONTINUE",
		[TOK_KW_DEFAULT]       = "TOK_KW_DEFAULT",
		[TOK_KW_DO]            = "TOK_KW_DO",
		[TOK_KW_ELSE]          = "TOK_KW_ELSE",
		[TOK_KW_ENUM]          = "TOK_KW_ENUM",
		[TOK_KW_EXTERN]        = "TOK_KW_EXTERN",
		[TOK_KW_FALSE]         = "TOK_KW_FALSE",
		[TOK_KW_FOR]           = "TOK_KW_FOR",
		[TOK_KW_GENERIC]       = "TOK_KW_GENERIC",
		[TOK_KW_GOTO]          = "TOK_KW_GOTO",
		[TOK_KW_IF]            = "TOK_KW_IF",
		[TOK_KW_IMAGINARY]     = "TOK_KW_IMAGINARY",
		[TOK_KW_INLINE]        = "TOK_KW_INLINE",
		[TOK_KW_NORETURN]      = "TOK_KW_NORETURN",
		[TOK_KW_NULLPTR]       = "TOK_KW_NULLPTR",
		[TOK_KW_REGISTER]      = "TOK_KW_REGISTER",
		[TOK_KW_RESTRICT]      = "TOK_KW_RESTRICT",
		[TOK_KW_RETURN]        = "TOK_KW_RETURN",
		[TOK_KW_SIZEOF]        = "TOK_KW_SIZEOF",
		[TOK_KW_STATIC]        = "TOK_KW_STATIC",
		[TOK_KW_STATIC_ASSERT] = "TOK_KW_STATIC_ASSERT",
		[TOK_KW_STRUCT]        = "TOK_KW_STRUCT",
		[TOK_KW_SWITCH]        = "TOK_KW_SWITCH",
		[TOK_KW_THREAD_LOCAL]  = "TOK_KW_THREAD_LOCAL",
		[TOK_KW_TRUE]          = "TOK_KW_TRUE",
		[TOK_KW_TYPEDEF]       = "TOK_KW_TYPEDEF",
		[TOK_KW_TYPEOF]        = "TOK_KW_TYPEOF",
		[TOK_KW_TYPEOF_UNQUAL] = "TOK_KW_TYPEOF_UNQUAL",
		[TOK_KW_UNION]         = "TOK_KW_UNION",
		[TOK_KW_VOLATILE]      = "TOK_KW_VOLATILE",
		[TOK_KW_WHILE]         = "TOK_KW_WHILE",
		[TOK_UNKNOWN]          = "TOK_UNKNOWN",
	};
	return token_type_str[type];
}

struct lc_c_source_location {
	struct lc_sv filename;
	uint32_t     line_no;
	uint32_t     column_no;
	uint32_t     line_begin;
};

struct lc_c_token {
	enum   lc_c_token_type      type;
	struct lc_c_source_location loc;
	struct lc_sv          value;
};

struct lc_c_lexer {
	struct lc_sv buffer;
	struct lc_sv filename;
	int32_t line_no;
	int32_t column_no;
	size_t  byte_offset;
};

static inline bool
lc_lex_c_sv_is_empty(struct lc_sv sv)
{
	return 0 == sv.length;
}

static inline char
lc_lex_c_sv_peak(struct lc_sv *sv)
{
	return *sv->s;
}

static inline bool
lc_lex_c_sv_has_prefix(struct lc_sv sv, struct lc_sv prefix)
{
	return prefix.length <= sv.length && 0 == lc_memcmp(sv.s, prefix.s, prefix.length);
}

static inline bool
lc_lex_c_sv_equal(struct lc_sv lhs, struct lc_sv rhs)
{
	return lhs.length == rhs.length && 0 == lc_memcmp(lhs.s, rhs.s, lhs.length);
}

static inline char
lc_lex_c_sv_pop(struct lc_sv *sv)
{
	if (lc_lex_c_sv_is_empty(*sv)) {
		return 0;
	}

	sv->length--;
	return *sv->s++;
}

static bool
lc_lex_c_try_parse_identifer_or_keyword(struct lc_c_lexer *ls, struct lc_c_token *token)
{
	struct lc_sv *stream = &(struct lc_sv){
		.s      = ls->buffer.s      + ls->byte_offset,
		.length = ls->buffer.length - ls->byte_offset,
	};


begin:

	switch (lc_lex_c_sv_peak(stream)) {
	default:          return false;
	case '_' ... '_': goto next;
	case 'a' ... 'z': goto next;
	case 'A' ... 'Z': goto next;
	}


next:

	lc_lex_c_sv_pop(stream);
	switch (lc_lex_c_sv_peak(stream)) {
	default:          goto kw_done;
	case '_' ... '_': goto next;
	case '0' ... '9': goto next_ident;
	case 'a' ... 'z': goto next;
	case 'A' ... 'Z': goto next;
	}


next_ident:

	lc_lex_c_sv_pop(stream);
	switch (lc_lex_c_sv_peak(stream)) {
	default:          goto ident_done;
	case '_' ... '_': goto next_ident;
	case '0' ... '9': goto next_ident;
	case 'a' ... 'z': goto next_ident;
	case 'A' ... 'Z': goto next_ident;
	}

kw_done:
	;

	static const struct {
		enum lc_c_token_type type;
		struct lc_sv content;
	} keywords[] = {
#define lc_lex_c_kw_map(k, v) { .type = (k), .content.s = (v), .content.length = LEN(v) - 1, }
		lc_lex_c_kw_map(TOK_KW_ALIGNAS,       "_Alignas"),
		lc_lex_c_kw_map(TOK_KW_ALIGNAS,       "alignas"),
		lc_lex_c_kw_map(TOK_KW_ALIGNOF,       "_Alignof"),
		lc_lex_c_kw_map(TOK_KW_ALIGNOF,       "alignof"),
		lc_lex_c_kw_map(TOK_KW_ASM,           "__asm__"),
		lc_lex_c_kw_map(TOK_KW_ASM,           "asm"),
		lc_lex_c_kw_map(TOK_KW_ATOMIC,        "_Atomic"),
		lc_lex_c_kw_map(TOK_KW_ATTRIBUTE,     "__attribute__"),
		lc_lex_c_kw_map(TOK_KW_AUTO,          "auto"),
		lc_lex_c_kw_map(TOK_KW_BITINT,        "_BitInt"),
		lc_lex_c_kw_map(TOK_KW_BREAK,         "break"),
		lc_lex_c_kw_map(TOK_KW_CASE,          "case"),
		lc_lex_c_kw_map(TOK_KW_CONST,         "const"),
		lc_lex_c_kw_map(TOK_KW_CONSTEXPR,     "constexpr"),
		lc_lex_c_kw_map(TOK_KW_CONTINUE,      "continue"),
		lc_lex_c_kw_map(TOK_KW_DEFAULT,       "default"),
		lc_lex_c_kw_map(TOK_KW_DO,            "do"),
		lc_lex_c_kw_map(TOK_KW_ELSE,          "else"),
		lc_lex_c_kw_map(TOK_KW_ENUM,          "enum"),
		lc_lex_c_kw_map(TOK_KW_EXTERN,        "extern"),
		lc_lex_c_kw_map(TOK_KW_FALSE,         "false"),
		lc_lex_c_kw_map(TOK_KW_FOR,           "for"),
		lc_lex_c_kw_map(TOK_KW_GENERIC,       "_Generic"),
		lc_lex_c_kw_map(TOK_KW_GOTO,          "goto"),
		lc_lex_c_kw_map(TOK_KW_IF,            "if"),
		lc_lex_c_kw_map(TOK_KW_IMAGINARY,     "_Imaginary"),
		lc_lex_c_kw_map(TOK_KW_INLINE,        "inline"),
		lc_lex_c_kw_map(TOK_KW_NORETURN,      "_Noreturn"),
		lc_lex_c_kw_map(TOK_KW_NULLPTR,       "nullptr"),
		lc_lex_c_kw_map(TOK_KW_REGISTER,      "register"),
		lc_lex_c_kw_map(TOK_KW_RESTRICT,      "restrict"),
		lc_lex_c_kw_map(TOK_KW_RESTRICT,      "__restrict"),
		lc_lex_c_kw_map(TOK_KW_RETURN,        "return"),
		lc_lex_c_kw_map(TOK_KW_SIZEOF,        "sizeof"),
		lc_lex_c_kw_map(TOK_KW_STATIC,        "static"),
		lc_lex_c_kw_map(TOK_KW_STATIC_ASSERT, "_Static_assert"),
		lc_lex_c_kw_map(TOK_KW_STATIC_ASSERT, "static_assert"),
		lc_lex_c_kw_map(TOK_KW_STRUCT,        "struct"),
		lc_lex_c_kw_map(TOK_KW_SWITCH,        "switch"),
		lc_lex_c_kw_map(TOK_KW_THREAD_LOCAL,  "_Thread_local"),
		lc_lex_c_kw_map(TOK_KW_THREAD_LOCAL,  "thread_local"),
		lc_lex_c_kw_map(TOK_KW_TRUE,          "true"),
		lc_lex_c_kw_map(TOK_KW_TYPEDEF,       "typedef"),
		lc_lex_c_kw_map(TOK_KW_TYPEOF,        "typeof"),
		lc_lex_c_kw_map(TOK_KW_TYPEOF,        "__typeof__"),
		lc_lex_c_kw_map(TOK_KW_TYPEOF_UNQUAL, "typeof_unqual"),
		lc_lex_c_kw_map(TOK_KW_UNION,         "union"),
		lc_lex_c_kw_map(TOK_KW_VOLATILE,      "volatile"),
		lc_lex_c_kw_map(TOK_KW_WHILE,         "while"),
	};

	struct lc_sv tok = {0};
	tok.s        = ls->buffer.s + ls->byte_offset;
	tok.length   = stream->s - tok.s;

	for (size_t i = 0; i < LEN(keywords); i++) {
		if (!lc_lex_c_sv_equal(tok, keywords[i].content))
			continue;

		*token = (struct lc_c_token){
			.type           = keywords[i].type,
			.loc.filename   = ls->filename,
			.loc.line_no    = ls->line_no,
			.loc.column_no  = ls->column_no,
			.loc.line_begin = ls->byte_offset - ls->column_no + 1,
			.value          = tok,
		};
		ls->column_no += token->value.length;
		ls->byte_offset = token->value.s + token->value.length - ls->buffer.s;
		return true;
	}

ident_done:
	tok.s        = ls->buffer.s + ls->byte_offset;
	tok.length   = stream->s - tok.s;

	*token = (struct lc_c_token){
		.type           = TOK_SYM,
		.loc.filename   = ls->filename,
		.loc.line_no    = ls->line_no,
		.loc.column_no  = ls->column_no,
		.loc.line_begin = ls->byte_offset - ls->column_no + 1,
		.value          = tok,
	};
	ls->column_no += token->value.length;
	ls->byte_offset = token->value.s + token->value.length - ls->buffer.s;

	return true;
}

static bool
lc_lex_c_try_parse_chr(struct lc_c_lexer *ls, struct lc_c_token *token)
{
	struct lc_sv *stream = &(struct lc_sv){
		.s      = ls->buffer.s      + ls->byte_offset,
		.length = ls->buffer.length - ls->byte_offset,
	};


	switch (lc_lex_c_sv_peak(stream)) {
	default:   return false;
	case '\'': goto parse_begin;
	}

parse_begin:

	lc_lex_c_sv_pop(stream);
	switch (lc_lex_c_sv_peak(stream)) {
	default:   goto parse_end;
	case '\\': goto parse_escaped;
	}

parse_escaped:
	/// https://en.cppreference.com/w/c/language/escape

	lc_lex_c_sv_pop(stream);
	switch (lc_lex_c_sv_peak(stream)) {
	default:   return false;
	case '\'': goto parse_end;
	case '\"': goto parse_end;
	case '\\': goto parse_end;
	case '?':  goto parse_end;
	case 'a':  goto parse_end;
	case 'b':  goto parse_end;
	case 'f':  goto parse_end;
	case 'n':  goto parse_end;
	case 'r':  goto parse_end;
	case 't':  goto parse_end;
	case 'v':  goto parse_end;
	case 'u':  goto parse_uni4;
	case 'U':  goto parse_uni8;
	case 'x':  goto parse_hex;
	case '0':  goto parse_oct;
	case '1':  goto parse_oct;
	case '2':  goto parse_oct;
	case '3':  goto parse_oct;
	case '4':  goto parse_oct;
	case '5':  goto parse_oct;
	case '6':  goto parse_oct;
	case '7':  goto parse_oct;
	}

parse_uni8:

	for (size_t i = 0; i < 4; i++) {
		lc_lex_c_sv_pop(stream);
		switch (lc_lex_c_sv_peak(stream)) {
		default:          return false;
		case '0' ... '9': break;
		case 'a' ... 'f': break;
		case 'A' ... 'F': break;
		}
	}

parse_uni4:

	for (size_t i = 0; i < 2; i++) {
		lc_lex_c_sv_pop(stream);
		switch (lc_lex_c_sv_peak(stream)) {
		default:          return false;
		case '0' ... '9': break;
		case 'a' ... 'f': break;
		case 'A' ... 'F': break;
		}
	}

parse_hex:

	for (size_t i = 0; i < 2; i++) {
		lc_lex_c_sv_pop(stream);
		switch (lc_lex_c_sv_peak(stream)) {
		default:          return false;
		case '0' ... '9': break;
		case 'a' ... 'f': break;
		case 'A' ... 'F': break;
		}
	}

	goto parse_end;

parse_oct:

	lc_lex_c_sv_pop(stream);
	switch (lc_lex_c_sv_peak(stream)) {
	default:          goto parse_end;
	case '0' ... '7': break;
	}

	lc_lex_c_sv_pop(stream);
	switch (lc_lex_c_sv_peak(stream)) {
	default:          goto parse_end;
	case '0' ... '7': break;
	}

	goto parse_end;

parse_end:

	lc_lex_c_sv_pop(stream);
	switch (lc_lex_c_sv_peak(stream)) {
	default:   return false;
	case '\'': goto parse_done;
	}

parse_done:

	lc_lex_c_sv_pop(stream);
	*token = (struct lc_c_token){
		.type           = TOK_LIT_CHR,
		.loc.filename   = ls->filename,
		.loc.line_no    = ls->line_no,
		.loc.column_no  = ls->column_no,
		.loc.line_begin = ls->byte_offset - ls->column_no + 1,
		.value.s        = ls->buffer.s + ls->byte_offset + 2,
		.value.length   = stream->s - (ls->buffer.s + ls->byte_offset) - 2,
	};
	ls->column_no += token->value.length + 2;
	ls->byte_offset = token->value.s + token->value.length - ls->buffer.s + 2;

	return true;
}

static bool
lc_lex_c_try_parse_str(struct lc_c_lexer *ls, struct lc_c_token *token)
{
	struct lc_sv *stream = &(struct lc_sv){
		.s      = ls->buffer.s      + ls->byte_offset,
		.length = ls->buffer.length - ls->byte_offset,
	};

parse_begin:

	switch (lc_lex_c_sv_peak(stream)) {
	default:   return false;
	case '\"': goto parse_inside;
	}

parse_inside:

	lc_lex_c_sv_pop(stream);
	switch (lc_lex_c_sv_peak(stream)) {
	default:   goto parse_inside;
	case '\\': goto parse_escaped;
	case '\"': goto parse_done;
	}

parse_escaped:

	lc_lex_c_sv_pop(stream);
	switch (lc_lex_c_sv_peak(stream)) {
	default:   return false;
	case '\'': goto parse_inside;
	case '\"': goto parse_inside;
	case '\\': goto parse_inside;
	case '?':  goto parse_inside;
	case 'a':  goto parse_inside;
	case 'b':  goto parse_inside;
	case 'f':  goto parse_inside;
	case 'n':  goto parse_inside;
	case 'r':  goto parse_inside;
	case 't':  goto parse_inside;
	case 'v':  goto parse_inside;
	case 'u':  goto parse_uni4;
	case 'U':  goto parse_uni8;
	case 'x':  goto parse_hex;
	case '0':  goto parse_oct;
	case '1':  goto parse_oct;
	case '2':  goto parse_oct;
	case '3':  goto parse_oct;
	case '4':  goto parse_oct;
	case '5':  goto parse_oct;
	case '6':  goto parse_oct;
	case '7':  goto parse_oct;
	}

parse_uni8:

	for (size_t i = 0; i < 4; i++) {
		lc_lex_c_sv_pop(stream);
		switch (lc_lex_c_sv_peak(stream)) {
		default:          return false;
		case '0' ... '9': break;
		case 'a' ... 'f': break;
		case 'A' ... 'F': break;
		}
	}

parse_uni4:

	for (size_t i = 0; i < 2; i++) {
		lc_lex_c_sv_pop(stream);
		switch (lc_lex_c_sv_peak(stream)) {
		default:          return false;
		case '0' ... '9': break;
		case 'a' ... 'f': break;
		case 'A' ... 'F': break;
		}
	}

parse_hex:

	for (size_t i = 0; i < 2; i++) {
		lc_lex_c_sv_pop(stream);
		switch (lc_lex_c_sv_peak(stream)) {
		default:          return false;
		case '0' ... '9': break;
		case 'a' ... 'f': break;
		case 'A' ... 'F': break;
		}
	}

	goto parse_inside;

parse_oct:

	lc_lex_c_sv_pop(stream);
	switch (lc_lex_c_sv_peak(stream)) {
	default:          goto parse_inside;
	case '0' ... '7': break;
	}

	lc_lex_c_sv_pop(stream);
	switch (lc_lex_c_sv_peak(stream)) {
	default:          goto parse_inside;
	case '0' ... '7': break;
	}

	goto parse_inside;

parse_done:

	lc_lex_c_sv_pop(stream);
	*token = (struct lc_c_token){
		.type           = TOK_LIT_STR,
		.loc.filename   = ls->filename,
		.loc.line_no    = ls->line_no,
		.loc.column_no  = ls->column_no,
		.loc.line_begin = ls->byte_offset - ls->column_no + 1,
		.value.s        = ls->buffer.s + ls->byte_offset + 1,
		.value.length   = stream->s - (ls->buffer.s + ls->byte_offset) - 2,
	};
	ls->column_no  += token->value.length + 2;
	ls->byte_offset = token->value.s + token->value.length - ls->buffer.s + 1;

	return true;
}

static bool
lc_lex_c_try_parse_number(struct lc_c_lexer *ls, struct lc_c_token *token)
{
	struct lc_sv *stream = &(struct lc_sv){
		.s      = ls->buffer.s      + ls->byte_offset,
		.length = ls->buffer.length - ls->byte_offset,
	};

	switch (lc_lex_c_sv_peak(stream)) {
	default: return false;
	case '0' ... '0': goto parse_base;
	case '1' ... '9': goto parse_dec;
	}

parse_base:

	lc_lex_c_sv_pop(stream);
	switch (lc_lex_c_sv_peak(stream)) {
	default:  goto done;
	case 'b': goto parse_bin;
	case 'o': goto parse_oct;
	case 'x': goto parse_hex;

	case '0': goto parse_oct;
	case '1': goto parse_oct;
	case '2': goto parse_oct;
	case '3': goto parse_oct;
	case '4': goto parse_oct;
	case '5': goto parse_oct;
	case '6': goto parse_oct;
	case '7': goto parse_oct;
	// case '8': goto parse_oct;
	// case '9': goto parse_oct;
	}

parse_bin:

	lc_lex_c_sv_pop(stream);
	switch (lc_lex_c_sv_peak(stream)) {
	default:  goto done;
	case '0': goto parse_bin;
	case '1': goto parse_bin;
	}


parse_oct:

	lc_lex_c_sv_pop(stream);
	switch (lc_lex_c_sv_peak(stream)) {
	default:  goto done;
	case '0' ... '7': goto parse_oct;
	}

parse_hex:

	lc_lex_c_sv_pop(stream);
	switch (lc_lex_c_sv_peak(stream)) {
	default:  goto done;
	case '0' ... '9': goto parse_hex;
	case 'a' ... 'f': goto parse_hex;
	case 'A' ... 'F': goto parse_hex;
	}

parse_dec:

	lc_lex_c_sv_pop(stream);
	switch (lc_lex_c_sv_peak(stream)) {
	default:  goto done;
	case '0' ... '9': goto parse_dec;
	}

done:

	*token = (struct lc_c_token){
		.type           = TOK_LIT_NUM,
		.loc.filename   = ls->filename,
		.loc.line_no    = ls->line_no,
		.loc.column_no  = ls->column_no,
		.loc.line_begin = ls->byte_offset - ls->column_no + 1,
		.value.s        = ls->buffer.s + ls->byte_offset,
		.value.length   = stream->s - (ls->buffer.s + ls->byte_offset),
	};
	ls->byte_offset = token->value.s + token->value.length - ls->buffer.s;
	ls->column_no += token->value.length;

	return true;
}

static bool
lc_lex_c_try_parse_preprocessor(struct lc_c_lexer *ls)
{
	struct lc_sv *stream = &(struct lc_sv){
		.s      = ls->buffer.s      + ls->byte_offset,
		.length = ls->buffer.length - ls->byte_offset,
	};

	if (!lc_lex_c_sv_has_prefix(*stream, sv_c("#")))
		return false;

	size_t line_no = 0;

	lc_lex_c_sv_pop(stream);
	lc_lex_c_sv_pop(stream);

	switch (lc_lex_c_sv_peak(stream)) {
	case '0' ... '9': goto parse_lineno;
	default:          goto parse_eol;
	}

parse_eol:
	switch (lc_lex_c_sv_peak(stream)) {
	case '\n': lc_lex_c_sv_pop(stream); goto done;
	default:   lc_lex_c_sv_pop(stream); goto parse_eol;
	}

parse_lineno:
	switch (lc_lex_c_sv_peak(stream)) {
	case '0' ... '9': line_no *= 10; line_no += lc_lex_c_sv_pop(stream) - '0'; goto parse_lineno;
	case ' ': goto parse_filename_begin;
	default:  goto parse_eol;
	}

parse_filename_begin:
	ls->line_no  = line_no;
	lc_lex_c_sv_pop(stream);
	switch (lc_lex_c_sv_peak(stream)) {
	default:   goto parse_eol;
	case '\"': break;
	}

	struct lc_sv filename = {
		.s = stream->s + 1,
	};
parse_filename:
	lc_lex_c_sv_pop(stream);
	switch (lc_lex_c_sv_peak(stream)) {
	default:   goto parse_filename;
	case '\"': break;
	}

	filename.length = stream->s - filename.s;
	ls->filename = filename;

	goto parse_eol;

done:

	ls->column_no = 1;
	ls->byte_offset = stream->s - ls->buffer.s;

	return true;
}

extern bool
lc_lex_c_token(struct lc_c_lexer *ls, struct lc_c_token *token)
{
lex_begin:
	;
	struct lc_sv *stream = &(struct lc_sv){
		.s      = ls->buffer.s      + ls->byte_offset,
		.length = ls->buffer.length - ls->byte_offset,
	};

lex_whitespace:
	while (!lc_lex_c_sv_is_empty(*stream)) {
		switch (lc_lex_c_sv_peak(stream)) {
			case ' ':
			case '\r':
			case '\t':
				lc_lex_c_sv_pop(stream);
				ls->column_no++;
				continue;
			case '\n':
				lc_lex_c_sv_pop(stream);
				ls->column_no = 1;
				ls->line_no++;
				continue;
			default: break;
		}

		break;
	}
	ls->byte_offset = stream->s - ls->buffer.s;

lex_preprocessor:
	if (lc_lex_c_try_parse_preprocessor(ls))
		goto lex_begin;

lex_single_line_comment:
	if (lc_lex_c_sv_has_prefix(*stream, sv_c("//"))) {
		while ('\n' != lc_lex_c_sv_peak(stream))
			lc_lex_c_sv_pop(stream);
		lc_lex_c_sv_pop(stream);
		ls->column_no = 1;
		ls->line_no++;
		ls->byte_offset = stream->s - ls->buffer.s;
		goto lex_begin;
	}

lex_multi_line_comment:
	if (lc_lex_c_sv_has_prefix(*stream, sv_c("/*"))) {
		lc_lex_c_sv_pop(stream); ls->column_no++;
		lc_lex_c_sv_pop(stream); ls->column_no++;

		while (!lc_lex_c_sv_has_prefix(*stream, sv_c("*/"))) {
			switch (lc_lex_c_sv_peak(stream)) {
			case '\n':
				ls->column_no = 0;
				ls->line_no++;
			}
			lc_lex_c_sv_pop(stream); ls->column_no++;
		}

		lc_lex_c_sv_pop(stream); ls->column_no++;
		lc_lex_c_sv_pop(stream); ls->column_no++;
		ls->byte_offset = stream->s - ls->buffer.s;
		goto lex_begin;
	}

lex_char:
	if (lc_lex_c_try_parse_chr(ls, token))
		goto done;

lex_string:
	if (lc_lex_c_try_parse_str(ls, token))
		goto done;

lex_numbers:
	if (lc_lex_c_try_parse_number(ls, token))
		goto done;

lex_identifer:
	if (lc_lex_c_try_parse_identifer_or_keyword(ls, token))
		goto done;

lex_punct:
	{
		static const struct {
			enum lc_c_token_type type;
			struct lc_sv content;
		} puncts[] = {
#define lc_lex_c_puncts_map(k, v) { .type = (k), .content.s = (v), .content.length = LEN(v) - 1, }
			lc_lex_c_puncts_map(TOK_ELIPSE,    "..."),
			lc_lex_c_puncts_map(TOK_QMARK,     "?"),
			lc_lex_c_puncts_map(TOK_SCOPE,     "::"),
			lc_lex_c_puncts_map(TOK_COLON,     ":"),
			lc_lex_c_puncts_map(TOK_SEMICOLON, ";"),
			lc_lex_c_puncts_map(TOK_ARROW,     "->"),
			lc_lex_c_puncts_map(TOK_DOT,       "."),
			lc_lex_c_puncts_map(TOK_COMMA,     ","),
			lc_lex_c_puncts_map(TOK_LBRACE,    "{"),
			lc_lex_c_puncts_map(TOK_RBRACE,    "}"),
			lc_lex_c_puncts_map(TOK_LBRACK,    "["),
			lc_lex_c_puncts_map(TOK_RBRACK,    "]"),
			lc_lex_c_puncts_map(TOK_LPAREN,    "("),
			lc_lex_c_puncts_map(TOK_RPAREN,    ")"),
			lc_lex_c_puncts_map(TOK_NEQ,       "!="),
			lc_lex_c_puncts_map(TOK_EQ,        "=="),
			lc_lex_c_puncts_map(TOK_LE,        "<="),
			lc_lex_c_puncts_map(TOK_GE,        ">="),
			lc_lex_c_puncts_map(TOK_LT,        "<"),
			lc_lex_c_puncts_map(TOK_GT,        ">"),
			lc_lex_c_puncts_map(TOK_ADD,       "+"),
			lc_lex_c_puncts_map(TOK_SUB,       "-"),
			lc_lex_c_puncts_map(TOK_MUL,       "*"),
			lc_lex_c_puncts_map(TOK_DIV,       "/"),
			lc_lex_c_puncts_map(TOK_MOD,       "%"),
			lc_lex_c_puncts_map(TOK_ASS,       "="),
			lc_lex_c_puncts_map(TOK_BANG,      "!"),
			lc_lex_c_puncts_map(TOK_AMP,       "&"),
			lc_lex_c_puncts_map(TOK_CMOD,      "%="),
			lc_lex_c_puncts_map(TOK_AND,       "&&"),
			lc_lex_c_puncts_map(TOK_CAND,      "&="),
			lc_lex_c_puncts_map(TOK_CMUL,      "*="),
			lc_lex_c_puncts_map(TOK_INC,       "++"),
			lc_lex_c_puncts_map(TOK_CADD,      "+="),
			lc_lex_c_puncts_map(TOK_DEC,       "--"),
			lc_lex_c_puncts_map(TOK_CSUB,      "-="),
			lc_lex_c_puncts_map(TOK_CDIV,      "/="),
			lc_lex_c_puncts_map(TOK_XOR,       "^"),
			lc_lex_c_puncts_map(TOK_CXOR,      "^="),
			lc_lex_c_puncts_map(TOK_BOR,       "|"),
			lc_lex_c_puncts_map(TOK_COR,       "|="),
			lc_lex_c_puncts_map(TOK_OR,        "||"),
			lc_lex_c_puncts_map(TOK_BNOT,      "~"),
		};

		size_t longest     = 0;
		size_t longest_idx = LEN(puncts);

		// Maximal munching rule
		for (size_t i = 0; i < LEN(puncts); i++) {
			if (!lc_lex_c_sv_has_prefix(*stream, puncts[i].content))
				continue;

			if (longest >= puncts[i].content.length)
				continue;

			longest = puncts[i].content.length;
			longest_idx = i;
		}

		if (longest_idx < LEN(puncts)) {
			*token = (struct lc_c_token){
				.type           = puncts[longest_idx].type,
				.loc.filename   = ls->filename,
				.loc.line_no    = ls->line_no,
				.loc.column_no  = ls->column_no,
				.loc.line_begin = ls->byte_offset - ls->column_no + 1,
				.value.s        = stream->s,
				.value.length   = puncts[longest_idx].content.length,
			};
			stream->s      += token->value.length;
			stream->length -= token->value.length;
			ls->column_no  += token->value.length;
			ls->byte_offset = token->value.s + token->value.length - ls->buffer.s;
			goto done;
		}
	}

	return false;

done:

	return true;
}

#ifdef __GNUC__
#pragma GCC diagnostic push
#endif

#endif
