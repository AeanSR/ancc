/*
    anlex: ancc lexer
    Aean, 2014.6.27 @HUST
*/

#include "ancc.h"

/* == Lexical parser. ============================= */
token_t lexparse(){
    enum{
        S_BREAK,
        S_IDENTIFY,
        S_CONSTNUM,
        S_CONSTNUM_Z,
        S_CONSTFLOAT,
        S_CONSTHEX,
        S_CONSTOCTAT,
        S_STRING,
        S_STRING_ESCAPE,
        S_CHAR,
        S_CHAR_ESCAPE,
        S_COMMENT_STAR,
        S_COMMENT_SLASH,
    };
    int state = S_BREAK;
    int i = 0;
    int lmidx;
    size_t lmlen, len;
    char* head;
    static const int inttype[3][2] = {
        { C_SIGNED, C_UNSIGNED },
        { C_SIGNEDLONG, C_UNSIGNEDLONG },
        { C_SIGNEDLONGLONG, C_UNSIGNEDLONGLONG },
    };
    static const int floattype[] = {C_SINGLE, C_DOUBLE};

    if(!source) read_source();
    mclear();
    head = source;
    while(*source){
        if(*source=='\\' && (*(source+1)==0x0d || *(source+1)==0x0a)){
            if (*(source+1)==0x0d && *(source+2)==0x0a)
                source+=3;
            else
                source+=2;
            lineno++;
            head = source;
        }else switch(state){
        case S_BREAK:
            mclear();
            lmidx = -1;
            lmlen = 0;
            i = 0;
            while(keylist[i].name){
                len = strlen(keylist[i].name);
                if(len > lmlen)
                    if(0==strncmp(keylist[i].name, source, len)){
                        lmlen = len;
                        lmidx = i;
                    }
                i++;
            }
            if(lmidx!=-1){
                source += strlen(keylist[lmidx].name);
                return (token_t){.no=keylist[lmidx].no, .pos=head, .val=strpool(keylist[lmidx].name)};
            }else if(*source==' ' || *source=='\t' || *source=='\r'){
                source++;
                head=source;
            }else if(*source=='\n'){
                source++;
                lineno++;
                head=source;
            }else if(*source=='/' && *(source+1)=='/'){
                state = S_COMMENT_SLASH;
                source+=2;
            }else if(*source=='/' && *(source+1)=='*'){
                state = S_COMMENT_STAR;
                source+=2;
            }else if(*source=='0'){
                state = S_CONSTNUM_Z;
                mwrite('0');
                source++;
            }else if(is_digit(*source)){
                state = S_CONSTNUM;
                mwrite(*source++);
            }else if(is_alphabet(*source) || *source=='_'){
                state = S_IDENTIFY;
                mwrite(*source++);
            }else if(*source=='"'){
                state = S_STRING;
                source++;
                head=source;
            }else if(*source=='\''){
                state = S_CHAR;
                source++;
                head=source;
            }else{
                eprintf("error: unexpected token %c(0x%02X)", *source, (unsigned)*source);
                source++;
            }
            break;
        case S_IDENTIFY:
            if(is_alphabet(*source) || is_digit(*source) || *source=='_'){
                mwrite(*source++);
            }else{
                state = S_BREAK;
                return (token_t){.no=IDENT, .pos=head, .val=strpool(memory)};
            }
            break;
        case S_CONSTNUM:
            if(is_digit(*source)){
                mwrite(*source++);
            }else if(*source=='.'){
                mwrite('.');
                source++;
                state = S_CONSTFLOAT;
            }else{
                int sign = 0, word = 0;
                if(*source=='U'||*source=='u')
                    sign++, mwrite(*source++);
                if(*source=='L'||*source=='l')
                    word++, mwrite(*source++);
                if(*source=='L'||*source=='l')
                    word++, mwrite(*source++);
                state = S_BREAK;
                return (token_t){.no=inttype[word][sign], .pos=head, .val=strpool(memory)};
            }
            break;
        case S_CONSTNUM_Z:
            if(is_octaldigit(*source)){
                mwrite(*source++);
                state = S_CONSTOCTAT;
            }else if(*source=='.'){
                mwrite('.');
                source++;
                state = S_CONSTFLOAT;
            }else if(*source=='x' || *source=='X'){
                mwrite('x');
                source++;
                state = S_CONSTHEX;
            }else{
                int sign = 0, word = 0;
                if(*source=='U'||*source=='u')
                    sign++, mwrite(*source++);
                if(*source=='L'||*source=='l')
                    word++, mwrite(*source++);
                if(*source=='L'||*source=='l')
                    word++, mwrite(*source++);
                state = S_BREAK;
                return (token_t){.no=inttype[word][sign], .pos=head, .val=strpool(memory)};
            }
            break;
        case S_CONSTOCTAT:
            if(is_octaldigit(*source)){
                mwrite(*source++);
            }else{
                int sign = 0, word = 0;
                if(*source=='U'||*source=='u')
                    sign++, mwrite(*source++);
                if(*source=='L'||*source=='l')
                    word++, mwrite(*source++);
                if(*source=='L'||*source=='l')
                    word++, mwrite(*source++);
                state = S_BREAK;
                return (token_t){.no=inttype[word][sign], .pos=head, .val=strpool(memory)};
            }
            break;
        case S_CONSTFLOAT:
            if(is_digit(*source)){
                mwrite(*source++);
            }else{
                int word = 0;
                if(*source=='F'||*source=='f')
                    mwrite(*source++);
                else if(*source=='L'||*source=='l')
                    word++, mwrite(*source++);
                state = S_BREAK;
                return (token_t){.no=floattype[word], .pos=head, .val=strpool(memory)};
            }
            break;
        case S_CONSTHEX:
            if(is_hexaldigit(*source)){
                mwrite(*source++);
            }else{
                int sign = 0, word = 0;
                if(*source=='U'||*source=='u')
                    sign++, mwrite(*source++);
                if(*source=='L'||*source=='l')
                    word++, mwrite(*source++);
                if(*source=='L'||*source=='l')
                    word++, mwrite(*source++);
                state = S_BREAK;
                return (token_t){.no=inttype[word][sign], .pos=head, .val=strpool(memory)};
            }
            break;
        case S_STRING:
            if(*source=='"'){
                source++;
                state = S_BREAK;
                return (token_t){.no=C_STRING, .pos=head, .val=strpool(memory)};
            }else if(*source=='\\'){
                state = S_STRING_ESCAPE;
                source++;
                mwrite('\\');
            }else if(*source=='\n'){
                eprintf("error: missing terminating \" character");
                lineno++;
                source++;
                state = S_BREAK;
                return (token_t){.no=C_STRING, .pos=head, .val=strpool(memory)};
            }else{
                mwrite(*source++);
            }
            break;
        case S_STRING_ESCAPE:
            if(is_simple_escape(*source)){
                mwrite(*source++);
                state = S_STRING;
            }else if(is_octaldigit(*source)){
                mwrite(*source++);
                if(is_octaldigit(*source)){
                    mwrite(*source++);
                    if(is_octaldigit(*source))
                        mwrite(*source++);
                }
                state = S_STRING;
            }else if(*source=='x' && is_hexaldigit(*(source+1))){
                mwrite('x');
                source++;
                while(is_hexaldigit(*source))
                    mwrite(*source++);
                state = S_STRING;
            }else{
                eprintf("warning: unknown escape sequence: \'\\%c\'", *source);
                mwrite(*source++);
                state = S_STRING;
            }
            break;
        case S_CHAR:
            if(*source=='\''){
                if (strlen(memory)>2 || (strlen(memory)>1 && *memory!='\\'))
                    eprintf("warning: multi-character character constant \"%s\"", memory);
                else if(*memory == 0)
                    eprintf("error: empty character constant");
                state = S_BREAK;
                source++;
                return (token_t){.no=C_CHAR, .pos=head, .val=strpool(memory)};
            }else if(*source=='\\'){
                state = S_CHAR_ESCAPE;
                source++;
                mwrite('\\');
            }else if(*source=='\n'){
                eprintf("error: missing terminating \' character");
                lineno++;
                source++;
                state = S_BREAK;
                return (token_t){.no=C_CHAR, .pos=head, .val=strpool(memory)};
            }else{
                mwrite(*source++);
            }
            break;
        case S_CHAR_ESCAPE:
            if(is_simple_escape(*source)){
                mwrite(*source++);
                state = S_CHAR;
            }else if(is_octaldigit(*source)){
                mwrite(*source++);
                if(is_octaldigit(*source)){
                    mwrite(*source++);
                    if(is_octaldigit(*source))
                        mwrite(*source++);
                }
                state = S_CHAR;
            }else if(*source=='x' && is_hexaldigit(*(source+1))){
                mwrite('x');
                source++;
                while(is_hexaldigit(*source))
                    mwrite(*source++);
                state = S_CHAR;
            }else{
                eprintf("warning: unknown escape sequence: \'\\%c\'", *source);
                mwrite(*source++);
                state = S_CHAR;
            }
            break;
        case S_COMMENT_SLASH:
            if (*source=='\n'){
                state = S_BREAK;
                source++;
            }else{
                source++;
            }
            break;
        case S_COMMENT_STAR:
            if (*source=='*' && *source=='/'){
                state = S_BREAK;
                source+=2;
            }else{
                source++;
            }
        default:
            printf("internal error: unexpected state %d of ancc lexer\n", state);
            exit(-1);
            break;
        }
    }
    return (token_t){.no=NAL};
}
