/*
    derive.c: derive item sets
    Aean, 2014.7.8
*/

#include "anccc.h"

typedef struct migtbl_t{
    int parent;
    int symbol;
    int derived;
    struct migtbl_t* next;
} migtbl_t;

migtbl_t* migrate_table = 0;
itemset_t** setfamily = 0;
size_t familymax = 0;
size_t familycount = 0;

int family(itemset_t* set){
    int i = 0;
    if (!setfamily){
        setfamily = calloc(1024 , sizeof(itemset_t*));
        familymax = 1024;
    }
    if (setfamily[familymax - 1]){
        itemset_t** newbase = calloc( familymax * 2, sizeof(itemset_t*) );
        memcpy(newbase, setfamily, familymax * sizeof(itemset_t*));
        free(setfamily);
        setfamily = newbase;
        familymax *= 2;
    }
    while(setfamily[i] && !setdup(setfamily[i], set)){
        i++;
    }
    if (!setfamily[i]){
        setfamily[i] = set;
        familycount++;
    }
    else delset(set);
    return i;
}

void recmigtbl(int set, int x, int d){
    if (!migrate_table){
        migrate_table = calloc(sizeof(migtbl_t), 1);
        migrate_table->parent = set;
        migrate_table->symbol = x;
        migrate_table->derived = d;
        return;
    }else{
        migtbl_t* p = migrate_table;
        while(p->next)
            p=p->next;
        p->next = calloc(sizeof(migtbl_t), 1);
        p->next->parent = set;
        p->next->symbol = x;
        p->next->derived = d;
        return;
    }
}

int do_migrate(int set, int x){
    itemset_t* j;
    item_t* p = setfamily[set]->item;
    int f;
    if (!p) return -1;
    if (!IST(x) && !ISNT(x)) return -1;
    if (x == NAL) return -1;
    j = conset(0);
    while(p){
        if (rule_list[p->rule].r[p->p] == x){
            additem(j, conitem(p->rule, p->p+1, p->la));
        }
        p = p->next;
    }
    if(!j->item) return -1;
    closure(j);
    f = family(j);
    recmigtbl(set, x, f);
    return f;
}

int migrate(int set, int x){
    migtbl_t* p = migrate_table;
    while(p){
        if (p->parent == set && p->symbol == x)
            return p->derived;
        p=p->next;
    }
    return do_migrate(set, x);
}

char* spaces(size_t num){
    static char buf[] = "          ";
    return buf + num;
}

void derive(){
    itemset_t* coreset = conset(conitem(0, 0, NAL));
    size_t oldcount = 0, oldoldcount = 1;
    char buf1[16], buf2[16];
    printf("Discovering the Canonical LR(1) item set family.\n");
    closure(coreset);
    family(coreset);
    while(oldcount != familycount){
        int i, j;
        printf("Walking on [%Iu, %Iu]:%s[>_____________________]", oldcount, familycount,
               spaces(strlen(itoa(oldcount, buf1, 10))+strlen(itoa(familycount, buf2, 10))));
        oldoldcount = oldcount ? oldcount : 1;
        oldcount = familycount;
        for(i=oldoldcount-1; i<oldcount; i++){
            item_t* p = setfamily[i]->item;
            while(p){
                j = rule_list[p->rule].r[p->p];
                if(j!=EOL)
                    migrate(i, j);
                p=p->next;
                //printf("%Iu, %Iu\n", oldcount, familycount);
            }
            if( !(i & 0xF) || i == oldcount - 1 ){
                printf("\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b");
                putchar('[');
                int n;
                for( n = (i - oldoldcount + 1) * 21 / (oldcount - oldoldcount + 1); n >= 0; n-- )
                    putchar('=');
                putchar('>');
                for( n = (i - oldoldcount + 1) * 21 / (oldcount - oldoldcount + 1); n < 20; n++ )
                    putchar('_');
                putchar(']');
            }
        }
        printf("\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b");
        printf("[=====================>]");
        printf(" Discovered %Iu new set.\n", familycount - oldcount);
    }
    printf("%Iu set in total.\n\n", familycount);
}

void printfamily(){
    int i;
    for(i=0; i<familycount; i++){
        printset(setfamily[i]);
        printf("--------\n");
    }
}
