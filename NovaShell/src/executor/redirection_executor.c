#include "executor.h"
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

int apply_redirections(Redirection *redirs) {
    Redirection *curr = redirs;
    while (curr) {
        int fd = -1;
        if (curr->type == REDIR_IN) {
            fd = open(curr->filename, O_RDONLY);
            if (fd < 0) { perror("open"); return -1; }
            dup2(fd, STDIN_FILENO);
            close(fd);
        } else if (curr->type == REDIR_OUT) {
            fd = open(curr->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd < 0) { perror("open"); return -1; }
            dup2(fd, STDOUT_FILENO);
            close(fd);
        } else if (curr->type == REDIR_APPEND) {
            fd = open(curr->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
            if (fd < 0) { perror("open"); return -1; }
            dup2(fd, STDOUT_FILENO);
            close(fd);
        } else if (curr->type == REDIR_ERR) {
            fd = open(curr->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd < 0) { perror("open"); return -1; }
            dup2(fd, STDERR_FILENO);
            close(fd);
        } else if (curr->type == REDIR_ERR_OUT) {
            dup2(STDOUT_FILENO, STDERR_FILENO);
        }
        curr = curr->next;
    }
    return 0;
}
