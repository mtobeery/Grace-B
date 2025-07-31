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

static ASTNode* parse_expression();

static ASTNode* parse_argument_list() {
    if (peek()->type == TOKEN_RPAREN) return NULL;
    ASTNode* head = parse_expression();
    ASTNode* tail = head;
    while (peek()->type == TOKEN_COMMA) {
        advance();
        ASTNode* arg = parse_expression();
        tail->next = arg;
        tail = arg;
    }
    return head;
}

static ASTNode* parse_param_list() {
    if (peek()->type == TOKEN_RPAREN) return NULL;
    ASTNode* head = NULL;
    ASTNode* tail = NULL;
    while (peek()->type == TOKEN_IDENTIFIER) {
        Token* t = advance();
        ASTNode* id = calloc(1, sizeof(ASTNode));
        id->type = AST_IDENTIFIER;
        id->name = strdup(t->lexeme);
        if (!head) head = id; else tail->next = id;
        tail = id;
        if (peek()->type == TOKEN_COMMA) advance();
        else break;
    }
    return head;
}

static ASTNode* parse_primary() {
    Token* t = advance();
    ASTNode* node = calloc(1, sizeof(ASTNode));

    if (t->type == TOKEN_NUMBER) {
        node->type = AST_INT_LITERAL;
        node->value = strdup(t->lexeme);
    } else if (t->type == TOKEN_IDENTIFIER) {
        if (peek()->type == TOKEN_LPAREN) {
            advance();
            ASTNode* args = parse_argument_list();
            if (peek()->type == TOKEN_RPAREN) advance();
            node->type = AST_FUNCTION_CALL;
            node->name = strdup(t->lexeme);
            node->args = args;
        } else {
            node->type = AST_IDENTIFIER;
            node->name = strdup(t->lexeme);
        }
    } else if (t->type == TOKEN_LPAREN) {
        node = parse_expression();
        if (peek()->type == TOKEN_RPAREN) advance();
        return node;
    } else if (t->type == TOKEN_LBRACKET) {
        node->type = AST_LIST_LITERAL;
        node->list_length = 0;
        node->list_values = NULL;
        int capacity = 0;
        if (peek()->type != TOKEN_RBRACKET) {
            while (1) {
                ASTNode* elem = parse_expression();
                if (node->list_length >= capacity) {
                    capacity = capacity ? capacity * 2 : 4;
                    node->list_values = realloc(node->list_values, sizeof(ASTNode*) * capacity);
                }
                node->list_values[node->list_length++] = elem;
                if (peek()->type == TOKEN_COMMA) {
                    advance();
                    continue;
                }
                break;
            }
        }
        if (peek()->type == TOKEN_RBRACKET) advance();
        return node;
    } else {
        free(node);
        return NULL;
    }

    return node;
}

static ASTNode* parse_expression() {
    ASTNode* left = parse_primary();
    while (peek()->type == TOKEN_PUNCTUATION &&
           (strcmp(peek()->lexeme, "+") == 0 ||
            strcmp(peek()->lexeme, "-") == 0 ||
            strcmp(peek()->lexeme, "==") == 0)) {
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

static ASTNode* parse_statement();

ASTNode* parse_tokens() {
    ASTNode* head = NULL;
    ASTNode* tail = NULL;

    while (peek()->type != TOKEN_EOF) {
        ASTNode* stmt = parse_statement();
        if (!stmt) break;

        if (!head) head = stmt;
        else tail->next = stmt;
        tail = stmt;
    }

    return head;
}

static ASTNode* parse_statement() {
    Token* t = advance();

    if (t->type == TOKEN_FN) {
        Token* name_tok = advance();
        advance(); // LPAREN
        ASTNode* params = parse_param_list();
        if (peek()->type == TOKEN_RPAREN) advance();
        ASTNode* body = parse_statement();

        ASTNode* node = calloc(1, sizeof(ASTNode));
        node->type = AST_FUNCTION_DECLARATION;
        node->name = strdup(name_tok->lexeme);
        node->args = params;
        node->right = body;
        node->next = NULL;

        return node;
    }

    if (t->type == TOKEN_RETURN) {
        ASTNode* expr = parse_expression();
        ASTNode* node = calloc(1, sizeof(ASTNode));
        node->type = AST_RETURN_STATEMENT;
        node->left = expr;
        return node;
    }

    if (t->type == TOKEN_INT) {
        Token* var = advance();
        advance();
        ASTNode* expr = parse_expression();

        ASTNode* node = calloc(1, sizeof(ASTNode));
        node->type = AST_VAR_DECL;
        node->name = strdup(var->lexeme);
        node->left = expr;
        node->next = NULL;

        return node;
    }

    if (t->type == TOKEN_IF || (t->type == TOKEN_IDENTIFIER && strcmp(t->lexeme, "if") == 0)) {
        ASTNode* condition = parse_expression();
        ASTNode* body = parse_statement();
        ASTNode* else_body = NULL;
        if (peek()->type == TOKEN_ELSE ||
            (peek()->type == TOKEN_IDENTIFIER && strcmp(peek()->lexeme, "else") == 0)) {
            advance();
            else_body = parse_statement();
        }

        ASTNode* node = malloc(sizeof(ASTNode));
        memset(node, 0, sizeof(ASTNode));
        node->type = AST_IF_STATEMENT;
        node->left = condition;
        node->right = body;
        node->else_branch = else_body;
        node->else_body = else_body; /* keep compatibility */
        node->next = NULL;

        return node;
    }

    if (t->type == TOKEN_WHILE || (t->type == TOKEN_IDENTIFIER && strcmp(t->lexeme, "while") == 0)) {
        ASTNode* condition = parse_expression();
        ASTNode* body = parse_statement();

        ASTNode* node = malloc(sizeof(ASTNode));
        memset(node, 0, sizeof(ASTNode));
        node->type = AST_WHILE_STATEMENT;
        node->left = condition;
        node->right = body;
        node->next = NULL;

        return node;
    }

    if (strcmp(t->lexeme, "print") == 0) {
        Token* next = advance();
        ASTNode* node = malloc(sizeof(ASTNode));
        memset(node, 0, sizeof(ASTNode));
        node->type = AST_PRINT_STATEMENT;
        node->name = strdup(next->lexeme);
        node->value = NULL;
        node->next = NULL;

        return node;
    }

    return NULL;
}
