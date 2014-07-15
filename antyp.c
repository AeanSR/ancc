/*
    antyp: ancc type subsystem
    Aean, 2014.7.15
*/

#include "ancc.h"

int type_equal( type_t* t1, type_t* t2 ){
    if (t1->spec != t2->spec) return 0;
    if (t1->qualifier != t2->qualifier) return 0;
    switch(t1->spec){
    case TYPESPEC_NUM:
        if (t1->u.as_num.size != t2->u.as_num.size) return 0;
        if (t1->u.as_num.sign != t2->u.as_num.sign) return 0;
        return 1;
    case TYPESPEC_AGGREGATE:
        if (t1->u.as_aggregate.is_union != t2->u.as_aggregate.is_union) return 0;
        {
            struct type_list* p1 = t1->u.as_aggregate.memberlist;
            struct type_list* p2 = t2->u.as_aggregate.memberlist;
            while(p1){
                if (!type_equal(p1->type, p2->type)) return 0;
                p1 = p1->next;
                p2 = p2->next;
                if (p1 && !p2) return 0;
                if (!p1 && p2) return 0;
            }
            return 1;
        }
    case TYPESPEC_FUNCTION:
        if (t1->u.as_function.is_inline != t2->u.as_function.is_inline) return 0;
        if (!type_equal(t1->u.as_function.returntype, t2->u.as_function.returntype)) return 0;
        {
            struct type_list* p1 = t1->u.as_function.parameterlist;
            struct type_list* p2 = t2->u.as_function.parameterlist;
            while(p1){
                if (!type_equal(p1->type, p2->type)) return 0;
                p1 = p1->next;
                p2 = p2->next;
                if (p1 && !p2) return 0;
                if (!p1 && p2) return 0;
            }
            return 1;
        }
    case TYPESPEC_POINTER:
        return type_equal(t1->u.as_pointer.pointee, t2->u.as_pointer.pointee);
    case TYPESPEC_ARRAY:
        return type_equal(t1->u.as_array.elemtype, t2->u.as_array.elemtype);
    }
    return 0;
}
