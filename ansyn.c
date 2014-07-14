/*
    ansyn: ancc syntax parser
    Aean, 2014.7.12
*/

#include "ancc.h"

int* lrstack;
size_t lrmax = 4096;
size_t lrp = 0;
int lrstate(){
    if(lrstack && lrp) return lrstack[lrp - 1];
    printf("internal error: lr stack don't have any states yet\n");
    exit(-1);
}
void lrpush(int state){
    if (!lrstack){
        lrstack = calloc(lrmax, sizeof(int));
        if(!lrstack) badalloc();
    }else if(lrp >= lrmax){
        int* newbase = calloc(lrmax * 2, sizeof(int));
        if(!newbase) badalloc();
        memcpy(newbase, lrstack, lrmax * sizeof(int));
        free(lrstack);
        lrstack = newbase;
        lrmax *= 2;
    }
    lrstack[lrp++] = state;
}
void lrreduce(int rno){
    rule_t rule = rule_list[rno];
    int rightlen = 0;
    while(rule.r[rightlen] != EOL) rightlen++;
    if (lrp <= rightlen){
        printf("internal error: reduction rule too long\n");
        exit(-1);
    }
    lrp -= rightlen;
    printf("\tstate %d, reduce %d, goto = %d\n", lrstate(), rightlen, goto_tbl[lrstate()][rule.left - NAL - 1]);
    lrpush( goto_tbl[lrstate()][rule.left - NAL - 1] );
}

void lr1(){
    lrpush(0);
    while(1){
        while(getchar()!='\n');
        int item = action_tbl[lrstate()][tla().no];
        printf("state %d, meet %s, action = %d\n", lrstate(), tla().val, item);
        if(item == 2147483647){
            printf("success.\n");
            return;
        }else if (item < 0){
            /** syntax error occured. */
            char expected[1024] = {0};
            char given[256];
            int i = 0;
            switch(tla().no){
                case C_CHAR: strcpy(given, "constchar"); break;
                case C_STRING: strcpy(given, "string"); break;
                case C_NUM: strcpy(given, "constnum"); break;
                case IDENT: strcpy(given, "identifier"); break;
                default: sprintf(given, "'%s'", tla().val);
            }
            int typed = 0, speced = 0, aggred = 0;
            for(i = 0; i <= NAL; i++){
                int sel = action_tbl[lrstate()][i];
                if (sel >= 0){
                    if (*expected && expected[strlen(expected)-1] != '/') strcat(expected, "/");
                    switch(i){
                        case C_CHAR: strcat(expected, "constchar"); break;
                        case C_STRING: strcat(expected, "string"); break;
                        case C_NUM: strcat(expected, "constnum"); break;
                        case IDENT: strcat(expected, "identifier"); break;

                        case K_AUTO: case K_CHAR: case K_INT: case K_LONG: case K_SHORT:
                        case K_UNSIGNED: case K_SIGNED: case K_FLOAT: case K_DOUBLE:
                        case K_COMPLEX: case K_BOOL: case K_VOID: case typedef_name:
                        if(!typed) strcat(expected, "typename"); typed = 1; break;

                        case K_REGISTER: case K_RESTRICT: case K_VOLATILE:
                        case K_STATIC: case K_CONST: case K_EXTERN:
                        if(!speced) strcat(expected, "typespec"); speced = 1; break;

                        case K_STRUCT: case K_ENUM: case K_UNION:
                        if(!aggred) strcat(expected, "aggregator"); aggred = 1; break;

                        case NAL: strcat(expected, "eof"); break;
                        default:
                            strcat(expected, "'");
                            strcat(expected, keyname(i));
                            strcat(expected, "'");
                    }
                }
            }

            err("syntax error, expected %s but given %s", expected, given);
            return;
        }else if(item >= 0 && item < familycount){
            lrpush(item);
            printf("\tshift %s\n", tla().val);
            tgf();
        }else{
            item -= familycount;
            printf("\treduce %d\n", item);
            lrreduce(item);
        }
    }
}
