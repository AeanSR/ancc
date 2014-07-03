/*
    io.c
    Aean, 2014.7.3
    This file is part of anlibc, which itself is part of ancc project.
    ancc project is licensed under the terms of the MIT License.
    Copyright (C) 2014 Aean & HUST
*/

#include "anlibc.h"

FILE* __anlib_getstream(unsigned no){
    return GetStdHandle(no);
}

int fputc(int c, FILE *stream){
    if( WriteFile(stream, &c, 1, 0, 0) )
        return c;
    else
        return -1;
}
int fputs(const char * __ANCC_RESTRICT s, FILE * __ANCC_RESTRICT stream){
    while(*s)
        fputc(*s++, stream);
    return 1;
}
int puts(const char * s){
    while(*s)
        fputc(*s++, stdout);
    return fputc('\n', stdout);
}
size_t fwrite(const void * __ANCC_RESTRICT ptr, size_t size, size_t nmemb, FILE * __ANCC_RESTRICT stream){
    size *= nmemb;
    nmemb = 0;
    WriteFile(stream, ptr, size, &nmemb, 0);
    return nmemb;
}

char d2c(int ch, int lcase){
    if (ch >= 0 && ch <= 9)
        return ch + '0';
    if (ch >= 10 && ch <= 16)
        return ch + lcase ? 'a' : 'A';
    return '@';
}

#define __PRINTD_BODY(pargsign, pargradix, pargcase)    if(1){ char* head = s; char buf[128]; \
    int i = 0; \
    if (pargsign && ch < 0){ \
        *s++ = '-'; \
        ch = -ch; \
    }else if (ch == 0){ \
        *s++ = '0'; \
    } \
    while(ch){ \
        buf[i++] = d2c(ch % pargradix, pargcase); \
        ch /= pargradix ; \
    } \
    while(i-- > 0){ \
        *s++ = buf[i]; \
    } \
    *s = 0; return (s-head);}else

int _printd(char* s, long int ch){
    __PRINTD_BODY(1, 10, 1);
}
//int _printld(char* s, long long int ch){
//    __PRINTD_BODY(1, 10, 1);
//}
int _printu(char* s, unsigned long ch){
    __PRINTD_BODY(0, 10, 1);
}
//int _printlu(char* s, unsigned long long ch){
//    __PRINTD_BODY(0, 10, 1);
//}


#define P(pargn, pargtype, printfunc) if(1){char* p = buf;\
                format+=pargn; \
                printfunc(buf, va_arg(arg, pargtype)); \
                while(*p && s < tail) *s++ = *p++; \
                state = 0;}else
#define PD(pargn, pargtype) P(pargn, pargtype, _printd)
#define PLD(pargn, pargtype) //P(pargn, pargtype, _printld)
#define PU(pargn, pargtype) P(pargn, pargtype, _printu)
#define PLU(pargn, pargtype) //P(pargn, pargtype, _printlu)

int vsnprintf(char * __ANCC_RESTRICT s, size_t n, const char * __ANCC_RESTRICT format, va_list arg){
    int state = 0;
    char* head = s;
    char* tail = s + n;
    char buf[128] = {0};
    size_t len = 0;

    while(*format && s < tail){
        switch(state){
        case 0:
            if (*format=='%'){
                format++;
                state = 1;
            }else{
                *s++ = *format++;
            }
            break;
        case 1:
            if (*format=='%'){
                *s++ = *format++;
                state = 0;
            }else if(*format=='h' && format[1]=='h' && (format[2]=='d' || format[2]=='i')){
                PD(3, signed char);
            }else if(*format=='h' && (format[1]=='d' || format[1]=='i')){
                PD(2, signed short);
            }else if((format[0]=='d' || format[0]=='i')){
                PD(1, signed int);
            }else if(*format=='l' && (format[1]=='d' || format[1]=='i')){
                PD(2, signed long);
            }else if(*format=='l' && format[1]=='l' && (format[2]=='d' || format[2]=='i')){
                PLD(3, signed long long);
            }else if (*format=='h' && format[1]=='h' && format[2]=='u'){
                PU(3, unsigned char);
            }else if(*format=='h' && format[1]=='u'){
                PU(2, unsigned short);
            }else if(*format=='u'){
                PU(1, unsigned int);
            }else if(*format=='l' && format[1]=='u'){
                PU(2, unsigned long);
            }else if(*format=='l' && format[1]=='l' && format[2]=='u'){
                PLU(3, unsigned long long);
            }else{
                format++;
                state = 0;
            }
            break;
        default: break;
        }
    }
    *s = 0;
    return (s - head);
}


