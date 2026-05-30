#ifndef NOVASHELL_BUILTINS_H
#define NOVASHELL_BUILTINS_H

#include "ast.h"

int execute_builtin(CommandData *cmd);
bool is_builtin(const char *cmd_name);

#endif /* NOVASHELL_BUILTINS_H */
