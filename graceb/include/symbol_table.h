#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include "ast.h"

typedef struct Symbol {
    char* name;
    char* value;
    struct Symbol* next;
} Symbol;

typedef struct FunctionSymbol {
    char* name;
    ASTNode* declaration;
    struct FunctionSymbol* next;
} FunctionSymbol;

void add_symbol(const char* name, const char* value);
const char* lookup_symbol(const char* name);
void clear_symbols();
void pop_symbols(int count);

void add_function(const char* name, ASTNode* decl);
FunctionSymbol* lookup_function(const char* name);
void clear_functions();

/* Backwards compatibility */
static inline const char* get_symbol(const char* name) { return lookup_symbol(name); }
static inline void free_symbols() { clear_symbols(); }

#endif // SYMBOL_TABLE_H
