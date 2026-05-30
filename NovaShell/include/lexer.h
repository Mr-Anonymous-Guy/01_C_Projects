#ifndef NOVASHELL_LEXER_H
#define NOVASHELL_LEXER_H

#include <stddef.h>
#include <stdbool.h>

typedef enum {
    TOKEN_EOF = 0,
    TOKEN_WORD,           /* ls, cat, args */
    TOKEN_PIPE,           /* | */
    TOKEN_REDIRECT_IN,    /* < */
    TOKEN_REDIRECT_OUT,   /* > */
    TOKEN_APPEND_OUT,     /* >> */
    TOKEN_REDIRECT_ERR,   /* 2> */
    TOKEN_REDIRECT_ERR_OUT, /* 2>&1 */
    TOKEN_BACKGROUND,     /* & */
    TOKEN_SEMICOLON,      /* ; */
    TOKEN_AND,            /* && */
    TOKEN_OR,             /* || */
    TOKEN_LPAREN,         /* ( */
    TOKEN_RPAREN,         /* ) */
    TOKEN_ERROR           /* Error occurred during lexing */
} TokenType;

typedef struct {
    TokenType type;
    char *value;          /* Allocated string, or NULL if not a WORD */
} Token;

typedef struct {
    Token *tokens;
    size_t count;
    size_t capacity;
} TokenStream;

/* Lexer State */
typedef struct {
    const char *input;
    size_t length;
    size_t position;
} Lexer;

/* Initialization and Teardown */
void lexer_init(Lexer *lexer, const char *input);
TokenStream *lexer_tokenize(Lexer *lexer);

void token_stream_free(TokenStream *stream);
void token_free(Token *token);

#endif /* NOVASHELL_LEXER_H */
