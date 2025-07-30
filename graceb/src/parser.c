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

ASTNode* parse_tokens() {
    ASTNode* head = NULL;
    ASTNode* tail = NULL;

    while (peek()->type != TOKEN_EOF) {
        Token* t = advance();

        if (t->type == TOKEN_INT) {
            Token* var = advance();
            advance();
            Token* val = advance();

            ASTNode* node = malloc(sizeof(ASTNode));
            node->type = AST_VAR_DECL;
            node->name = strdup(var->lexeme);
            node->value = strdup(val->lexeme);
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
