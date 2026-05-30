#include "lexer.h"
#include "tokenizer.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>

void lexer_init(Lexer *lexer, const char *input) {
    lexer->input = input;
    lexer->length = strlen(input);
    lexer->position = 0;
}

static bool is_operator_char(char c) {
    return (c == '|' || c == '<' || c == '>' || c == '&' || c == ';' || c == '(' || c == ')');
}

static bool is_whitespace(char c) {
    return (c == ' ' || c == '\t' || c == '\n' || c == '\r');
}

/* Lexer state machine */
TokenStream *lexer_tokenize(Lexer *lexer) {
    TokenStream *stream = malloc(sizeof(TokenStream));
    stream->tokens = NULL;
    stream->count = 0;
    stream->capacity = 0;

    while (lexer->position < lexer->length) {
        tokenizer_skip_whitespace(lexer);
        char c = tokenizer_peek(lexer);
        
        if (c == '\0') break;

        /* Operators */
        if (c == '|') {
            if (tokenizer_peek_next(lexer) == '|') {
                tokenizer_advance(lexer);
                tokenizer_advance(lexer);
                tokenizer_push_token(stream, TOKEN_OR, NULL, 0);
            } else {
                tokenizer_advance(lexer);
                tokenizer_push_token(stream, TOKEN_PIPE, NULL, 0);
            }
            continue;
        }
        
        if (c == '&') {
            if (tokenizer_peek_next(lexer) == '&') {
                tokenizer_advance(lexer);
                tokenizer_advance(lexer);
                tokenizer_push_token(stream, TOKEN_AND, NULL, 0);
            } else {
                tokenizer_advance(lexer);
                tokenizer_push_token(stream, TOKEN_BACKGROUND, NULL, 0);
            }
            continue;
        }

        if (c == '<') {
            tokenizer_advance(lexer);
            tokenizer_push_token(stream, TOKEN_REDIRECT_IN, NULL, 0);
            continue;
        }

        if (c == '>') {
            if (tokenizer_peek_next(lexer) == '>') {
                tokenizer_advance(lexer);
                tokenizer_advance(lexer);
                tokenizer_push_token(stream, TOKEN_APPEND_OUT, NULL, 0);
            } else {
                tokenizer_advance(lexer);
                tokenizer_push_token(stream, TOKEN_REDIRECT_OUT, NULL, 0);
            }
            continue;
        }

        if (c == '2' && tokenizer_peek_next(lexer) == '>') {
            tokenizer_advance(lexer); /* '2' */
            tokenizer_advance(lexer); /* '>' */
            if (tokenizer_peek(lexer) == '&' && tokenizer_peek_next(lexer) == '1') {
                tokenizer_advance(lexer); /* '&' */
                tokenizer_advance(lexer); /* '1' */
                tokenizer_push_token(stream, TOKEN_REDIRECT_ERR_OUT, NULL, 0);
            } else {
                tokenizer_push_token(stream, TOKEN_REDIRECT_ERR, NULL, 0);
            }
            continue;
        }

        if (c == ';') {
            tokenizer_advance(lexer);
            tokenizer_push_token(stream, TOKEN_SEMICOLON, NULL, 0);
            continue;
        }

        if (c == '(') {
            tokenizer_advance(lexer);
            tokenizer_push_token(stream, TOKEN_LPAREN, NULL, 0);
            continue;
        }

        if (c == ')') {
            tokenizer_advance(lexer);
            tokenizer_push_token(stream, TOKEN_RPAREN, NULL, 0);
            continue;
        }

        /* Parse a WORD */
        size_t start = lexer->position;
        bool in_single_quote = false;
        bool in_double_quote = false;
        bool escaped = false;
        
        while (lexer->position < lexer->length) {
            char curr = tokenizer_peek(lexer);
            
            if (!escaped && curr == '\\') {
                escaped = true;
                tokenizer_advance(lexer);
                continue;
            }

            if (!escaped && curr == '\'' && !in_double_quote) {
                in_single_quote = !in_single_quote;
            } else if (!escaped && curr == '"' && !in_single_quote) {
                in_double_quote = !in_double_quote;
            } else if (!escaped && !in_single_quote && !in_double_quote) {
                if (is_whitespace(curr) || is_operator_char(curr)) {
                    /* Special case: 2>&1 handled as an operator above, but if it starts with 2... 
                     * if it's just '2', it's a WORD. If '2>', it would have been caught above. 
                     * So this is fine. */
                    break;
                }
            }

            escaped = false;
            tokenizer_advance(lexer);
        }

        size_t length = lexer->position - start;
        if (length > 0) {
            /* Copy the raw token including quotes, expansion happens later */
            tokenizer_push_token(stream, TOKEN_WORD, lexer->input + start, length);
        }
    }
    
    tokenizer_push_token(stream, TOKEN_EOF, NULL, 0);
    return stream;
}
