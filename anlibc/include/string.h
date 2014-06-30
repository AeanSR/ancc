/*
    string.h
    Aean, 2014.6.30
    This file is part of anlibc, which itself is part of ancc project.
    ancc project is licensed under the terms of the MIT License.
    Copyright (C) 2014 Aean & HUST
*/

#ifndef __ANLIBC_STRING_INCLUDED_
#define __ANLIBC_STRING_INCLUDED_

#ifndef __ANCC_RESTRICT
#define __ANCC_RESTRICT /* restrict is not implemented. */
#endif

void *memcpy(void * __ANCC_RESTRICT s1, const void * __ANCC_RESTRICT s2, size_t n);
void *memmove(void *s1, const void *s2, size_t n);
char* strcpy(char* __ANCC_RESTRICT dst, const char* __ANCC_RESTRICT src);
void* memset(void* _dst, int _val, size_t _size);
size_t strlen(const char* str);

#endif /* Guard word. */
