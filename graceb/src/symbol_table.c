#include <stdlib.h>
#include <string.h>
#include "../include/symbol_table.h"

#define MAX_SYMBOLS 256

static Symbol symbols[MAX_SYMBOLS];
static int symbol_count = 0;

void add_symbol(const char* name, const char* value) {
    if (symbol_count >= MAX_SYMBOLS) return;
    symbols[symbol_count].name = strdup(name);
    symbols[symbol_count].value = strdup(value);
    symbol_count++;
}

const char* get_symbol(const char* name) {
    for (int i = 0; i < symbol_count; i++) {
        if (strcmp(symbols[i].name, name) == 0) {
            return symbols[i].value;
        }
    }
    return NULL;
}

void free_symbols() {
    for (int i = 0; i < symbol_count; i++) {
        free(symbols[i].name);
        free(symbols[i].value);
    }
    symbol_count = 0;
}
