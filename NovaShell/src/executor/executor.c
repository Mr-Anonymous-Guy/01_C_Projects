#include "executor.h"
#include "builtins.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

int execute_node(ASTNode *node, ExecContext *ctx) {
    if (!node) return 0;
    
    switch (node->type) {
        case AST_COMMAND:
            return execute_command(&node->data.command, ctx);
        case AST_PIPELINE:
            return execute_pipeline(node->data.compound.left, node->data.compound.right, ctx);
        case AST_LOGICAL_AND:
            return execute_logical_and(node->data.compound.left, node->data.compound.right, ctx);
        case AST_LOGICAL_OR:
            return execute_logical_or(node->data.compound.left, node->data.compound.right, ctx);
        case AST_SEQUENCE:
            return execute_sequence(node->data.compound.left, node->data.compound.right, ctx);
        case AST_BACKGROUND:
            return execute_background(node->data.compound.left, node->data.compound.right, ctx);
        case AST_SUBSHELL:
            return execute_subshell(&node->data.subshell, ctx);
    }
    return 1;
}

int execute_ast(ASTNode *node) {
    ExecContext ctx = {
        .fd_in = STDIN_FILENO,
        .fd_out = STDOUT_FILENO,
        .fd_err = STDERR_FILENO,
        .background = false,
        .pgid = 0
    };
    return execute_node(node, &ctx);
}

int execute_command(CommandData *cmd, ExecContext *ctx) {
    if (cmd->arg_count == 0) {
        /* Apply redirections and exit if it's just e.g. "> file" */
        if (apply_redirections(cmd->redirs) < 0) return 1;
        return 0;
    }

    if (is_builtin(cmd->args[0])) {
        int saved_stdin = -1, saved_stdout = -1, saved_stderr = -1;

        if (cmd->redirs) {
            saved_stdin = dup(STDIN_FILENO);
            saved_stdout = dup(STDOUT_FILENO);
            saved_stderr = dup(STDERR_FILENO);
            if (apply_redirections(cmd->redirs) < 0) {
                dup2(saved_stdin, STDIN_FILENO); close(saved_stdin);
                dup2(saved_stdout, STDOUT_FILENO); close(saved_stdout);
                dup2(saved_stderr, STDERR_FILENO); close(saved_stderr);
                return 1;
            }
        }

        int status = execute_builtin(cmd);

        if (cmd->redirs) {
            dup2(saved_stdin, STDIN_FILENO); close(saved_stdin);
            dup2(saved_stdout, STDOUT_FILENO); close(saved_stdout);
            dup2(saved_stderr, STDERR_FILENO); close(saved_stderr);
        }

        return status;
    }

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        return 1;
    }

    if (pid == 0) {
        /* Child */
        if (ctx->fd_in != STDIN_FILENO) {
            dup2(ctx->fd_in, STDIN_FILENO);
            close(ctx->fd_in);
        }
        if (ctx->fd_out != STDOUT_FILENO) {
            dup2(ctx->fd_out, STDOUT_FILENO);
            close(ctx->fd_out);
        }
        if (ctx->fd_err != STDERR_FILENO) {
            dup2(ctx->fd_err, STDERR_FILENO);
            close(ctx->fd_err);
        }

        if (apply_redirections(cmd->redirs) < 0) {
            exit(1);
        }

        /* TODO: Process groups and jobs */

        execvp(cmd->args[0], cmd->args);
        perror("execvp");
        exit(127);
    } else {
        /* Parent */
        if (ctx->fd_in != STDIN_FILENO) close(ctx->fd_in);
        if (ctx->fd_out != STDOUT_FILENO) close(ctx->fd_out);

        int status;
        waitpid(pid, &status, 0);

        if (WIFEXITED(status)) {
            return WEXITSTATUS(status);
        }
        return 1;
    }
}

int execute_logical_and(ASTNode *left, ASTNode *right, ExecContext *ctx) {
    int status = execute_node(left, ctx);
    if (status == 0) {
        return execute_node(right, ctx);
    }
    return status;
}

int execute_logical_or(ASTNode *left, ASTNode *right, ExecContext *ctx) {
    int status = execute_node(left, ctx);
    if (status != 0) {
        return execute_node(right, ctx);
    }
    return status;
}

int execute_sequence(ASTNode *left, ASTNode *right, ExecContext *ctx) {
    execute_node(left, ctx);
    return execute_node(right, ctx);
}

int execute_background(ASTNode *left, ASTNode *right, ExecContext *ctx) {
    pid_t pid = fork();
    if (pid == 0) {
        /* In background */
        /* TODO: pgid and signals */
        int status = execute_node(left, ctx);
        exit(status);
    } else if (pid > 0) {
        printf("[Background Job] %d\n", pid);
        /* Right is typically another sequence or EOF */
        if (right) {
            return execute_node(right, ctx);
        }
    } else {
        perror("fork");
    }
    return 0;
}

int execute_subshell(SubshellData *subshell, ExecContext *ctx) {
    pid_t pid = fork();
    if (pid == 0) {
        if (apply_redirections(subshell->redirs) < 0) exit(1);
        int status = execute_node(subshell->child, ctx);
        exit(status);
    } else if (pid > 0) {
        int status;
        waitpid(pid, &status, 0);
        if (WIFEXITED(status)) return WEXITSTATUS(status);
        return 1;
    }
    return 1;
}
