#ifndef NOVASHELL_PARSER_H
#define NOVASHELL_PARSER_H

#include "lexer.h"
#include "ast.h"

typedef struct {
    TokenStream *stream;
    size_t position;
} Parser;

void parser_init(Parser *parser, TokenStream *stream);
ASTNode *parser_parse(Parser *parser);

/* Grammar functions exposed for internal use */
ASTNode *parse_expression(Parser *parser, int precedence);
ASTNode *parse_command(Parser *parser);

/* Helpers */
Token *parser_peek(Parser *parser);
Token *parser_advance(Parser *parser);
bool parser_match(Parser *parser, TokenType type);

#endif /* NOVASHELL_PARSER_H */
