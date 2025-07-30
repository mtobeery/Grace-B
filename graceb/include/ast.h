#ifndef AST_H
#define AST_H

typedef enum {
    AST_INT_LITERAL,
    AST_IDENTIFIER,
    AST_PRINT_STATEMENT
} ASTNodeType;

typedef struct ASTNode {
    ASTNodeType type;
    char* value;
    struct ASTNode* next;
} ASTNode;

ASTNode* parse_tokens();
void print_ast(ASTNode* root);
void free_ast(ASTNode* root);

#endif // AST_H
