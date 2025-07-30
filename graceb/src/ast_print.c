#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/ast.h"
#include "../include/symbol_table.h"

int evaluate(ASTNode* node) {
    if (!node) return 0;
    switch (node->type) {
        case AST_INT_LITERAL:
            return atoi(node->value);
        case AST_IDENTIFIER: {
            const char* val = lookup_symbol(node->name);
            return val ? atoi(val) : 0;
        }
        case AST_BINARY_EXPR: {
            int l = evaluate(node->left);
            int r = evaluate(node->right);
            if (node->op == '+') return l + r;
            if (node->op == '-') return l - r;
            if (node->op == '=') return l == r;
            return 0;
        }
        case AST_FUNCTION_CALL_EXPR: {
            /* simple built-in functions */
            if (!node->name) return 0;
            int arg1 = 0, arg2 = 0;
            if (node->args) {
                arg1 = evaluate(node->args);
                if (node->args->next)
                    arg2 = evaluate(node->args->next);
            }
            if (strcmp(node->name, "add") == 0) return arg1 + arg2;
            if (strcmp(node->name, "multiply") == 0) return arg1 * arg2;
            return 0;
        }
        default:
            return 0;
    }
}

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
        case AST_BINARY_EXPR: {
            int v = evaluate(root);
            printf("BIN_EXPR: %d\n", v);
            break;
        }
        case AST_FUNCTION_CALL_EXPR: {
            int v = evaluate(root);
            printf("CALL %s => %d\n", root->name, v);
            break;
        }
        case AST_IF_STATEMENT: {
            int cond = evaluate(root->left);
            printf("IF (%d)\n", cond);
            if (cond) {
                print_ast(root->right);
            } else if (root->else_branch) {
                print_ast(root->else_branch);
            }
            break;
        }
        case AST_WHILE_STATEMENT: {
            int cond = evaluate(root->left);
            printf("WHILE (%d)\n", cond);
            while (evaluate(root->left)) {
                print_ast(root->right);
            }
            break;
        }
        default:
            printf("UNKNOWN AST NODE\n");
        }
        root = root->next;
    }
}

static void free_node(ASTNode* node) {
    if (!node) return;
    free_node(node->left);
    free_node(node->right);
    free_node(node->else_branch);
    free_node(node->args);
    free(node->name);
    free(node->value);
    free(node);
}

void free_ast(ASTNode* root) {
    while (root) {
        ASTNode* next = root->next;
        free_node(root);
        root = next;
    }
}
