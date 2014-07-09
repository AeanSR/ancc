/*
    malloc.c
    Aean, 2014.7.2
    This file is part of anlibc, which itself is part of ancc project.
    ancc project is licensed under the terms of the MIT License.
    Copyright (C) 2014 Aean
*/

#include "anlibc.h"

void* malloc(size_t size){
    return HeapAlloc(GetProcessHeap(), 0, size);
}
void free(void* p){
    HeapFree(GetProcessHeap(), 0, p);
}
void* calloc(size_t c, size_t s){
    return HeapAlloc(GetProcessHeap(), 0x8, c * s);
}
void* realloc(void *ptr, size_t size){
    return HeapReAlloc(GetProcessHeap(), 0, ptr, size);
}
