/*
    anccc: another noobish C compiler's compiler
    Aean, 2014.7.6

    anccc is part of ancc project.
    ancc project is licensed under the terms of the MIT License.
    Copyright (C) 2014 Aean & HUST
*/

#include "anccc.h"

void test_first(){
    int i, j, k;
    int* p;
    char c[100];
    while(1){
        i=-1;
        scanf("%s", c);
        for(k=0;nonterminal_list[k].no!=EOL;k++)
            if (0==strcmp(c, nonterminal_list[k].name)){
                i=nonterminal_list[k].no;
                break;
            }
        if (i==-1){
            printf("not a nonterminal\n");
            continue;
        }
        j = 0;
        p = FIRST(i);
        while(p[j]!=EOL){
            for(k=0;k<=NAL;k++)
                if( terminal_list[k].no == p[j] ){
                    printf(terminal_list[k].name);
                    break;
                }
            j++;
            printf(", ");
        }
    }
}

int main(){
    itemset_t* set = conset(conitem(0, 0, NAL));
    printset(set);
    printf("-------------\n");
    closure(set);
    printset(set);
}
