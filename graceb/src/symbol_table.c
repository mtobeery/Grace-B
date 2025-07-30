#include <stdlib.h>
#include <string.h>
#include "../include/symbol_table.h"

static Symbol* head = NULL;
static FunctionSymbol* func_head = NULL;

void add_symbol(const char* name, const char* value) {
    Symbol* s = malloc(sizeof(Symbol));
    if (!s) return;
    s->name = strdup(name);
    s->value = strdup(value);
    s->next = head;
    head = s;
}

void pop_symbols(int count) {
    for (int i = 0; i < count && head; i++) {
        Symbol* tmp = head;
        head = head->next;
        free(tmp->name);
        free(tmp->value);
        free(tmp);
    }
}

const char* lookup_symbol(const char* name) {
    Symbol* cur = head;
    while (cur) {
        if (strcmp(cur->name, name) == 0) {
            return cur->value;
        }
        cur = cur->next;
    }
    return NULL;
}

void clear_symbols() {
    Symbol* cur = head;
    while (cur) {
        Symbol* tmp = cur;
        cur = cur->next;
        free(tmp->name);
        free(tmp->value);
        free(tmp);
    }
    head = NULL;
}

void add_function(const char* name, ASTNode* decl) {
    FunctionSymbol* f = malloc(sizeof(FunctionSymbol));
    if (!f) return;
    f->name = strdup(name);
    f->declaration = decl;
    f->next = func_head;
    func_head = f;
}

FunctionSymbol* lookup_function(const char* name) {
    FunctionSymbol* cur = func_head;
    while (cur) {
        if (strcmp(cur->name, name) == 0) return cur;
        cur = cur->next;
    }
    return NULL;
}

void clear_functions() {
    FunctionSymbol* cur = func_head;
    while (cur) {
        FunctionSymbol* tmp = cur;
        cur = cur->next;
        free(tmp->name);
        free(tmp);
    }
    func_head = NULL;
}
