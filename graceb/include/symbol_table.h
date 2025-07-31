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

typedef struct symbol_table {
    Symbol* head;
} symbol_table_t;

symbol_table_t* create_symbol_table();
void free_symbol_table(symbol_table_t* table);
void add_symbol(symbol_table_t* table, const char* name, const char* value);
const char* lookup_symbol(symbol_table_t* table, const char* name);

void add_function(const char* name, ASTNode* decl);
FunctionSymbol* lookup_function(const char* name);
void clear_functions();

#endif // SYMBOL_TABLE_H
