#include <stdio.h>
#include <stdlib.h>
#include "../include/ast.h"

void print_ast(ASTNode* root) {
    while (root) {
        switch (root->type) {
            case AST_PRINT_STATEMENT:
                printf("PRINT: %s\n", root->value);
                break;
            case AST_VAR_DECL:
                printf("VAR_DECL: int %s = %s\n", root->name, root->value);
                break;
            case AST_INT_LITERAL:
                printf("INT: %s\n", root->value);
                break;
            default:
                printf("UNKNOWN AST NODE\n");
        }
        root = root->next;
    }
}

void free_ast(ASTNode* root) {
    while (root) {
        ASTNode* next = root->next;
        free(root->name);
        free(root->value);
        free(root);
        root = next;
    }
}
