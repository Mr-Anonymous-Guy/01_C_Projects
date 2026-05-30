#include "executor.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>

/* Forward declaration for recursion */
extern int execute_node(ASTNode *node, ExecContext *ctx);

int execute_pipeline(ASTNode *left, ASTNode *right, ExecContext *ctx) {
    int pipefd[2];
    if (pipe(pipefd) < 0) {
        perror("pipe");
        return 1;
    }

    ExecContext left_ctx = *ctx;
    left_ctx.fd_out = pipefd[1];

    ExecContext right_ctx = *ctx;
    right_ctx.fd_in = pipefd[0];

    /* The actual process launching for pipelines should be orchestrated slightly differently
     * to avoid waiting for the left side before starting the right side.
     * execute_node for commands currently waitpid's synchronously.
     * To do this right, we need to defer waitpid, but for now we'll do a basic
     * fork of the left side, then run the right side in the parent/child. */
     
    /* Proper POSIX pipeline execution: */
    pid_t left_pid = fork();
    if (left_pid == 0) {
        /* Left child */
        close(pipefd[0]); /* Close read end */
        /* If left is another pipeline, it will naturally wire up */
        int status = execute_node(left, &left_ctx);
        exit(status);
    }
    
    /* We are in the parent. We don't wait yet. We just close the write end of the pipe
     * and execute the right side. */
    close(pipefd[1]);

    /* If right is also a pipeline, execute_node will recurse, continuing the chain */
    int right_status = execute_node(right, &right_ctx);

    /* Close read end after right is done or handed off */
    if (ctx->fd_in != STDIN_FILENO) close(ctx->fd_in);
    
    /* We should reap the left child now */
    waitpid(left_pid, NULL, 0);

    return right_status;
}
