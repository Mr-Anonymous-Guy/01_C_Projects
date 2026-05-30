#include "core.h"
#include "jobs.h"
#include <stdio.h>
#include <stdlib.h>

ShellContext shell_ctx;

int main(int argc, char **argv) {
    /* Ignore argc and argv for now */
    (void)argc;
    (void)argv;

    /* Initialize terminal and process group */
    init_shell_terminal();

    /* Initialize shell context */
    shell_ctx.running = 1;
    shell_ctx.exit_status = 0;

    /* Start REPL */
    shell_loop();

    return shell_ctx.exit_status;
}
