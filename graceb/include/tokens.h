#ifndef TOKENS_H
#define TOKENS_H

typedef enum {
    TOKEN_EOF,
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,
    TOKEN_STRING,
    TOKEN_OPERATOR,
    TOKEN_KEYWORD,
    TOKEN_PUNCTUATION,
    TOKEN_INT
} TokenType;

typedef struct {
    TokenType type;
    char*     lexeme;
    int       line;
    int       column;
} Token;

#endif // TOKENS_H
