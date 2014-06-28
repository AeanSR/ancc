/*
    ancc: another noobish C compiler
    Aean, 2014.6.28 @HUST
*/

#include "ancc.h"

int main(){
    //token_t t;
    char t;
    pfin = fopen("anlex.c", "rb");
    /*do{
        t = lexparse();
        printf("%d, \"%s\"\n", t.no, t.val);
    }while(t.no!=NAL);*/
    read_source();
    while((t=GF())){
        putchar(t);
    }
}
