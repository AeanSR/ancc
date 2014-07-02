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

void* memcpy(void * __ANCC_RESTRICT s1, const void * __ANCC_RESTRICT s2, size_t n);
void* memmove(void *s1, const void *s2, size_t n);
char* strcpy(char* __ANCC_RESTRICT dst, const char* __ANCC_RESTRICT src);
char* strncpy(char* __ANCC_RESTRICT dst, const char* __ANCC_RESTRICT src, size_t n);
char* strcat(char* __ANCC_RESTRICT dst, const char* __ANCC_RESTRICT src);
char* strncat(char* __ANCC_RESTRICT dst, const char* __ANCC_RESTRICT src, size_t n);
int memcmp(const void *s1, const void *s2, size_t n);
int strcmp(const char *s1, const char *s2);
int strncmp(const char *s1, const char *s2, size_t n);
void* memchr(const void* ptr, int _ch, size_t n);
char* strchr(const char* _str, int _ch);
char* strstr(const char* _str, const char* _ch);
void* memset(void* _dst, int _val, size_t _size);
unsigned int strlen(const char* str);

#endif /* Guard word. */
