/*
    ancc: another noobish C compiler
    Aean, 2014.6.28
        Website: http://aean.net/  Mail: v@aean.net

    ancc project is licensed under the terms of the MIT License.
    Copyright (C) 2014 Aean & HUST
*/

#include "ancc.h"

int main(){
    token_t t;
    //char t;
    push_file("anlex.c");
    read_source();
    do{
        t = lexparse();
        printf("%s ", t.val);
    }while(t.no!=NAL);
/*    read_source();
    while((t=CGF())){
        putchar(t);
    }*/
    return 1;
}
