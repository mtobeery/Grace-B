#include <stdio.h>
#include <stdlib.h>
#include "../include/ast.h"
#include "../include/symbol_table.h"
#include "../include/stack.h"

static int return_flag = 0;
static int return_value = 0;

static int call_function(ASTNode* call);

int evaluate(ASTNode* node) {
    if (!node) return 0;
    switch (node->type) {
        case AST_INT_LITERAL:
            return atoi(node->value);
        case AST_IDENTIFIER: {
            const char* val = lookup_symbol(current_symbols(), node->name);
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
        case AST_FUNCTION_CALL:
            return call_function(node);
        default:
            return 0;
    }
}

void print_ast(ASTNode* root) {
    while (root) {
        switch (root->type) {
            case AST_PRINT_STATEMENT: {
                const char* resolved = lookup_symbol(current_symbols(), root->name);
                if (resolved)
                    printf("PRINT: %s = %s\n", root->name, resolved);
                else if (root->value)
                    printf("PRINT: %s\n", root->value);
                else
                    printf("PRINT: %s\n", root->name ? root->name : "<null>");
                break;
            }
            case AST_VAR_DECL: {
                int v = evaluate(root->left);
                char buf[64];
                snprintf(buf, sizeof(buf), "%d", v);
                add_symbol(current_symbols(), root->name, buf);
                printf("VAR_DECL: int %s = %d\n", root->name, v);
                break;
            }
            case AST_FUNCTION_DECLARATION:
                add_function(root->name, root);
                printf("FN_DECL: %s\n", root->name);
                break;
            case AST_RETURN_STATEMENT:
                return_value = evaluate(root->left);
                return_flag = 1;
                return;
            case AST_FUNCTION_CALL:
                evaluate(root);
                break;
            case AST_INT_LITERAL:
                printf("INT: %s\n", root->value);
                break;
        case AST_BINARY_EXPR: {
            int v = evaluate(root);
            printf("BIN_EXPR: %d\n", v);
            break;
        }
        case AST_IF_STATEMENT: {
            int cond = evaluate(root->left);
            printf("IF (%d)\n", cond);
            if (cond) {
                print_ast(root->right);
            } else if (root->else_body) {
                print_ast(root->else_body);
            } else if (root->else_branch) {
                /* fallback for older field */
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

static int call_function(ASTNode* call) {
    FunctionSymbol* f = lookup_function(call->name);
    if (!f) return 0;
    symbol_table_t* table = create_symbol_table();
    push_frame(table);
    ASTNode* param = f->declaration->args;
    ASTNode* arg = call->args;
    while (param && arg) {
        int val = evaluate(arg);
        char buf[64];
        snprintf(buf, sizeof(buf), "%d", val);
        add_symbol(current_symbols(), param->name, buf);
        param = param->next;
        arg = arg->next;
    }
    return_flag = 0;
    return_value = 0;
    print_ast(f->declaration->right);
    pop_frame();
    return return_value;
}

static void free_node(ASTNode* node) {
    if (!node) return;
    free_node(node->left);
    free_node(node->right);
    /* else_branch is kept for backward compatibility and may alias else_body */
    if (node->else_body && node->else_body != node->else_branch)
        free_node(node->else_body);
    else if (node->else_branch)
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
