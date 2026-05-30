#include "lexer.h"
#include <stdlib.h>
#include <string.h>

void token_stream_free(TokenStream *stream) {
    if (!stream) return;
    for (size_t i = 0; i < stream->count; i++) {
        token_free(&stream->tokens[i]);
    }
    free(stream->tokens);
    free(stream);
}

void token_free(Token *token) {
    if (!token) return;
    if (token->value) {
        free(token->value);
        token->value = NULL;
    }
}
