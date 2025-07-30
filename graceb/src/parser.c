#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/tokens.h"
#include "../include/ast.h"
#include "../include/symbol_table.h"

extern Token tokens[];
extern int token_count;

static int current = 0;

static Token* peek() {
    return &tokens[current];
}

static Token* advance() {
    return &tokens[current++];
}

static ASTNode* parse_primary() {
    Token* t = advance();
    ASTNode* node = malloc(sizeof(ASTNode));
    memset(node, 0, sizeof(ASTNode));

    if (t->type == TOKEN_NUMBER) {
        node->type = AST_INT_LITERAL;
        node->value = strdup(t->lexeme);
    } else if (t->type == TOKEN_IDENTIFIER) {
        node->type = AST_IDENTIFIER;
        node->name = strdup(t->lexeme);
    } else {
        free(node);
        return NULL;
    }

    return node;
}

static ASTNode* parse_expression() {
    ASTNode* left = parse_primary();
    while (peek()->type == TOKEN_PUNCTUATION &&
           (strcmp(peek()->lexeme, "+") == 0 || strcmp(peek()->lexeme, "-") == 0)) {
        char op = peek()->lexeme[0];
        advance();
        ASTNode* right = parse_primary();
        ASTNode* expr = malloc(sizeof(ASTNode));
        memset(expr, 0, sizeof(ASTNode));
        expr->type = AST_BINARY_EXPR;
        expr->op = op;
        expr->left = left;
        expr->right = right;
        left = expr;
    }
    return left;
}

ASTNode* parse_tokens() {
    ASTNode* head = NULL;
    ASTNode* tail = NULL;

    while (peek()->type != TOKEN_EOF) {
        Token* t = advance();

        if (t->type == TOKEN_INT) {
            Token* var = advance();
            advance();
            ASTNode* expr = parse_expression();

            int result = evaluate(expr);
            char buffer[64];
            snprintf(buffer, sizeof(buffer), "%d", result);

            ASTNode* node = malloc(sizeof(ASTNode));
            memset(node, 0, sizeof(ASTNode));
            node->type = AST_VAR_DECL;
            node->name = strdup(var->lexeme);
            node->value = strdup(buffer);
            node->left = expr;
            node->next = NULL;

            add_symbol(node->name, node->value);

            if (!head) head = node;
            else tail->next = node;
            tail = node;
            continue;
        }

        if (strcmp(t->lexeme, "print") == 0) {
            Token* next = advance();
            ASTNode* node = malloc(sizeof(ASTNode));
            memset(node, 0, sizeof(ASTNode));
            node->type = AST_PRINT_STATEMENT;
            node->name = strdup(next->lexeme);
            node->value = NULL;
            node->next = NULL;

            if (!head) head = node;
            else tail->next = node;
            tail = node;
        }
    }

    return head;
}
