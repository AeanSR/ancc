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
    while(1){
        char c;
        scanf("%c", &c);
        while(getchar()!='\n');
        t = c == '1' ? tla() : tgf() ;
        printf("%s ", t.val);
    }
/*    read_source();
    while((t=CGF())){
        putchar(t);
    }*/
    return 1;
}
