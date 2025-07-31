#include <stdio.h>
#include <stdlib.h>
#include "../include/tokens.h"
#include "../include/ast.h"
#include "../include/symbol_table.h"

void tokenize(const char* source);

int main(int argc, char** argv) {
    if (argc < 2) {
        printf("Usage: graceb <file.b>\n");
        return 1;
    }

    FILE* f = fopen(argv[1], "r");
    if (!f) {
        perror("Error opening file");
        return 1;
    }

    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    fseek(f, 0, SEEK_SET);
    char* source = malloc(size + 1);
    fread(source, 1, size, f);
    source[size] = '\0';
    fclose(f);

    tokenize(source);

    ASTNode* root = parse_tokens();
    eval_program(root);
    free_ast(root);
    clear_symbols();
    clear_functions();

    free(source);
    return 0;
}
