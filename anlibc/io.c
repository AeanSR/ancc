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
