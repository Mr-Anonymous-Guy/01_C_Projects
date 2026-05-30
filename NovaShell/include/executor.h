#ifndef NOVASHELL_EXECUTOR_H
#define NOVASHELL_EXECUTOR_H

#include "ast.h"
#include <unistd.h>
#include <sys/types.h>

/* Execution Context passed down during recursive execution */
typedef struct {
    int fd_in;       /* File descriptor for standard input */
    int fd_out;      /* File descriptor for standard output */
    int fd_err;      /* File descriptor for standard error */
    bool background; /* Is this command running in the background? */
    pid_t pgid;      /* Process group ID to join (or 0 if new) */
} ExecContext;

/* Core execution entry point */
int execute_ast(ASTNode *node);
int execute_node(ASTNode *node, ExecContext *ctx);

/* Sub-executors */
int execute_command(CommandData *cmd, ExecContext *ctx);
int execute_pipeline(ASTNode *left, ASTNode *right, ExecContext *ctx);
int execute_logical_and(ASTNode *left, ASTNode *right, ExecContext *ctx);
int execute_logical_or(ASTNode *left, ASTNode *right, ExecContext *ctx);
int execute_sequence(ASTNode *left, ASTNode *right, ExecContext *ctx);
int execute_background(ASTNode *left, ASTNode *right, ExecContext *ctx);
int execute_subshell(SubshellData *subshell, ExecContext *ctx);

/* Redirection Engine */
int apply_redirections(Redirection *redirs);

#endif /* NOVASHELL_EXECUTOR_H */
