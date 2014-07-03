/*
    stdio.h
    Aean, 2014.7.3
    This file is part of anlibc, which itself is part of ancc project.
    ancc project is licensed under the terms of the MIT License.
    Copyright (C) 2014 Aean & HUST
*/

#ifndef __ANLIBC_STDIO_INCLUDED_
#define __ANLIBC_STDIO_INCLUDED_

typedef unsigned FILE;
FILE* __anlib_getstream(unsigned no);
#define stdin  __anlib_getstream(-10)
#define stdout __anlib_getstream(-11)
#define stderr __anlib_getstream(-12)

int fputc(int c, FILE *stream);


#endif /* Guard word. */
