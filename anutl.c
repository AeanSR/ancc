/*
    anutl: ancc ultilities
    Aean, 2014.6.27 @HUST
*/

#include "ancc.h"

/* == Bad alloc. ================================== */
void badalloc(){
    printf("internal error: bad alloc\n");
    exit(-1);
}

/* == Source file stream. ========================= */
char* source = 0;
int lineno = 1;
sourceline_t* cur = 0;
sourceline_t* intl_s = 0;
FILE* pfin = 0;
void read_source(){
    int l = 1;
    sourceline_t* p = 0;
    while(!feof(pfin)){
        cur = calloc(1, sizeof(sourceline_t));
        if (!cur) badalloc();
        if (!intl_s) intl_s = cur;
        cur->next = 0;
        cur->lno = l++;
        cur->source = read_line(pfin);
        if (p) p->next = cur;
        p = cur;
    }
    cur = intl_s;
    source = cur->source;
}

typedef struct linkstring_t{
    char d;
    struct linkstring_t* next;
}linkstring_t;

char* read_line(FILE* f){
    linkstring_t first;
    linkstring_t *head = &first, *p, *q;
    char c;
    char *out, *outh;
    size_t len = 0;
    if (!(len+=fread(&c, 1, 1, f))) return strpool("");
    if (c=='\r')
        if (!(len+=fread(&c, 1, 1, f))) return strpool("");
    if (c=='\n') return strpool("");
    first.d = c;
    first.next = 0;
    q = head;
    while((len+=fread(&c, 1, 1, f))){
        if (c=='\n') break;
        p = malloc(sizeof(linkstring_t));
        if (!p) badalloc();
        p->d = c;
        p->next = 0;
        q->next = p;
        q = p;
    }
    outh = out = calloc(len + 32,1);
    if (!out) badalloc();
    p = head;
    while(p){
        *out++ = p->d;
        q = p;
        p = p->next;
        if (q!=&first)
            free(q);
    }
    return outh;
}
char _gc(int fwd){
    char ch = 0;
    if (!intl_s) read_source();
    ch = *source;
    if (ch){
        source+=!!fwd;
        return ch;
    }
    if (!cur->next) return 0;
    if (!fwd) return '\n';
    cur = cur->next;
    source = cur->source;
    lineno = cur->lno;
    return '\n';
}
char _lagc(size_t k){
    sourceline_t* bkcur = cur;
    char* bksource = source;
    int bklno = lineno;
    char ch;
    while(k-->0) _gc(1);
    ch = _gc(0);
    lineno = bklno;
    source = bksource;
    cur = bkcur;
    return ch;
}
char _gf(size_t k){
    char ch = 0;
    if (!k) return _lagc(0);
    while(k-->0) ch=_gc(1);
    return ch;
}

/* == Char to String aggregator. ================== */
char* memory = 0;
char* memory_p = 0;
size_t memory_size = 4096;

void mwrite(char s){
    if (!memory)
        memory_p = memory = calloc(memory_size + 5, 1);
    if (!memory)
        badalloc();
    if (memory_p - memory >= memory_size){
        char* newbase = calloc(memory_size + memory_size + 5, 1);
        if (!newbase) badalloc();
        memory_size += memory_size;
        strcpy(newbase, memory);
        memory_p = newbase + (intptr_t)(memory_p - memory);
        free(memory);
        memory = newbase;
    }
    *memory_p++ = s;
}

void mclear(){
    if (memory)
        memset(memory, 0, memory_size);
    memory_p = memory;
}

/* == String Pool ================================= */
char* strpool(const char* str){
    static pool_t* pool = 0;
    pool_t* p = pool;
    if (p){
        if (0 == strcmp(str, p->val))
            return p->val;
        while(p->next){
            p = p->next;
            if (0 == strcmp(str, p->val))
                return p->val;
        }
        p->next = malloc(sizeof(pool_t));
        if (!p->next) badalloc();
        p->next->next = 0;
        return (p->next->val = _strdup(str));
    }else{
        pool = malloc(sizeof(pool_t));
        if (!pool) badalloc();
        pool->next = 0;
        return (pool->val = _strdup(str));
    }
}

/* == Error report. =============================== */
void eprintf(const char* message, ...){
    char* s = source;
    char* p;
    char buffer[256] = {0};
    va_list vl;

    /*
        Use the same error output format from GCC:
            :<line>:<offset>: <message>
            < a single line of source code >
                             ^   <-- a mark pointing to where error occurred.
    */

    /* First, roll back to the last '\n'. */
    /*while(s > intl_s){
        if (*(s-1) == '\n'){
            break;
        }
        s--;
    }*/
    CLA();
    s = cur->source;

    /* Then, print out error message. */
    va_start(vl, message);
    vsnprintf(buffer, 256, message, vl);
    va_end(vl);
    printf("\n:%d:%u: %s\n", lineno, 1U + (unsigned)((uintptr_t)source - (uintptr_t)s), buffer);

    /* Print a single line of source code. */
    /*p = s;
    while(*p)
        putchar(*p++);
    putchar('\n');*/
    printf("%s\n", cur->source);

    /* Print a ^ mark. */
    p = s;
    while(p++<source)
        putchar(' ');
    putchar('^');
    putchar('\n');
}

