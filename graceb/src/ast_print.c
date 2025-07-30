#include <stdio.h>
#include <stdlib.h>
#include "../include/ast.h"
#include "../include/symbol_table.h"

void print_ast(ASTNode* root) {
    while (root) {
        switch (root->type) {
            case AST_PRINT_STATEMENT: {
                const char* resolved = lookup_symbol(root->name);
                if (resolved)
                    printf("PRINT: %s = %s\n", root->name, resolved);
                else if (root->value)
                    printf("PRINT: %s\n", root->value);
                else
                    printf("PRINT: %s\n", root->name ? root->name : "<null>");
                break;
            }
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
