/*
    stdio.h
    Aean, 2014.7.3
    This file is part of anlibc, which itself is part of ancc project.
    ancc project is licensed under the terms of the MIT License.
    Copyright (C) 2014 Aean & HUST
*/

#ifndef __ANLIBC_STDIO_INCLUDED_
#define __ANLIBC_STDIO_INCLUDED_

#ifndef __ANLIBC_INTLBUILD__
#include <stdarg.h>
#include <stddef.h>
#endif

#ifndef __ANCC_RESTRICT
#ifdef __ANCC__
#define __ANCC_RESTRICT restrict
#else
#define __ANCC_RESTRICT __restrict
#endif
#endif

typedef unsigned FILE;
FILE* __anlibc_getstream(unsigned no);
#define stdin  __anlibc_getstream(-10)
#define stdout __anlibc_getstream(-11)
#define stderr __anlibc_getstream(-12)

#define EOF (-1)
#define SEEK_CUR (1)
#define SEEK_END (2)
#define SEEK_SET (0)

int fputc(int c, FILE *stream);
int fgetc(FILE *stream);
int fputs(const char * __ANCC_RESTRICT s, FILE * __ANCC_RESTRICT stream);
char* fgets(char * __ANCC_RESTRICT s, int n, FILE * __ANCC_RESTRICT stream);
#define putc(c, stream) fputc(c, stream)
#define putchar(c) fputc(c, stdout)
#define getc(stream) fgetc(stream)
#define getchar(c) fgetc(stdin)

int puts(const char * s);
char* gets(char* s);

size_t fwrite(const void * __ANCC_RESTRICT ptr, size_t size, size_t nmemb, FILE * __ANCC_RESTRICT stream);
int vsnprintf(char * __ANCC_RESTRICT s, size_t n, const char * __ANCC_RESTRICT format, va_list arg);
int snprintf(char * __ANCC_RESTRICT s, size_t n, const char * __ANCC_RESTRICT format, ...);
int vsprintf(char * __ANCC_RESTRICT s, const char * __ANCC_RESTRICT format, va_list arg);
int sprintf(char * __ANCC_RESTRICT s, const char * __ANCC_RESTRICT format, ...);
int vfnprintf(FILE * __ANCC_RESTRICT f, size_t n, const char * __ANCC_RESTRICT format, va_list arg);
int fnprintf(FILE * __ANCC_RESTRICT f, size_t n, const char * __ANCC_RESTRICT format, ...);
int vfprintf(FILE * __ANCC_RESTRICT f, const char * __ANCC_RESTRICT format, va_list arg);
int fprintf(FILE * __ANCC_RESTRICT f, const char * __ANCC_RESTRICT format, ...);
int printf(const char * format, ...);

int fclose(FILE *stream);
int fflush(FILE *stream);
FILE *fopen(const char * __ANCC_RESTRICT filename, const char * __ANCC_RESTRICT mode);

int fseek(FILE *stream, long int offset, int whence);
long int ftell(FILE *stream);
void rewind(FILE *stream);
int feof(FILE *stream);

#endif /* Guard word. */
