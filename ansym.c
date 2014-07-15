/*
    ansym: ancc symbol table
    Aean, 2014.7.15
*/

#include "ancc.h"

symbol_scope_t* symtbl = 0;
symbol_scope_t* current_scope = 0;

void init_symbol_table(){
    symtbl = calloc(sizeof(symbol_scope_t), 1);
    if (!symtbl) badalloc();
    current_scope = symtbl;
}

symbol_t* check_symbol_conflict(const char* name, int attribute){
    if (!symtbl) init_symbol_table();
    symbol_list_t* j = current_scope->symbol_list;
    while(j){
        if (j->symbol->name == name && j->symbol->attribute == attribute) return j->symbol;
        j = j->next;
    }
    return NULL;
}

void add_symbol(symbol_t* newsymbol){
    symbol_list_t* newnode;
    symbol_t* conflict;
    if (!symtbl) init_symbol_table();
    conflict = check_symbol_conflict(newsymbol->name, newsymbol->attribute);
    if (conflict){
        switch(newsymbol->attribute){
        case SYMBOL_TYPEDEF:
            if (!type_equal(conflict->u.as_typedef.type, newsymbol->u.as_typedef.type)){
                eprintf( token(0, newsymbol->pos, newsymbol->cur, 0), EPRINTF_ERROR,
                        "conflicting type defined as '%s'", newsymbol->name);
                error_occured++;
                eprintf( token(0, conflict->pos, conflict->cur, 0), EPRINTF_NOTE,
                        "previous typedef of '%s' was here", conflict->name);
            }
            return;
        case SYMBOL_AGGREGATE:
            if (strlen(newsymbol->name)){
                if (!type_equal(conflict->u.as_aggregate.type, newsymbol->u.as_aggregate.type)){
                    eprintf( token(0, newsymbol->pos, newsymbol->cur, 0), EPRINTF_ERROR,
                            "ambiguous aggregator '%s'", newsymbol->name);
                    error_occured++;
                    eprintf( token(0, conflict->pos, conflict->cur, 0), EPRINTF_NOTE,
                            "previous declaration of '%s' was here", conflict->name);
                }
                return;
            }else break;
        case SYMBOL_VARIABLE:
            if (!type_equal(conflict->u.as_variable.type, newsymbol->u.as_variable.type)){
                eprintf( token(0, newsymbol->pos, newsymbol->cur, 0), EPRINTF_ERROR,
                        "conflicting type for '%s'", newsymbol->name);
                error_occured++;
                eprintf( token(0, conflict->pos, conflict->cur, 0), EPRINTF_NOTE,
                        "previous declaration of '%s' was here", conflict->name);
            }
            return;
        case SYMBOL_LABEL:
            eprintf( token(0, newsymbol->pos, newsymbol->cur, 0), EPRINTF_ERROR,
                    "ambiguous declaration for label '%s'", newsymbol->name);
            error_occured++;
            eprintf( token(0, conflict->pos, conflict->cur, 0), EPRINTF_NOTE,
                    "previous label '%s' was here", conflict->name);
            return;
        }
    }
    newnode = calloc(sizeof(symbol_list_t), 1);
    if (!newnode) badalloc();
    newnode->symbol = newsymbol;
    newnode->next = current_scope->symbol_list;
    current_scope->symbol_list = newnode;
}

symbol_t* lookup_symbol(const char* name, int attribute){
    if (!symtbl) init_symbol_table();
    symbol_scope_t* i = current_scope;
    while(i){
        symbol_list_t* j = i->symbol_list;
        while(j){
            if (j->symbol->name == name && j->symbol->attribute == attribute) return j->symbol;
            j = j->next;
        }
        i = i->prev;
    }
    return NULL;
}

void push_scope(){
    symbol_scope_t* newscope;
    if (!symtbl) init_symbol_table();
    newscope = calloc(sizeof(symbol_scope_t), 1);
    if (!newscope) badalloc();
    newscope->prev = current_scope;
    current_scope->next = newscope;
    current_scope = newscope;
}

void pop_scope(){
    symbol_list_t *p, *q;
    if (!current_scope) return;
    p = current_scope->symbol_list;
    while(p){
        q = p;
        p = p->next;
        free(q);
    }
    current_scope = current_scope->prev;
    free(current_scope->next);
}

symbol_t* create_typedef_symbol(type_t* type, token_t tok){
    symbol_t* newsymbol = calloc(sizeof(symbol_t), 1);
    if (!newsymbol) badalloc();
    newsymbol->attribute = SYMBOL_TYPEDEF;
    newsymbol->name = tok.val;
    newsymbol->pos = tok.pos;
    newsymbol->cur = tok.cur;
    newsymbol->u.as_typedef.type = type;
    return newsymbol;
}
