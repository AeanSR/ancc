/*
    ancc: another noobish C compiler
    Aean, 2014.6.28
        Website: http://aean.net/  Mail: v@aean.net

    ancc project is licensed under the terms of the MIT License.
    Copyright (C) 2014 Aean
*/

#include "ancc.h"

int main(){
    token_t t;
    push_file("anlex.c");
    preprocess();
    read_source();
    lr1();
    printf("%d errors, %d warnings\n", error_occured, warning_occured);
/*    read_source();
    while((t=CGF())){
        putchar(t);
    }*/
    return 1;
}
