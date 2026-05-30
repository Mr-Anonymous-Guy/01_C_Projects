#ifndef NOVASHELL_CORE_H
#define NOVASHELL_CORE_H

typedef struct {
    int running;
    int exit_status;
} ShellContext;

extern ShellContext shell_ctx;

void shell_loop(void);

#endif /* NOVASHELL_CORE_H */
