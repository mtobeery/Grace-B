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
            add_token(TOKEN_IDENTIFIER, word, line, col);
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

        // Single-char punctuation
        char punct[2] = {*p, '\0'};
        add_token(TOKEN_PUNCTUATION, punct, line, col);
        p++; col++;
    }

    add_token(TOKEN_EOF, "EOF", line, col);
}
