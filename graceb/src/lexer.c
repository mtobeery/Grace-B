#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../include/tokens.h"

#define MAX_TOKENS 1024

Token tokens[MAX_TOKENS];
int token_count = 0;

void add_token(TokenType type, const char* lexeme, int line, int column) {
    if (token_count >= MAX_TOKENS) return;
    tokens[token_count].type = type;
    tokens[token_count].lexeme = strdup(lexeme);
    tokens[token_count].line = line;
    tokens[token_count].column = column;
    token_count++;
}

void tokenize(const char* source) {
    int line = 1, col = 1;
    const char* p = source;

    while (*p != '\0') {
        if (isspace(*p)) {
            if (*p == '\n') { line++; col = 1; }
            else col++;
            p++;
            continue;
        }

        if (isalpha(*p)) {
            const char* start = p;
            while (isalnum(*p)) p++;
            int len = p - start;
            char* word = strndup(start, len);
            if (strcmp(word, "int") == 0) {
                add_token(TOKEN_INT, word, line, col);
            } else if (strcmp(word, "if") == 0) {
                add_token(TOKEN_IF, word, line, col);
            } else if (strcmp(word, "else") == 0) {
                add_token(TOKEN_ELSE, word, line, col);
            } else if (strcmp(word, "while") == 0) {
                add_token(TOKEN_WHILE, word, line, col);
            } else if (strcmp(word, "fn") == 0) {
                add_token(TOKEN_FN, word, line, col);
            } else if (strcmp(word, "return") == 0) {
                add_token(TOKEN_RETURN, word, line, col);
            } else {
                add_token(TOKEN_IDENTIFIER, word, line, col);
            }
            col += len;
            free(word);
            continue;
        }

        if (isdigit(*p)) {
            const char* start = p;
            while (isdigit(*p)) p++;
            int len = p - start;
            char* num = strndup(start, len);
            add_token(TOKEN_NUMBER, num, line, col);
            col += len;
            free(num);
            continue;
        }

        // Operators and punctuation
        if (*p == '=' && *(p + 1) == '=') {
            add_token(TOKEN_PUNCTUATION, "==", line, col);
            p += 2; col += 2;
            continue;
        }

        if (*p == '(') { add_token(TOKEN_LPAREN, "(", line, col); p++; col++; continue; }
        if (*p == ')') { add_token(TOKEN_RPAREN, ")", line, col); p++; col++; continue; }
        if (*p == ',') { add_token(TOKEN_COMMA, ",", line, col); p++; col++; continue; }

        char punct[2] = {*p, '\0'};
        add_token(TOKEN_PUNCTUATION, punct, line, col);
        p++; col++;
    }

    add_token(TOKEN_EOF, "EOF", line, col);
}
