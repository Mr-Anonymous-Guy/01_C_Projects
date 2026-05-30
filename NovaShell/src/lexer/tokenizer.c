#include "lexer.h"
#include "tokenizer.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void tokenizer_push_token(TokenStream *stream, TokenType type, const char *value, size_t val_len) {
    if (stream->count >= stream->capacity) {
        stream->capacity = (stream->capacity == 0) ? 16 : stream->capacity * 2;
        stream->tokens = realloc(stream->tokens, stream->capacity * sizeof(Token));
    }
    Token *t = &stream->tokens[stream->count++];
    t->type = type;
    if (value) {
        t->value = malloc(val_len + 1);
        strncpy(t->value, value, val_len);
        t->value[val_len] = '\0';
    } else {
        t->value = NULL;
    }
}

char tokenizer_peek(Lexer *lexer) {
    if (lexer->position >= lexer->length) return '\0';
    return lexer->input[lexer->position];
}

char tokenizer_peek_next(Lexer *lexer) {
    if (lexer->position + 1 >= lexer->length) return '\0';
    return lexer->input[lexer->position + 1];
}

char tokenizer_advance(Lexer *lexer) {
    if (lexer->position >= lexer->length) return '\0';
    return lexer->input[lexer->position++];
}

void tokenizer_skip_whitespace(Lexer *lexer) {
    while (lexer->position < lexer->length) {
        char c = lexer->input[lexer->position];
        if (c == ' ' || c == '\t' || c == '\n' || c == '\r') {
            lexer->position++;
        } else {
            break;
        }
    }
}
