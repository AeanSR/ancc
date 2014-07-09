/*
    first.c: calc first set
    Aean, 2014.7.8
*/

#include "anccc.h"

int* f[EOL] = {0};

int* FIRST( int nonterm ) {
    int buf[NAL] = {0};
    int* ret;
    if( f[nonterm] ) return f[nonterm];
    if( IST( nonterm ) ){
        ret = calloc(8, 1);
        ret[0] = nonterm;
        ret[1] = EOL;
        return ret;
    }
    int i = 0, j = 0, k = 0, l = 0;
    for( ; rule_list[i].left != EOL; i++ ) {
        if( rule_list[i].left != nonterm ) continue;
        if( rule_list[i].r[0] == nonterm ) continue;
        if( IST( rule_list[i].r[0] ) ) {
            l = j - 1;
            while( l >= 0 && rule_list[i].r[0] != buf[l] ) l--;
            if( l < 0 ) buf[j++] = rule_list[i].r[0];
        } else if( ISNT( rule_list[i].r[0] ) ) {
            ret = FIRST( rule_list[i].r[0] );
            k = 0;
            while( ret[k] != EOL ) {
                l = j - 1;
                while( l >= 0 && ret[k] != buf[l] ) l--;
                if( l < 0 ) buf[j++] = ret[k];
                k++;
            }
        } else printf( "bug: not a terminal nor a nonterminal\n" );
    }
    buf[j++] = EOL;
    f[nonterm] = malloc( j * sizeof( int ) );
    memcpy( f[nonterm], buf, j * sizeof( int ) );
    return f[nonterm];
}
