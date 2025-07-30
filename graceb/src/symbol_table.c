#include <stdlib.h>
#include <string.h>
#include "../include/symbol_table.h"

static Function* f_head = NULL;

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

void add_function(const char* name, ASTNode* body) {
    Function* f = malloc(sizeof(Function));
    if (!f) return;
    f->name = strdup(name);
    f->body = body;
    f->next = f_head;
    f_head = f;
}

ASTNode* lookup_function(const char* name) {
    Function* cur = f_head;
    while (cur) {
        if (strcmp(cur->name, name) == 0) {
            return cur->body;
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

    Function* fcur = f_head;
    while (fcur) {
        Function* tmp = fcur;
        fcur = fcur->next;
        free(tmp->name);
        /* body not managed for now */
        free(tmp);
    }
    f_head = NULL;
}
