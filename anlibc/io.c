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
    unsigned nbyte = 0;
    if( WriteFile(stream, &c, 1, &nbyte, 0) )
        return nbyte == 1 ? c : -1;
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
        return ch - 10 + (lcase ? 'a' : 'A');
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
    while(i < minlen) \
        buf[i++] = leadzero ? '0' : ' '; \
    while(i-- > 0){ \
        *s++ = buf[i]; \
    } \
    *s = 0; return (s-head);}else

int _printd(char* s, long int ch, int minlen, int leadzero){
    __PRINTD_BODY(1, 10, 1);
}
int _printu(char* s, unsigned long ch, int minlen, int leadzero){
    __PRINTD_BODY(0, 10, 1);
}
int _printx(char* s, unsigned long ch, int minlen, int leadzero){
    __PRINTD_BODY(0, 16, 1);
}
int _printX(char* s, unsigned long ch, int minlen, int leadzero){
    __PRINTD_BODY(0, 16, 0);
}
int _printo(char* s, unsigned long ch, int minlen, int leadzero){
    __PRINTD_BODY(0, 8, 1);
}


#define P(pargn, pargtype, printfunc) if(1){char* p = buf;\
                format+=pargn; \
                printfunc(buf, va_arg(arg, pargtype), minlen, leadzero); \
                while(*p && count > 0){ if(select) *s++ = *p++; else fputc(*p++, f); count--; } \
                state = 0; leadzero = 0; minlen = 0;}else
#define PD(pargn, pargtype) P(pargn, pargtype, _printd)
#define PU(pargn, pargtype) P(pargn, pargtype, _printu)
#define Px(pargn, pargtype) P(pargn, pargtype, _printx)
#define PX(pargn, pargtype) P(pargn, pargtype, _printX)
#define PO(pargn, pargtype) P(pargn, pargtype, _printo)


int coreprintf(char * __ANCC_RESTRICT s, FILE* f, int select, size_t n, const char * __ANCC_RESTRICT format, va_list arg){
    int state = 0;
    int count = n;
    char buf[128] = {0};
    size_t len = 0;
    int minlen = 0;
    int leadzero = 0;

    while(*format && count){
        switch(state){
        case 0:
            if (*format=='%'){
                format++;
                state = 2;
            }else{
                if(select) *s++ = *format++; else fputc(*format++, f);
                count--;
            }
            break;
        case 1:
            if (*format=='%'){
                if(select) *s++ = *format++; else fputc(*format++, f);
                count--;
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
                PD(3, signed long long);
            }else if (*format=='h' && format[1]=='h' && format[2]=='u'){
                PU(3, unsigned char);
            }else if(*format=='h' && format[1]=='u'){
                PU(2, unsigned short);
            }else if(*format=='u'){
                PU(1, unsigned int);
            }else if(*format=='l' && format[1]=='u'){
                PU(2, unsigned long);
            }else if(*format=='l' && format[1]=='l' && format[2]=='u'){
                PU(3, unsigned long long);
            }else if (*format=='h' && format[1]=='h' && format[2]=='x'){
                Px(3, unsigned char);
            }else if(*format=='h' && format[1]=='x'){
                Px(2, unsigned short);
            }else if(*format=='x'){
                Px(1, unsigned int);
            }else if(*format=='l' && format[1]=='x'){
                Px(2, unsigned long);
            }else if(*format=='l' && format[1]=='l' && format[2]=='x'){
                Px(3, unsigned long long);
            }else if (*format=='h' && format[1]=='h' && format[2]=='X'){
                PX(3, unsigned char);
            }else if(*format=='h' && format[1]=='X'){
                PX(2, unsigned short);
            }else if(*format=='X'){
                PX(1, unsigned int);
            }else if(*format=='l' && format[1]=='X'){
                PX(2, unsigned long);
            }else if(*format=='l' && format[1]=='l' && format[2]=='X'){
                PX(3, unsigned long long);
            }else if (*format=='h' && format[1]=='h' && format[2]=='o'){
                PO(3, unsigned char);
            }else if(*format=='h' && format[1]=='o'){
                PO(2, unsigned short);
            }else if(*format=='o'){
                PO(1, unsigned int);
            }else if(*format=='l' && format[1]=='o'){
                PO(2, unsigned long);
            }else if(*format=='l' && format[1]=='l' && format[2]=='o'){
                PO(3, unsigned long long);
            }else if(*format=='s'){
                const char* str = va_arg(arg, const char*);
                format++;
                while(*str && count > 0){
                    if(select) *s++ = *str++; else fputc(*str++, f);
                    count--;
                }
                state = 0;
            }else if(*format=='c'){
                char ch = va_arg(arg, char);
                format++;
                if (count > 0){
                    if(select) *s++ = ch; else fputc(ch, f);
                    count--;
                }
                state = 0;
            }else{
                format++;
                state = 0;
            }
            break;
        case 2:
            if (*format=='0'){
                leadzero = 1;
                format++;
            }
            state = 3;
            break;
        case 3:
            if (*format >= '0' && *format <= '9'){
                minlen = minlen * 10 + (*format++ - '0');
            }else{
                state = 1;
            }
        default: break;
        }
    }
    if (select)
        *s = 0;
    return (n - count);
}

