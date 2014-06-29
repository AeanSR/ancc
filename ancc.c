/*
    ancc: another noobish C compiler
    Aean, 2014.6.28 @HUST
*/

#include "ancc.h"

int main(){
    token_t t;
    //char t;
    pfin = fopen("anlex.c", "rb");
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
