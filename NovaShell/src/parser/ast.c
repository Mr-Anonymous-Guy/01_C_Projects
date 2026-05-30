#include "ast.h"
#include <stdlib.h>
#include <string.h>

ASTNode *ast_new_command(void) {
    ASTNode *node = calloc(1, sizeof(ASTNode));
    node->type = AST_COMMAND;
    node->data.command.args = NULL;
    node->data.command.arg_count = 0;
    node->data.command.redirs = NULL;
    return node;
}

ASTNode *ast_new_compound(ASTNodeType type, ASTNode *left, ASTNode *right) {
    ASTNode *node = calloc(1, sizeof(ASTNode));
    node->type = type;
    node->data.compound.left = left;
    node->data.compound.right = right;
    return node;
}

ASTNode *ast_new_subshell(ASTNode *child) {
    ASTNode *node = calloc(1, sizeof(ASTNode));
    node->type = AST_SUBSHELL;
    node->data.subshell.child = child;
    node->data.subshell.redirs = NULL;
    return node;
}

void ast_add_argument(ASTNode *cmd_node, const char *arg) {
    if (cmd_node->type != AST_COMMAND) return;
    
    CommandData *cmd = &cmd_node->data.command;
    cmd->arg_count++;
    cmd->args = realloc(cmd->args, (cmd->arg_count + 1) * sizeof(char *));
    cmd->args[cmd->arg_count - 1] = strdup(arg);
    cmd->args[cmd->arg_count] = NULL;
}

void ast_add_redirection(ASTNode *node, int type, const char *filename) {
    Redirection *redir = calloc(1, sizeof(Redirection));
    redir->type = type;
    redir->filename = strdup(filename);
    redir->next = NULL;

    Redirection **head = NULL;
    if (node->type == AST_COMMAND) {
        head = &node->data.command.redirs;
    } else if (node->type == AST_SUBSHELL) {
        head = &node->data.subshell.redirs;
    } else {
        free(redir->filename);
        free(redir);
        return;
    }

    if (!*head) {
        *head = redir;
    } else {
        Redirection *curr = *head;
        while (curr->next) curr = curr->next;
        curr->next = redir;
    }
}

static void free_redirections(Redirection *redir) {
    while (redir) {
        Redirection *next = redir->next;
        free(redir->filename);
        free(redir);
        redir = next;
    }
}

void ast_free(ASTNode *node) {
    if (!node) return;

    switch (node->type) {
        case AST_COMMAND: {
            if (node->data.command.args) {
                for (size_t i = 0; i < node->data.command.arg_count; i++) {
                    free(node->data.command.args[i]);
                }
                free(node->data.command.args);
            }
            free_redirections(node->data.command.redirs);
            break;
        }
        case AST_PIPELINE:
        case AST_LOGICAL_AND:
        case AST_LOGICAL_OR:
        case AST_BACKGROUND:
        case AST_SEQUENCE: {
            ast_free(node->data.compound.left);
            ast_free(node->data.compound.right);
            break;
        }
        case AST_SUBSHELL: {
            ast_free(node->data.subshell.child);
            free_redirections(node->data.subshell.redirs);
            break;
        }
    }
    free(node);
}
