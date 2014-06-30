/*
    anutl: ancc ultilities
    Aean, 2014.6.27
*/

#include "ancc.h"

/* == Bad alloc. ================================== */
void badalloc() {
    printf( "internal error: bad alloc\n" );
    exit( -1 );
}

/* == Source file stream. ========================= */
char* source = 0;
sourceline_t* cur = 0;
sourceline_t* intl_s = 0;
void read_source() {
    int l = 1;
    sourceline_t* p = 0;
    while( !feof( file_pointer() ) ) {
        cur = calloc( 1, sizeof( sourceline_t ) );
        if ( !cur ) badalloc();
        if ( !intl_s ) intl_s = cur;
        cur->next = 0;
        cur->lno = l++;
        cur->fname = file_name();
        cur->source = read_line( file_pointer() );
        if ( p ) p->next = cur;
        p = cur;
    }
    cur = intl_s;
    source = cur->source;
}

typedef struct linkstring_t {
    char d;
    struct linkstring_t* next;
} linkstring_t;

char* read_line( FILE* f ) {
    linkstring_t first;
    linkstring_t* head = &first, *p, *q;
    char c;
    char* out, *outh;
    size_t len = 0;
    if ( !( len += fread( &c, 1, 1, f ) ) )
        return strpool( "" );
    if ( c == '\r' )
        if ( !( len += fread( &c, 1, 1, f ) ) )
            return strpool( "" );
    if ( c == '\n' )
        return strpool( "" );

    first.d = c;
    first.next = 0;
    q = head;
    while( fread( &c, 1, 1, f ) ) {
        len++;
        if ( c == '\n' ) break;
        p = malloc( sizeof( linkstring_t ) );
        if ( !p ) badalloc();
        p->d = c;
        p->next = 0;
        q->next = p;
        q = p;
    }
    outh = out = calloc( len + 32, 1 );

    if ( !out ) badalloc();
    p = head;
    while( p ) {
        *out++ = p->d;
        q = p;
        p = p->next;
        if ( q != &first )
            free( q );
    }
    return outh;
}
char _gc( int fwd ) {
    char ch = 0;
    if ( !intl_s ) read_source();
    ch = *source;
    if ( ch ) {
        source += !!fwd;
        return ch;
    }
    if ( !cur->next ) return 0;
    if ( !fwd ) return '\n';
    cur = cur->next;
    source = cur->source;
    return '\n';
}
char _lagc( size_t k ) {
    sourceline_t* bkcur = cur;
    char* bksource = source;
    char ch;
    while( k-- > 0 ) _gc( 1 );
    ch = _gc( 0 );
    source = bksource;
    cur = bkcur;
    return ch;
}
char _gf( size_t k ) {
    char ch = 0;
    if ( !k ) return _lagc( 0 );
    while( k-- > 0 ) ch = _gc( 1 );
    return ch;
}
void reset_char_stream() {
    if ( !intl_s ) return;
    cur = intl_s;
    source = intl_s->source;
}

/* == File manager. =============================== */
sourcefile_t* intl_f = 0;
sourcefile_t* workingfile = 0;
FILE* file_pointer() {
    if( workingfile )
        if ( workingfile->fp )
            return workingfile->fp;
    return 0;
}
char* file_name() {
    if( workingfile )
        if ( workingfile->fname )
            return workingfile->fname;
    return 0;
}
void push_file( const char* filename __ANCC_BY_VAL ) {
    sourcefile_t* newfile = calloc( sizeof( sourcefile_t ), 1 );
    if ( !newfile ) badalloc();
    newfile->fname = _strdup( filename );
    newfile->fp = fopen( newfile->fname, "rb" );
    /** To do: exception for file open. */
    newfile->next = 0;
    newfile->prev = workingfile;
    if ( !intl_f ) intl_f = newfile;
    if ( workingfile ) workingfile->next = newfile;
    workingfile = newfile;
}
void pop_file() {
    sourcefile_t* p;
    if ( !workingfile ) return;
    if ( workingfile->fname ) free( workingfile->fname );
    if ( workingfile->fp ) fclose( workingfile->fp );
    workingfile = p = workingfile->prev;
    if ( intl_f == p ) intl_f = 0;
    if ( workingfile ) workingfile->next = 0;
    free( p );
}

