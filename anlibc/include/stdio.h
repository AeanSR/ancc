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
#define __ANCC_RESTRICT /* restrict is not implemented. */
#endif

typedef unsigned FILE;
FILE* __anlib_getstream(unsigned no);
#define stdin  __anlib_getstream(-10)
#define stdout __anlib_getstream(-11)
#define stderr __anlib_getstream(-12)

int fputc(int c, FILE *stream);
int fputs(const char * __ANCC_RESTRICT s, FILE * __ANCC_RESTRICT stream);
#define putc(c, stream) fputc(c, stream)
#define putchar(c) fputc(c, stdout)
int puts(const char * s);
size_t fwrite(const void * __ANCC_RESTRICT ptr, size_t size, size_t nmemb, FILE * __ANCC_RESTRICT stream);
int vsnprintf(char * __ANCC_RESTRICT s, size_t n, const char * __ANCC_RESTRICT format, va_list arg);

#endif /* Guard word. */
