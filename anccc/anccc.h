/*
    anccc: another noobish C compiler's compiler
    Aean, 2014.7.6

    anccc is part of ancc project.
    ancc project is licensed under the terms of the MIT License.
    Copyright (C) 2014 Aean
*/

#ifndef ANCCC_H_INCLUDED
#define ANCCC_H_INCLUDED

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct symbol_t{
    int no;
    char* name;
} symbol_t;
typedef struct rule_t{
    int left;
    int r[10];
} rule_t;
typedef struct item_t{
    int rule;
    int p;
    int la;
    struct item_t* next;
} item_t;
typedef struct itemset_t{
    item_t* item;
    unsigned long long zobrist;
    int zobred;
} itemset_t;

extern symbol_t* symbol_list[];
extern symbol_t terminal_list[];
extern symbol_t nonterminal_list[];
extern rule_t rule_list[];

#define IST(v) ((v)>=IDENT && (v)<=NAL)
#define ISNT(v) ((v)>=string_literal && (v)<EOL)

int* FIRST(int nonterm);
char* nameofsym(int sym);
void printitem(item_t* item);
void printset(itemset_t* set);
void closure(itemset_t* set);
int additem(itemset_t* set, item_t* newitem);
int setdup(itemset_t* s1, itemset_t* s2);
int itemeq(item_t* t1, item_t* t2);
itemset_t* conset(item_t* core);
item_t* conitem(int rule, int p, int la);
void delset(itemset_t* set);

void derive();
int migrate(int set, int x);
extern itemset_t** setfamily;
extern size_t familycount;
char* spaces(size_t num);
void writetbl();

enum{
    IDENT,
    C_CHAR,     C_STRING,
    C_NUM,
    K_AUTO,     K_BREAK,    K_CASE,     K_CHAR,
    K_CONST,    K_CONTINUE, K_DEFAULT,  K_DO,
    K_DOUBLE,   K_ELSE,     K_ENUM,     K_EXTERN,
    K_FLOAT,    K_FOR,      K_GOTO,     K_IF,
    K_INLINE,   K_INT,      K_LONG,     K_REGISTER,
    K_RESTRICT, K_RETURN,   K_SHORT,    K_SIGNED,
    K_SIZEOF,   K_STATIC,   K_STRUCT,   K_SWITCH,
    K_TYPEDEF,  K_UNION,    K_UNSIGNED, K_VOID,
    K_VOLATILE, K_WHILE,    K_BOOL,     K_COMPLEX,
    K_IMAGINARY,
    PUNC1,  PUNC2,  PUNC3,  PUNC4,  PUNC5,  PUNC6,  PUNC7,  PUNC8,  PUNC9,  PUNC10,
    PUNC11, PUNC12, PUNC13, PUNC14, PUNC15, PUNC16, PUNC17, PUNC18, PUNC19, PUNC20,
    PUNC21, PUNC22, PUNC23, PUNC24, PUNC25, PUNC26, PUNC27, PUNC28, PUNC29, PUNC30,
    PUNC31, PUNC32, PUNC33, PUNC34, PUNC35, PUNC36, PUNC37, PUNC38, PUNC39, PUNC40,
    PUNC41, PUNC42, PUNC43, PUNC44, PUNC45, PUNC46, PUNC47, PUNC48,
    typedef_name,
    NAL,
    string_literal,
    constant,
    primary_expression,
    postfix_expression,
    argument_expression_list,
    unary_expression,
    unary_operator,
    cast_expression,
    multiplicative_expression,
    additive_expression,
    shift_expression,
    relational_expression,
    equality_expression,
    AND_expression,
    exclusive_OR_expression,
    inclusive_OR_expression,
    logical_AND_expression,
    logical_OR_expression,
    conditional_expression,
    assignment_expression,
    assignment_operator,
    expression,
    constant_expression,
    declaration,
    declaration_specifiers,
    init_declarator_list,
    init_declarator,
    storage_class_specifier,
    type_specifier,
    struct_or_union_specifier,
    struct_or_union,
    struct_declaration_list,
    struct_declaration,
    specifier_qualifier_list,
    struct_declarator_list,
    struct_declarator,
    enum_specifier,
    enumerator_list,
    enumerator,
    type_qualifier,
    function_specifier,
    declarator,
    direct_declarator,
    pointer,
    type_qualifier_list,
    parameter_type_list,
    parameter_list,
    parameter_declaration,
    identifier_list,
    type_name,
    abstract_declarator,
    direct_abstract_declarator,
    direct_abstract_declarator_final,
    initializer,
    initializer_list,
    designation,
    designator_list,
    designator,
    statement,
    labeled_statement,
    compound_statement,
    block_item_list,
    block_item,
    expression_statement,
    selection_statement,
    iteration_statement,
    jump_statement,
    translation_unit,
    external_declaration,
    function_definition,
    declaration_list,
    whole_file,
    EOL,
};


#endif // ANCCC_H_INCLUDED
