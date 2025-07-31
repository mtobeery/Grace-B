#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/ast.h"
#include "../include/symbol_table.h"

eval_result_t eval_node(ASTNode* node); /* forward */

static eval_result_t call_function(ASTNode* call) {
    eval_result_t result = {0, NULL, false};
    FunctionSymbol* f = lookup_function(call->name);
    if (!f)
        return result;

    ASTNode* param = f->declaration->args;
    ASTNode* arg = call->args;
    int pushed = 0;
    while (param && arg) {
        eval_result_t v = eval_node(arg);
        char buf[64];
        snprintf(buf, sizeof(buf), "%d", v.value);
        add_symbol(param->name, buf);
        if (v.str_value) free(v.str_value);
        pushed++;
        param = param->next;
        arg = arg->next;
    }

    ASTNode* stmt = f->declaration->right;
    while (stmt) {
        eval_result_t r = eval_node(stmt);
        if (r.is_return) {
            pop_symbols(pushed);
            return r;
        }
        stmt = stmt->next;
    }

    pop_symbols(pushed);
    return result;
}

eval_result_t eval_node(ASTNode* node) {
    eval_result_t result = {0, NULL, false};
    if (!node)
        return result;

    switch (node->type) {
        case AST_INT_LITERAL:
            result.value = atoi(node->value);
            break;
        case AST_IDENTIFIER: {
            const char* val = lookup_symbol(node->name);
            result.value = val ? atoi(val) : 0;
            break;
        }
        case AST_BINARY_EXPR: {
            eval_result_t l = eval_node(node->left);
            eval_result_t r = eval_node(node->right);
            if (node->op == '+')
                result.value = l.value + r.value;
            else if (node->op == '-')
                result.value = l.value - r.value;
            else if (node->op == '=')
                result.value = (l.value == r.value);
            break;
        }
        case AST_LIST_LITERAL: {
            size_t cap = node->list_length * 16 + 2;
            char* buf = malloc(cap);
            if (!buf) break;
            size_t pos = 0;
            buf[pos++] = '[';
            for (int i = 0; i < node->list_length; i++) {
                eval_result_t v = eval_node(node->list_values[i]);
                char tmp[64];
                snprintf(tmp, sizeof(tmp), "%d", v.value);
                size_t len = strlen(tmp);
                if (pos + len + 2 >= cap) {
                    cap = cap * 2 + len + 2;
                    buf = realloc(buf, cap);
                }
                memcpy(buf + pos, tmp, len);
                pos += len;
                if (i != node->list_length - 1) {
                    buf[pos++] = ',';
                    buf[pos++] = ' ';
                }
            }
            if (pos + 2 > cap) buf = realloc(buf, pos + 2);
            buf[pos++] = ']';
            buf[pos] = '\0';
            result.str_value = buf;
            break;
        }
        case AST_PRINT_STATEMENT: {
            const char* resolved = lookup_symbol(node->name);
            if (resolved)
                printf("%s\n", resolved);
            else if (node->value)
                printf("%s\n", node->value);
            else
                printf("%s\n", node->name ? node->name : "0");
            break;
        }
        case AST_VAR_DECL: {
            eval_result_t v = eval_node(node->left);
            if (node->left->type == AST_LIST_LITERAL && v.str_value) {
                add_symbol(node->name, v.str_value);
                free(v.str_value);
            } else {
                char buf[64];
                snprintf(buf, sizeof(buf), "%d", v.value);
                add_symbol(node->name, buf);
            }
            break;
        }
        case AST_FUNCTION_DECLARATION:
            add_function(node->name, node);
            break;
        case AST_RETURN_STATEMENT: {
            eval_result_t v = eval_node(node->left);
            result.value = v.value;
            result.str_value = v.str_value;
            result.is_return = true;
            return result;
        }
        case AST_FUNCTION_CALL:
            return call_function(node);
        case AST_IF_STATEMENT: {
            eval_result_t cond = eval_node(node->left);
            ASTNode* branch = cond.value ? node->right : (node->else_body ? node->else_body : node->else_branch);
            ASTNode* stmt = branch;
            while (stmt) {
                eval_result_t r = eval_node(stmt);
                if (r.is_return)
                    return r;
                stmt = stmt->next;
            }
            break;
        }
        case AST_WHILE_STATEMENT: {
            while (1) {
                eval_result_t cond = eval_node(node->left);
                if (!cond.value)
                    break;
                ASTNode* stmt = node->right;
                while (stmt) {
                    eval_result_t r = eval_node(stmt);
                    if (r.is_return)
                        return r;
                    stmt = stmt->next;
                }
            }
            break;
        }
        default:
            break;
    }

    return result;
}

void eval_program(ASTNode* root) {
    ASTNode* cur = root;
    while (cur) {
        eval_node(cur);
        cur = cur->next;
    }
}

static void free_node(ASTNode* node) {
    if (!node) return;
    free_node(node->left);
    free_node(node->right);
    if (node->else_body && node->else_body != node->else_branch)
        free_node(node->else_body);
    else if (node->else_branch)
        free_node(node->else_branch);
    free_node(node->args);
    if (node->type == AST_LIST_LITERAL && node->list_values) {
        for (int i = 0; i < node->list_length; i++)
            free_node(node->list_values[i]);
        free(node->list_values);
    }
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
