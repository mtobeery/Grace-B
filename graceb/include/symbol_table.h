#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include "ast.h"

typedef struct Symbol {
    char* name;
    char* value;
    struct Symbol* next;
} Symbol;

typedef struct Function {
    char* name;
    struct ASTNode* body;
    struct Function* next;
} Function;

void add_symbol(const char* name, const char* value);
const char* lookup_symbol(const char* name);
void add_function(const char* name, struct ASTNode* body);
struct ASTNode* lookup_function(const char* name);
void clear_symbols();

/* Backwards compatibility */
static inline const char* get_symbol(const char* name) { return lookup_symbol(name); }
static inline void free_symbols() { clear_symbols(); }

#endif // SYMBOL_TABLE_H