int vsnprintf(char * __ANCC_RESTRICT s, size_t n, const char * __ANCC_RESTRICT format, va_list arg){
    return coreprintf(s, 0, 1, n, format, arg);
}
int snprintf(char * __ANCC_RESTRICT s, size_t n, const char * __ANCC_RESTRICT format, ...){
    va_list arg;
    int ret;
    va_start(arg, format);
    ret = coreprintf(s, 0, 1, n, format, arg);
    va_end(arg);
    return ret;
}
int vsprintf(char * __ANCC_RESTRICT s, const char * __ANCC_RESTRICT format, va_list arg){
    return coreprintf(s, 0, 1, 0x7FFFFFFFUL, format, arg);
}
int sprintf(char * __ANCC_RESTRICT s, const char * __ANCC_RESTRICT format, ...){
    va_list arg;
    int ret;
    va_start(arg, format);
    ret = coreprintf(s, 0, 1, 0x7FFFFFFFUL, format, arg);
    va_end(arg);
    return ret;
}
int vfnprintf(FILE * __ANCC_RESTRICT f, size_t n, const char * __ANCC_RESTRICT format, va_list arg){
    return coreprintf(0, f, 0, n, format, arg);
}
int fnprintf(FILE * __ANCC_RESTRICT f, size_t n, const char * __ANCC_RESTRICT format, ...){
    va_list arg;
    int ret;
    va_start(arg, format);
    ret = coreprintf(0, f, 0, n, format, arg);
    va_end(arg);
    return ret;
}
int vfprintf(FILE * __ANCC_RESTRICT f, const char * __ANCC_RESTRICT format, va_list arg){
    return coreprintf(0, f, 0, 0x7FFFFFFFUL, format, arg);
}
int fprintf(FILE * __ANCC_RESTRICT f, const char * __ANCC_RESTRICT format, ...){
    va_list arg;
    int ret;
    va_start(arg, format);
    ret = coreprintf(0, f, 0, 0x7FFFFFFFUL, format, arg);
    va_end(arg);
    return ret;
}
int printf(const char * format, ...){
    va_list arg;
    int ret;
    va_start(arg, format);
    ret = coreprintf(0, stdout, 0, 0x7FFFFFFFUL, format, arg);
    va_end(arg);
    return ret;
}

int fclose(FILE *stream){
    return CloseHandle(stream);
}
int fflush(FILE *stream){
    return FlushFileBuffers(stream);
}
FILE *fopen(const char * __ANCC_RESTRICT filename, const char * __ANCC_RESTRICT mode){
    if (0==strcmp(mode, "r") || 0==strcmp(mode, "rb")){
        return CreateFileA(filename, 0x80000000L, 1, 0, 3, 0x80, 0);
    }else if(0==strcmp(mode, "w") || 0==strcmp(mode, "wb")){
        return CreateFileA(filename, 0x40000000L, 1, 0, 2, 0x80, 0);
    }else if(0==strcmp(mode, "a") || 0==strcmp(mode, "ab")){
        return CreateFileA(filename, 4, 1, 0, 4, 0x80, 0);
    }else if(0==strcmp(mode, "r+") || 0==strcmp(mode, "rb+") || 0==strcmp(mode, "r+b")){
        return CreateFileA(filename, 0xC0000000L, 1, 0, 3, 0x80, 0);
    }else if(0==strcmp(mode, "w+") || 0==strcmp(mode, "wb+") || 0==strcmp(mode, "w+b")){
        return CreateFileA(filename, 0xC0000000L, 1, 0, 2, 0x80, 0);
    }else if(0==strcmp(mode, "a+") || 0==strcmp(mode, "ab+") || 0==strcmp(mode, "a+b")){
        return CreateFileA(filename, 4, 1, 0, 4, 0x80, 0);
    }
    return 0;
}
