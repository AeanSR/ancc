/*
    anast: ancc abstract syntax tree
    Aean, 2014.7.21
*/

#include "ancc.h"

typedef struct astnode_t{
    int spec;
    union {
        struct {

        } declarator_specifier;
    } u;
} astnode_t;