/* == Char to String aggregator. ================== */
char* memory = 0;
char* memory_p = 0;
size_t memory_size = 4096;

void mwrite( char s ) {
    if ( !memory )
        memory_p = memory = calloc( memory_size + 5, 1 );
    if ( !memory )
        badalloc();
    if ( memory_p - memory >= memory_size ) {
        char* newbase = calloc( memory_size + memory_size + 5, 1 );
        if ( !newbase ) badalloc();
        memory_size += memory_size;
        strcpy( newbase, memory );
        memory_p = newbase + ( intptr_t )( memory_p - memory );
        free( memory );
        memory = newbase;
    }
    *memory_p++ = s;
}

void mclear() {
    if ( memory )
        memset( memory, 0, memory_size );
    memory_p = memory;
}

/* == String Pool ================================= */
char* strpool( const char* str __ANCC_BY_VAL ) {
    static pool_t* pool = 0;
    pool_t* p = pool;
    if ( p ) {
        if ( 0 == strcmp( str, p->val ) )
            return p->val;
        while( p->next ) {
            p = p->next;
            if ( 0 == strcmp( str, p->val ) )
                return p->val;
        }
        p->next = malloc( sizeof( pool_t ) );
        if ( !p->next ) badalloc();
        p->next->next = 0;
        return ( p->next->val = _strdup( str ) );
    } else {
        pool = malloc( sizeof( pool_t ) );
        if ( !pool ) badalloc();
        pool->next = 0;
        return ( pool->val = _strdup( str ) );
    }
}

/* == Error report. =============================== */
void eprintf( const char* message __ANCC_BY_VAL __ANCC_SIZE_LIMIT(256), ... ) {
    char* p;
    char buffer[256] = {0};
    va_list vl;

    /*
        Use the same error output format from GCC:
            :<line>:<offset>: <message>
            < a single line of source code >
                             ^   <-- a mark pointing to where error occurred.
    */

    va_start( vl, message );
    vsnprintf( buffer, 256, message, vl );
    va_end( vl );
    printf( "\n:%d:%u: %s\n", cur->lno, 1U + ( unsigned )( ( uintptr_t )source - ( uintptr_t )cur->source ), buffer );

    printf( "%s\n", cur->source );

    p = cur->source;
    while( p++ < source )
        putchar( ' ' );
    putchar( '^' );
    putchar( '\n' );
}

/* == token_t constructor. ======================== */
token_t token( int no, char* pos, sourceline_t* cur, char* val ) {
    token_t t;
    t.no = no;
    t.pos = pos;
    t.cur = cur;
    t.val = val;
    return t;
}

/* == Character recognizer. ======================= */
int is_digit( char c ) {
    return c <= '9' && c >= '0';
}
int is_octaldigit( char c ) {
    return c <= '7' && c >= '0';
}
int is_hexaldigit( char c ) {
    return is_digit( c ) || ( c <= 'F' && c >= 'A' ) || ( c <= 'f' && c >= 'a' );
}
int is_simple_escape( char c ) {
    return c == '\'' || c == '"' || c == '?' || c == '\\' ||
           c == 'a' || c == 'b'  || c == 'f' || c == 'n' || c == 'r' ||
           c == 't' || c == 'v';
}
int is_uppercase( char c ) {
    return c <= 'Z' && c >= 'A';
}
int is_lowercase( char c ) {
    return c <= 'z' && c >= 'a';
}
int is_alphabet( char c ) {
    return is_uppercase( c ) || is_lowercase( c );
}
