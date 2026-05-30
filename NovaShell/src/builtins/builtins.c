#include "builtins.h"
#include "core.h"
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

/* Forward declarations of handlers */
static int builtin_cd(CommandData *cmd);
static int builtin_exit(CommandData *cmd);
static int builtin_pwd(CommandData *cmd);
static int builtin_help(CommandData *cmd);
static int builtin_history(CommandData *cmd);
static int builtin_export(CommandData *cmd);
static int builtin_unset(CommandData *cmd);
static int builtin_jobs(CommandData *cmd);
static int builtin_fg(CommandData *cmd);
static int builtin_bg(CommandData *cmd);

typedef int (*BuiltinHandler)(CommandData *cmd);

typedef struct {
    const char *name;
    BuiltinHandler handler;
    const char *help_text;
} BuiltinDef;

static BuiltinDef builtins[] = {
    {"cd", builtin_cd, "Change the shell working directory."},
    {"exit", builtin_exit, "Exit the shell."},
    {"pwd", builtin_pwd, "Print current working directory."},
    {"help", builtin_help, "Display information about builtin commands."},
    {"history", builtin_history, "Display the command history."},
    {"export", builtin_export, "Set environment variable."},
    {"unset", builtin_unset, "Unset environment variable."},
    {"jobs", builtin_jobs, "List active jobs."},
    {"fg", builtin_fg, "Bring job to foreground."},
    {"bg", builtin_bg, "Resume job in background."},
    {NULL, NULL, NULL}
};

bool is_builtin(const char *cmd_name) {
    if (!cmd_name) return false;
    for (int i = 0; builtins[i].name != NULL; i++) {
        if (strcmp(cmd_name, builtins[i].name) == 0) {
            return true;
        }
    }
    return false;
}

int execute_builtin(CommandData *cmd) {
    if (!cmd || !cmd->args[0]) return 1;
    const char *name = cmd->args[0];

    for (int i = 0; builtins[i].name != NULL; i++) {
        if (strcmp(name, builtins[i].name) == 0) {
            return builtins[i].handler(cmd);
        }
    }
    return 1;
}

/* --- Handlers --- */

static int builtin_cd(CommandData *cmd) {
    const char *dir = cmd->args[1] ? cmd->args[1] : getenv("HOME");
    if (dir && chdir(dir) != 0) {
        perror("cd");
        return 1;
    }
    return 0;
}

static int builtin_exit(CommandData *cmd) {
    int status = cmd->args[1] ? atoi(cmd->args[1]) : 0;
    shell_ctx.exit_status = status;
    shell_ctx.running = 0;
    return status;
}

static int builtin_pwd(CommandData *cmd) {
    (void)cmd;
    char buf[1024];
    if (getcwd(buf, sizeof(buf))) {
        printf("%s\n", buf);
        return 0;
    }
    perror("pwd");
    return 1;
}

static int builtin_help(CommandData *cmd) {
    (void)cmd;
    printf("NovaShell - Professional POSIX Shell\n");
    printf("These shell commands are defined internally. Type 'help' to see this list.\n\n");
    for (int i = 0; builtins[i].name != NULL; i++) {
        printf("  %-10s %s\n", builtins[i].name, builtins[i].help_text);
    }
    return 0;
}

/* Stubs for unimplemented subsystems */

static int builtin_history(CommandData *cmd) {
    (void)cmd;
    printf("history: Subsystem not fully implemented yet.\n");
    return 0;
}

static int builtin_export(CommandData *cmd) {
    (void)cmd;
    printf("export: Subsystem not fully implemented yet.\n");
    return 0;
}

static int builtin_unset(CommandData *cmd) {
    (void)cmd;
    printf("unset: Subsystem not fully implemented yet.\n");
    return 0;
}

static int builtin_jobs(CommandData *cmd) {
    (void)cmd;
    printf("jobs: Subsystem not fully implemented yet.\n");
    return 0;
}

static int builtin_fg(CommandData *cmd) {
    (void)cmd;
    printf("fg: Subsystem not fully implemented yet.\n");
    return 0;
}

static int builtin_bg(CommandData *cmd) {
    (void)cmd;
    printf("bg: Subsystem not fully implemented yet.\n");
    return 0;
}
