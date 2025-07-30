#include <stdio.h>
#include <stdlib.h>
#include "../include/tokens.h"

extern Token tokens[];
extern int token_count;
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

    for (int i = 0; i < token_count; i++) {
        printf("Token[%d]: %s (type %d) at line %d col %d\n",
               i,
               tokens[i].lexeme,
               tokens[i].type,
               tokens[i].line,
               tokens[i].column);
    }

    free(source);
    return 0;
}
