#ifndef NOVASHELL_AST_H
#define NOVASHELL_AST_H

#include <stddef.h>
#include <stdbool.h>

typedef enum {
    AST_COMMAND,
    AST_PIPELINE,
    AST_LOGICAL_AND,
    AST_LOGICAL_OR,
    AST_BACKGROUND,
    AST_SUBSHELL,
    AST_SEQUENCE,      /* commands separated by ';' */
} ASTNodeType;

typedef struct ASTNode ASTNode;

/* Redirection structure */
typedef struct Redirection {
    enum { REDIR_IN, REDIR_OUT, REDIR_APPEND, REDIR_ERR, REDIR_ERR_OUT } type;
    char *filename;
    struct Redirection *next;
} Redirection;

/* Command Node Data */
typedef struct {
    char **args;       /* NULL-terminated array of arguments */
    size_t arg_count;
    Redirection *redirs;
} CommandData;

/* Compound Node Data (Pipeline, Logical, etc.) */
typedef struct {
    ASTNode *left;
    ASTNode *right;
} CompoundData;

/* Subshell Node Data */
typedef struct {
    ASTNode *child;
    Redirection *redirs;
} SubshellData;

struct ASTNode {
    ASTNodeType type;
    union {
        CommandData command;
        CompoundData compound;
        SubshellData subshell;
    } data;
};

/* AST Constructors */
ASTNode *ast_new_command(void);
ASTNode *ast_new_compound(ASTNodeType type, ASTNode *left, ASTNode *right);
ASTNode *ast_new_subshell(ASTNode *child);
void ast_add_argument(ASTNode *cmd_node, const char *arg);
void ast_add_redirection(ASTNode *node, int type, const char *filename);

/* Memory Management */
void ast_free(ASTNode *node);

#endif /* NOVASHELL_AST_H */
