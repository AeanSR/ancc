/*
    ancc: another noobish C compiler
    Aean, 2014.6.27 @HUST
*/

#ifndef __ANCC_HEADER_INCLUDED_
#define __ANCC_HEADER_INCLUDED_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdarg.h>

typedef struct{
    char* name;
    int   no;
} keyword_t;
typedef struct pool_t{
    struct pool_t* next;
    char* val;
} pool_t;
typedef struct{
    int no;
    char* pos;
    char* val;
} token_t;
typedef struct ppsblock_t{
    char* source;
    int lno;
    struct ppsblock_t* next;
} sourceline_t;
typedef struct _variadic_arg_1_t{
    int d;
}_variadic_arg_1_t;

extern char* memory;
extern keyword_t keylist[];
extern char* source;
extern sourceline_t* cur;
extern FILE* pfin;
extern int lineno;

int is_digit(char c);
int is_octaldigit(char c);
int is_hexaldigit(char c);
int is_simple_escape(char c);
int is_uppercase(char c);
int is_lowercase(char c);
int is_alphabet(char c);

void badalloc();
void read_source();
char* read_line(FILE* f);
char _gc(int fwd);
char _lagc(size_t k);
char _gf(size_t k);
#define LA(...) _lagc((_variadic_arg_1_t){__VA_ARGS__}.d)
#define GF(...) _gf((_variadic_arg_1_t){__VA_ARGS__}.d==0?1:(_variadic_arg_1_t){__VA_ARGS__}.d)
void mwrite(char s);
void mclear();
char* strpool(const char* str);
void eprintf(const char* message, ...);

#define STR2(v) #v
#define STR(v) STR2(v)
#define P(...) strpool(STR(__VA_ARGS__))

enum{
    IDENT,
    C_CHAR,     C_STRING,
    C_SINGLE,   C_DOUBLE,
    C_SIGNED,   C_UNSIGNED,     C_SIGNEDLONG,   C_UNSIGNEDLONG,
    C_SIGNEDLONGLONG,           C_UNSIGNEDLONGLONG,
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
    NAL,
};

token_t lexparse();

#endif /* Guard word. */
