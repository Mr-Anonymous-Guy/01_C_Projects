#ifndef NOVASHELL_TOKENIZER_H
#define NOVASHELL_TOKENIZER_H

#include "lexer.h"

void tokenizer_push_token(TokenStream *stream, TokenType type, const char *value, size_t val_len);
char tokenizer_peek(Lexer *lexer);
char tokenizer_peek_next(Lexer *lexer);
char tokenizer_advance(Lexer *lexer);
void tokenizer_skip_whitespace(Lexer *lexer);

#endif /* NOVASHELL_TOKENIZER_H */
