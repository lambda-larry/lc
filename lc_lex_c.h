#ifndef LC_LEX_C_H
#define LC_LEX_C_H

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
	uint32_t           line_no;
	uint32_t           column_no;
	const char         *line_begin;
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


	static const struct {
		enum lc_c_token_type type;
		struct lc_sv content;
	} keywords[] = {
		{.type = TOK_KW_ALIGNAS,       .content = sv_c("_Alignas")},
		{.type = TOK_KW_ALIGNAS,       .content = sv_c("alignas")},
		{.type = TOK_KW_ALIGNOF,       .content = sv_c("_Alignof")},
		{.type = TOK_KW_ALIGNOF,       .content = sv_c("alignof")},
		{.type = TOK_KW_ASM,           .content = sv_c("__asm__")},
		{.type = TOK_KW_ASM,           .content = sv_c("asm")},
		{.type = TOK_KW_ATOMIC,        .content = sv_c("_Atomic")},
		{.type = TOK_KW_ATTRIBUTE,     .content = sv_c("__attribute__")},
		{.type = TOK_KW_AUTO,          .content = sv_c("auto")},
		{.type = TOK_KW_BITINT,        .content = sv_c("_BitInt")},
		{.type = TOK_KW_BREAK,         .content = sv_c("break")},
		{.type = TOK_KW_CASE,          .content = sv_c("case")},
		{.type = TOK_KW_CONST,         .content = sv_c("const")},
		{.type = TOK_KW_CONSTEXPR,     .content = sv_c("constexpr")},
		{.type = TOK_KW_CONTINUE,      .content = sv_c("continue")},
		{.type = TOK_KW_DEFAULT,       .content = sv_c("default")},
		{.type = TOK_KW_DO,            .content = sv_c("do")},
		{.type = TOK_KW_ELSE,          .content = sv_c("else")},
		{.type = TOK_KW_ENUM,          .content = sv_c("enum")},
		{.type = TOK_KW_EXTERN,        .content = sv_c("extern")},
		{.type = TOK_KW_FALSE,         .content = sv_c("false")},
		{.type = TOK_KW_FOR,           .content = sv_c("for")},
		{.type = TOK_KW_GENERIC,       .content = sv_c("_Generic")},
		{.type = TOK_KW_GOTO,          .content = sv_c("goto")},
		{.type = TOK_KW_IF,            .content = sv_c("if")},
		{.type = TOK_KW_IMAGINARY,     .content = sv_c("_Imaginary")},
		{.type = TOK_KW_INLINE,        .content = sv_c("inline")},
		{.type = TOK_KW_NORETURN,      .content = sv_c("_Noreturn")},
		{.type = TOK_KW_NULLPTR,       .content = sv_c("nullptr")},
		{.type = TOK_KW_REGISTER,      .content = sv_c("register")},
		{.type = TOK_KW_RESTRICT,      .content = sv_c("restrict")},
		{.type = TOK_KW_RESTRICT,      .content = sv_c("__restrict")},
		{.type = TOK_KW_RETURN,        .content = sv_c("return")},
		{.type = TOK_KW_SIZEOF,        .content = sv_c("sizeof")},
		{.type = TOK_KW_STATIC,        .content = sv_c("static")},
		{.type = TOK_KW_STATIC_ASSERT, .content = sv_c("_Static_assert")},
		{.type = TOK_KW_STATIC_ASSERT, .content = sv_c("static_assert")},
		{.type = TOK_KW_STRUCT,        .content = sv_c("struct")},
		{.type = TOK_KW_SWITCH,        .content = sv_c("switch")},
		{.type = TOK_KW_THREAD_LOCAL,  .content = sv_c("_Thread_local")},
		{.type = TOK_KW_THREAD_LOCAL,  .content = sv_c("thread_local")},
		{.type = TOK_KW_TRUE,          .content = sv_c("true")},
		{.type = TOK_KW_TYPEDEF,       .content = sv_c("typedef")},
		{.type = TOK_KW_TYPEOF,        .content = sv_c("typeof")},
		{.type = TOK_KW_TYPEOF,        .content = sv_c("__typeof__")},
		{.type = TOK_KW_TYPEOF_UNQUAL, .content = sv_c("typeof_unqual")},
		{.type = TOK_KW_UNION,         .content = sv_c("union")},
		{.type = TOK_KW_VOLATILE,      .content = sv_c("volatile")},
		{.type = TOK_KW_WHILE,         .content = sv_c("while")},
	};

	struct lc_sv tok = {};
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
			// .loc.line_begin = stream_->s - (ls->column_no - 1),
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
		// .loc.line_begin = stream_->s - (ls->column_no - 1),
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
		// .loc.line_begin = stream_->s - (ls->column_no - 1),
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
		// .loc.line_begin = stream_->s - (ls->column_no - 1),
		.value.s        = ls->buffer.s + ls->byte_offset + 1,
		.value.length   = stream->s - (ls->buffer.s + ls->byte_offset) - 2,
	};
	ls->column_no  += token->value.length + 2;
	ls->byte_offset = token->value.s + token->value.length - ls->buffer.s + 2;

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
		// .loc.line_begin = stream_->s - (ls->column_no - 1),
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
			{.type = TOK_ELIPSE,    .content = sv_c("...")},
			{.type = TOK_QMARK,     .content = sv_c("?")},
			{.type = TOK_SCOPE,     .content = sv_c("::")},
			{.type = TOK_COLON,     .content = sv_c(":")},
			{.type = TOK_SEMICOLON, .content = sv_c(";")},
			{.type = TOK_ARROW,     .content = sv_c("->")},
			{.type = TOK_DOT,       .content = sv_c(".")},
			{.type = TOK_COMMA,     .content = sv_c(",")},
			{.type = TOK_LBRACE,    .content = sv_c("{")},
			{.type = TOK_RBRACE,    .content = sv_c("}")},
			{.type = TOK_LBRACK,    .content = sv_c("[")},
			{.type = TOK_RBRACK,    .content = sv_c("]")},
			{.type = TOK_LPAREN,    .content = sv_c("(")},
			{.type = TOK_RPAREN,    .content = sv_c(")")},
			{.type = TOK_NEQ,       .content = sv_c("!=")},
			{.type = TOK_EQ,        .content = sv_c("==")},
			{.type = TOK_LE,        .content = sv_c("<=")},
			{.type = TOK_GE,        .content = sv_c(">=")},
			{.type = TOK_LT,        .content = sv_c("<")},
			{.type = TOK_GT,        .content = sv_c(">")},
			{.type = TOK_ADD,       .content = sv_c("+")},
			{.type = TOK_SUB,       .content = sv_c("-")},
			{.type = TOK_MUL,       .content = sv_c("*")},
			{.type = TOK_DIV,       .content = sv_c("/")},
			{.type = TOK_MOD,       .content = sv_c("%")},
			{.type = TOK_ASS,       .content = sv_c("=")},
			{.type = TOK_BANG,      .content = sv_c("!")},
			{.type = TOK_AMP,       .content = sv_c("&")},
			{.type = TOK_CMOD,      .content = sv_c("%=")},
			{.type = TOK_AND,       .content = sv_c("&&")},
			{.type = TOK_CAND,      .content = sv_c("&=")},
			{.type = TOK_CMUL,      .content = sv_c("*=")},
			{.type = TOK_INC,       .content = sv_c("++")},
			{.type = TOK_CADD,      .content = sv_c("+=")},
			{.type = TOK_DEC,       .content = sv_c("--")},
			{.type = TOK_CSUB,      .content = sv_c("-=")},
			{.type = TOK_CDIV,      .content = sv_c("/=")},
			{.type = TOK_XOR,       .content = sv_c("^")},
			{.type = TOK_CXOR,      .content = sv_c("^=")},
			{.type = TOK_BOR,       .content = sv_c("|")},
			{.type = TOK_COR,       .content = sv_c("|=")},
			{.type = TOK_OR,        .content = sv_c("||")},
			{.type = TOK_BNOT,      .content = sv_c("~")},
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
				.loc.line_begin = stream->s - (ls->column_no - 1),
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


#endif
