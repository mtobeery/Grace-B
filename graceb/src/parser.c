#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/tokens.h"
#include "../include/ast.h"

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

        if (strcmp(t->lexeme, "print") == 0) {
            Token* next = advance();
            ASTNode* node = malloc(sizeof(ASTNode));
            node->type = AST_PRINT_STATEMENT;
            node->value = strdup(next->lexeme);
            node->next = NULL;

            if (!head) head = node;
            else tail->next = node;
            tail = node;
        }
    }

    return head;
}
