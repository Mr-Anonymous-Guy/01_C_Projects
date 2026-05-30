#ifndef NOVASHELL_JOBS_H
#define NOVASHELL_JOBS_H

#include <sys/types.h>
#include <termios.h>
#include <stdbool.h>

extern pid_t shell_pgid;
extern struct termios shell_tmodes;
extern int shell_terminal;
extern int shell_is_interactive;

void init_shell_terminal(void);
void set_terminal_foreground(pid_t pgid);

#endif /* NOVASHELL_JOBS_H */
