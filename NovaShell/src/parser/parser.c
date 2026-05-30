#include "parser.h"
#include <stdio.h>
#include <stdlib.h>

/* Precedence levels */
enum {
    PREC_NONE = 0,
    PREC_SEQUENCE,   /* ; */
    PREC_BACKGROUND, /* & */
    PREC_LOGICAL,    /* &&, || */
    PREC_PIPELINE,   /* | */
};

static int get_precedence(TokenType type) {
    switch (type) {
        case TOKEN_SEMICOLON: return PREC_SEQUENCE;
        case TOKEN_BACKGROUND: return PREC_BACKGROUND;
        case TOKEN_AND:
        case TOKEN_OR: return PREC_LOGICAL;
        case TOKEN_PIPE: return PREC_PIPELINE;
        default: return PREC_NONE;
    }
}

void parser_init(Parser *parser, TokenStream *stream) {
    parser->stream = stream;
    parser->position = 0;
}

Token *parser_peek(Parser *parser) {
    if (parser->position >= parser->stream->count) return NULL;
    return &parser->stream->tokens[parser->position];
}

Token *parser_advance(Parser *parser) {
    if (parser->position >= parser->stream->count) return NULL;
    return &parser->stream->tokens[parser->position++];
}

bool parser_match(Parser *parser, TokenType type) {
    Token *t = parser_peek(parser);
    if (t && t->type == type) {
        parser_advance(parser);
        return true;
    }
    return false;
}

static void parse_redirections(Parser *parser, ASTNode *node) {
    while (true) {
        Token *t = parser_peek(parser);
        if (!t) break;

        int redir_type = -1;
        if (t->type == TOKEN_REDIRECT_IN) redir_type = REDIR_IN;
        else if (t->type == TOKEN_REDIRECT_OUT) redir_type = REDIR_OUT;
        else if (t->type == TOKEN_APPEND_OUT) redir_type = REDIR_APPEND;
        else if (t->type == TOKEN_REDIRECT_ERR) redir_type = REDIR_ERR;
        else if (t->type == TOKEN_REDIRECT_ERR_OUT) redir_type = REDIR_ERR_OUT;
        
        if (redir_type != -1) {
            parser_advance(parser); /* consume operator */
            Token *file = parser_advance(parser);
            if (file && file->type == TOKEN_WORD) {
                ast_add_redirection(node, redir_type, file->value);
            } else {
                /* Error: expected filename after redirection */
                /* For now, just continue, should handle robustly in real shell */
            }
        } else {
            break;
        }
    }
}

ASTNode *parse_command(Parser *parser) {
    Token *t = parser_peek(parser);
    if (!t) return NULL;

    if (t->type == TOKEN_LPAREN) {
        parser_advance(parser); /* consume '(' */
        ASTNode *expr = parse_expression(parser, PREC_NONE);
        if (!parser_match(parser, TOKEN_RPAREN)) {
            /* Error: expected ')' */
        }
        ASTNode *subshell = ast_new_subshell(expr);
        parse_redirections(parser, subshell);
        return subshell;
    }

    if (t->type != TOKEN_WORD && t->type != TOKEN_REDIRECT_IN && t->type != TOKEN_REDIRECT_OUT &&
        t->type != TOKEN_APPEND_OUT && t->type != TOKEN_REDIRECT_ERR && t->type != TOKEN_REDIRECT_ERR_OUT) {
        return NULL;
    }

    ASTNode *cmd = ast_new_command();
    
    while (true) {
        parse_redirections(parser, cmd);
        
        t = parser_peek(parser);
        if (t && t->type == TOKEN_WORD) {
            ast_add_argument(cmd, t->value);
            parser_advance(parser);
        } else if (t && (t->type == TOKEN_REDIRECT_IN || t->type == TOKEN_REDIRECT_OUT ||
                   t->type == TOKEN_APPEND_OUT || t->type == TOKEN_REDIRECT_ERR || t->type == TOKEN_REDIRECT_ERR_OUT)) {
            continue; /* Loop handles it */
        } else {
            break;
        }
    }

    return cmd;
}

ASTNode *parse_expression(Parser *parser, int precedence) {
    ASTNode *left = parse_command(parser);
    if (!left) return NULL;

    while (true) {
        Token *op = parser_peek(parser);
        if (!op || op->type == TOKEN_EOF) break;

        int op_prec = get_precedence(op->type);
        if (op_prec == PREC_NONE || op_prec < precedence) {
            break;
        }

        parser_advance(parser); /* consume operator */

        /* If operator is background '&', it doesn't strictly need a right-hand side,
         * or it's just treated as modifying the left tree and we continue. 
         * E.g., `cmd1 & cmd2` is actually two commands separated by '&'. */
        ASTNodeType node_type;
        if (op->type == TOKEN_PIPE) node_type = AST_PIPELINE;
        else if (op->type == TOKEN_AND) node_type = AST_LOGICAL_AND;
        else if (op->type == TOKEN_OR) node_type = AST_LOGICAL_OR;
        else if (op->type == TOKEN_SEMICOLON) node_type = AST_SEQUENCE;
        else if (op->type == TOKEN_BACKGROUND) node_type = AST_BACKGROUND;
        else break;

        if (node_type == AST_BACKGROUND) {
            /* '&' behaves like sequence but runs left in background */
            /* The right side might be empty (e.g. `sleep 10 &`) */
            ASTNode *right = NULL;
            Token *next = parser_peek(parser);
            if (next && next->type != TOKEN_EOF && next->type != TOKEN_RPAREN) {
                right = parse_expression(parser, op_prec);
            }
            left = ast_new_compound(node_type, left, right);
        } else {
            ASTNode *right = parse_expression(parser, op_prec + (node_type != AST_PIPELINE ? 1 : 0));
            left = ast_new_compound(node_type, left, right);
        }
    }

    return left;
}

ASTNode *parser_parse(Parser *parser) {
    if (!parser->stream || parser->stream->count == 0) return NULL;
    return parse_expression(parser, PREC_NONE);
}
