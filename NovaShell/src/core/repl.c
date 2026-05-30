#include "core.h"
#include "lexer.h"
#include "parser.h"
#include "executor.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void shell_loop(void) {
    char *line = NULL;
    size_t len = 0;
    ssize_t nread;

    while (shell_ctx.running) {
        if (isatty(STDIN_FILENO)) {
            printf("nova> ");
            fflush(stdout);
        }

        nread = getline(&line, &len, stdin);
        if (nread == -1) {
            if (isatty(STDIN_FILENO)) printf("\n");
            break; /* EOF */
        }

        /* Lexer */
        Lexer lexer;
        lexer_init(&lexer, line);
        TokenStream *stream = lexer_tokenize(&lexer);

        /* Parser */
        Parser parser;
        parser_init(&parser, stream);
        ASTNode *ast = parser_parse(&parser);

        /* Executor */
        if (ast) {
            execute_ast(ast);
            ast_free(ast);
        }

        token_stream_free(stream);
    }

    free(line);
}
