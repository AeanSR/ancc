/*
    anccc: another noobish C compiler's compiler
    Aean, 2014.7.6

    anccc is part of ancc project.
    ancc project is licensed under the terms of the MIT License.
    Copyright (C) 2014 Aean & HUST
*/

#include "anccc.h"

int main(int argc, char** argv){
    int i = 0;
    while(symbol_list[0][i].no != EOL)
        printf("%s\t", symbol_list[0][i++].name);
    putchar('\n');
    i = 0;
    while(symbol_list[1][i].no != EOL)
        printf("%s\t", symbol_list[1][i++].name);

}
