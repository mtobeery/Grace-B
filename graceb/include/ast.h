#ifndef AST_H
#define AST_H

typedef enum {
    AST_INT_LITERAL,
    AST_IDENTIFIER,
    AST_PRINT_STATEMENT,
    AST_VAR_DECL
} ASTNodeType;

typedef struct ASTNode {
    ASTNodeType type;
    char* name;
    char* value;
    struct ASTNode* next;
} ASTNode;

ASTNode* parse_tokens();
void print_ast(ASTNode* root);
void free_ast(ASTNode* root);

#endif // AST_H
