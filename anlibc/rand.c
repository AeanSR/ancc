/*
    rand.c
    Aean, 2014.7.5
    This file is part of anlibc, which itself is part of ancc project.
    ancc project is licensed under the terms of the MIT License.
    Copyright (C) 2014 Aean & HUST
*/

static unsigned int __anlibc_seed = 4262;

int rand(void){
    __anlibc_seed = __anlibc_seed * 214013L + 2531011L;
    return (__anlibc_seed >> 16) & 0x7fff;
}

void srand(unsigned int seed){
    __anlibc_seed = seed;
}
