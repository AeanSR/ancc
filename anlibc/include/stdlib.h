/*
    stdlib.h
    Aean, 2014.7.3
    This file is part of anlibc, which itself is part of ancc project.
    ancc project is licensed under the terms of the MIT License.
    Copyright (C) 2014 Aean & HUST
*/

#ifndef __ANLIBC_STDLIB_INCLUDED_
#define __ANLIBC_STDLIB_INCLUDED_

void* malloc(size_t size);
void free(void* p);
void* calloc(size_t c, size_t s);
void* realloc(void *ptr, size_t size);

#endif /* Guard word. */
