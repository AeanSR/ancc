/*
    ancc: another noobish C compiler
    Aean, 2014.6.27
        Website: http://aean.net/  Mail: v@aean.net

    ancc project is licensed under the terms of the MIT License.
    Copyright (C) 2014 Aean
*/

#ifndef __ANCC_HEADER_INCLUDED_
#define __ANCC_HEADER_INCLUDED_
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

/*
    An argument followed with __ANCC_BY_VAL means the pointer passed into will be duplicated.
    If the pointer is allocated on heap, caller take the respons to free that pointer.
    If the pointer is volatile or constant, callee take the respons to preserve a copy of data.
*/
#define __ANCC_BY_VAL
#define __ANCC_SIZE_LIMIT(bytes)

typedef struct{
    char* name;
    int   no;
} keyword_t;
typedef struct pool_t{
    struct pool_t* next;
    char* val;
} pool_t;
typedef struct sourceline_t{
    char* source;
    int lno;
    char* fname;
    struct sourceline_t* next;
} sourceline_t;
typedef struct sourcefile_t{
    char* fname;
    FILE* fp;
    struct sourcefile_t* next;
    struct sourcefile_t* prev;
} sourcefile_t;
typedef struct{
    int no;
    char* pos;
    sourceline_t* cur;
    char* val;
} token_t;
token_t token(int no, char* pos, sourceline_t* cur, char* val);
typedef struct rule_t{
    int left;
    int r[10];
} rule_t;

/*
typedef struct _variadic_arg_1_t{
    int d;
}_variadic_arg_1_t;
*/
extern char* memory;
extern keyword_t keylist[];
extern char* source;
extern sourceline_t* cur;
extern int error_occured;
extern int warning_occured;

int is_digit(char c);
int is_octaldigit(char c);
int is_hexaldigit(char c);
int is_simple_escape(char c);
int is_uppercase(char c);
int is_lowercase(char c);
int is_alphabet(char c);

void badalloc();
void read_source();
char _gc(int fwd);
char _lagc(size_t k);
char _gf(size_t k);
//#define CLA(...) _lagc((_variadic_arg_1_t){__VA_ARGS__}.d)
//#define CGF(...) _gf((_variadic_arg_1_t){__VA_ARGS__}.d==0?1:(_variadic_arg_1_t){__VA_ARGS__}.d)
#define CLA(...) _lagc((int)(__VA_ARGS__##.1))
#define CGF(...) _gf((int)(__VA_ARGS__##.1)?(int)(__VA_ARGS__##.1):1)
void reset_char_stream();
FILE* file_pointer();
char* file_name();
void push_file(const char* filename __ANCC_BY_VAL);
void pop_file();
void mwrite(char s);
void mclear();
char* strpool(const char* str __ANCC_BY_VAL);

void err( const char* message __ANCC_BY_VAL __ANCC_SIZE_LIMIT(256), ... );
void warn( const char* message __ANCC_BY_VAL __ANCC_SIZE_LIMIT(256), ... );
void eprintf( int type, const char* message __ANCC_BY_VAL __ANCC_SIZE_LIMIT(256), va_list vl );

char* vc_dir_path();

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
    typedef_name, /** typedef_name is distinguished by lexer */
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

void preprocess();
token_t lexparse();

#endif /* Guard word. */