/* == token_t constructor. ======================== */
token_t token(int no, char* pos, sourceline_t* cur, char* val){
	token_t t;
	t.no = no;
	t.pos = pos;
	t.cur = cur;
	t.val = val;
	return t;
}

/* == Character recognizer. ======================= */
int is_digit(char c){
    return c <= '9' && c >= '0';
}
int is_octaldigit(char c){
    return c <= '7' && c >= '0';
}
int is_hexaldigit(char c){
    return is_digit(c) || (c <= 'F' && c >= 'A') || (c <= 'f' && c >= 'a');
}
int is_simple_escape(char c){
    return c == '\'' || c == '"' || c == '?' || c == '\\' ||
    c == 'a' || c == 'b'  || c == 'f' || c == 'n' || c == 'r' ||
    c == 't' || c == 'v';
}
int is_uppercase(char c){
    return c <= 'Z' && c >= 'A';
}
int is_lowercase(char c){
    return c <= 'z' && c >= 'a';
}
int is_alphabet(char c){
    return is_uppercase(c) || is_lowercase(c);
}

/* == Keyword list. =============================== */
keyword_t keylist[] = {
    {"auto"         ,  K_AUTO},
    {"break"        ,  K_BREAK},
    {"case"         ,  K_CASE},
    {"char"         ,  K_CHAR},
    {"const"        ,  K_CONST},
    {"continue"     ,  K_CONTINUE},
    {"default"      ,  K_DEFAULT},
    {"do"           ,  K_DO},
    {"double"       ,  K_DOUBLE},
    {"else"         ,  K_ELSE},
    {"enum"         ,  K_ENUM},
    {"extern"       ,  K_EXTERN},
    {"float"        ,  K_FLOAT},
    {"for"          ,  K_FOR},
    {"goto"         ,  K_GOTO},
    {"if"           ,  K_IF},
    {"inline"       ,  K_INLINE},
    {"int"          ,  K_INT},
    {"long"         ,  K_LONG},
    {"register"     ,  K_REGISTER},
    {"restrict"     ,  K_RESTRICT},
    {"return"       ,  K_RETURN},
    {"short"        ,  K_SHORT},
    {"signed"       ,  K_SIGNED},
    {"sizeof"       ,  K_SIZEOF},
    {"static"       ,  K_STATIC},
    {"struct"       ,  K_STRUCT},
    {"switch"       ,  K_SWITCH},
    {"typedef"      ,  K_TYPEDEF},
    {"union"        ,  K_UNION},
    {"unsigned"     ,  K_UNSIGNED},
    {"void"         ,  K_VOID},
    {"volatile"     ,  K_VOLATILE},
    {"while"        ,  K_WHILE},
    {"_Bool"        ,  K_BOOL},
    {"_Complex"     ,  K_COMPLEX},
    {"_Imaginary"   ,  K_IMAGINARY},
    {"["            ,  PUNC1},
    {"]"            ,  PUNC2},
    {"("            ,  PUNC3},
    {")"            ,  PUNC4},
    {"{"            ,  PUNC5},
    {"}"            ,  PUNC6},
    {"."            ,  PUNC7},
    {"->"           ,  PUNC8},
    {"++"           ,  PUNC9},
    {"--"           ,  PUNC10},
    {"&"            ,  PUNC11},
    {"*"            ,  PUNC12},
    {"+"            ,  PUNC13},
    {"-"            ,  PUNC14},
    {"~"            ,  PUNC15},
    {"!"            ,  PUNC16},
    {"/"            ,  PUNC17},
    {"%"            ,  PUNC18},
    {"<<"           ,  PUNC19},
    {">>"           ,  PUNC20},
    {"<"            ,  PUNC21},
    {">"            ,  PUNC22},
    {"<="           ,  PUNC23},
    {">="           ,  PUNC24},
    {"=="           ,  PUNC25},
    {"!="           ,  PUNC26},
    {"^"            ,  PUNC27},
    {"|"            ,  PUNC28},
    {"&&"           ,  PUNC29},
    {"||"           ,  PUNC30},
    {"?"            ,  PUNC31},
    {":"            ,  PUNC32},
    {";"            ,  PUNC33},
    {"..."          ,  PUNC34},
    {"="            ,  PUNC35},
    {"*="           ,  PUNC36},
    {"/="           ,  PUNC37},
    {"%="           ,  PUNC38},
    {"+="           ,  PUNC39},
    {"-="           ,  PUNC40},
    {"<<="          ,  PUNC41},
    {">>="          ,  PUNC42},
    {"&="           ,  PUNC43},
    {"^="           ,  PUNC44},
    {"|="           ,  PUNC45},
    {","            ,  PUNC46},
    {"#"            ,  PUNC47},
    {"##"           ,  PUNC48},
    {"<:"           ,  PUNC1},
    {":>"           ,  PUNC2},
    {"<%"           ,  PUNC5},
    {"%>"           ,  PUNC6},
    {"%:"           ,  PUNC47},
    {"%:%:"         ,  PUNC48},
    {NULL           ,  NAL}
};
