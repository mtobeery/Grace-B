#ifndef AST_H
#define AST_H

typedef enum {
    AST_INT_LITERAL,
    AST_IDENTIFIER,
    AST_BINARY_EXPR,
    AST_PRINT_STATEMENT,
    AST_VAR_DECL
} ASTNodeType;

typedef struct ASTNode {
    ASTNodeType type;
    char* name;
    char* value;
    char op;
    struct ASTNode* left;
    struct ASTNode* right;
    struct ASTNode* next;
} ASTNode;

ASTNode* parse_tokens();
void print_ast(ASTNode* root);
void free_ast(ASTNode* root);
int evaluate(ASTNode* node);

#endif // AST_H
