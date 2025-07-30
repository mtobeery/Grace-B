#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

typedef struct {
    char* name;
    char* value;
} Symbol;

void add_symbol(const char* name, const char* value);
const char* get_symbol(const char* name);
void free_symbols();

#endif // SYMBOL_TABLE_H
