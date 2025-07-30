#include <stdlib.h>
#include <string.h>
#include "../include/symbol_table.h"

static Symbol* head = NULL;

void add_symbol(const char* name, const char* value) {
    Symbol* s = malloc(sizeof(Symbol));
    if (!s) return;
    s->name = strdup(name);
    s->value = strdup(value);
    s->next = head;
    head = s;
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
