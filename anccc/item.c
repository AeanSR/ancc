/*
    item.c: item set manipulator
    Aean, 2014.7.8
*/

#include "anccc.h"

unsigned long long zobrist_table[254][11][NAL+1];
int zobrfilled = 0;

void zobrfill(){
    unsigned long long seed = 4262, rn;
    int i, j, k;
    for(i=0;i<254;i++)
    for(j=0;j<11;j++)
    for(k=0;k<=NAL;k++){
        rn = seed = seed * 2862933555777941757ULL + 3037000493ULL;
        rn ^= rn >> 14;
        rn ^= rn << 11;
        rn ^= rn << 53;
        rn ^= rn >> 49;
        zobrist_table[i][j][k] = rn;
    }
    zobrfilled = 1;
}

void rezobr(itemset_t* set){
    item_t* p = set->item;
    if(!zobrfilled) zobrfill();
    set->zobrist = 0;
    while(p){
        set->zobrist ^= zobrist_table[p->rule][p->p][p->la];
        p=p->next;
    }
}

item_t* conitem(int rule, int p, int la){
    item_t* ret = calloc(sizeof(item_t), 1);
    ret->rule = rule;
    ret->p = p;
    ret->la = la;
    return ret;
}

itemset_t* conset(item_t* core){
    itemset_t* ret = calloc(sizeof(itemset_t), 1);
    ret->item = core;
    ret->zobrist = 0;
    return ret;
}

int itemeq(item_t* t1, item_t* t2){
    return t1->la == t2->la && t1->p == t2->p && t1->rule == t2->rule;
}

int setdup(itemset_t* s1, itemset_t* s2){
    rezobr(s1);
    rezobr(s2);
    return s1->zobrist == s2->zobrist;
}

int additem(itemset_t* set, item_t* newitem){
    if (!set->item){
        set->item = newitem;
        return 1;
    }else{
        int flag = 0;
        item_t* p = set->item;
        while(p->next){
            if (itemeq(p, newitem)){
                flag = 1;
                break;
            }
            p = p->next;
        }
        if (!flag){
            if (!itemeq(p, newitem)){
                p->next = newitem;
                return 1;
            }
        }
    }
    return 0;
}

void closure(itemset_t* set){
    item_t* p = set->item;
    int flag = 0;
    if (!p) return;
    do{
        while(p){
            flag = 0;
            int sym = rule_list[p->rule].r[p->p];
            if (ISNT(sym)){
                int i = 0;
                while(rule_list[i].left != EOL){
                    if (rule_list[i].left == sym){
                        int* la;
                        int j = 0;
                        if(rule_list[p->rule].r[p->p + 1] == EOL){
                            la = alloca(8);
                            la[0] = p->la;
                            la[1] = EOL;
                        }else{
                            la = FIRST(rule_list[p->rule].r[p->p + 1]);
                        }
                        while(la[j] != EOL)
                            flag |= additem(set, conitem(i, 0, la[j++]));
                    }
                    i++;
                }
            }
            p=p->next;
        }
    }while(flag);
}

char* nameofsym(int sym){
    symbol_t* list = symbol_list[ISNT(sym)];
    while(list->no != EOL){
        if (list->no == sym) return list->name;
        list++;
    }
    return 0;
}

void printitem(item_t* item){
    int i;
    printf( "%s ->", nameofsym( rule_list[item->rule].left ) );
    for(i=0; rule_list[item->rule].r[i]!=EOL; i++){
        if ( i==item->p ) printf(" .");
        printf(" %s", nameofsym(rule_list[item->rule].r[i]));
    }
    printf(", %s\n", nameofsym(item->la));
}

void printset(itemset_t* set){
    item_t* p = set->item;
    if (!p){
        printf("[Empty set]\n");
        return;
    }
    while(p){
        printitem(p);
        p = p->next;
    }
}
