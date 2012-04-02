#include "pro_lookup.h"

#include <stdlib.h>


PRO_INTERNAL pro_ref pro_lookup_new(pro_state* s,
    struct pro_env* env, unsigned int index)
{
    pro_lookup* t = malloc(sizeof(*t));
    t->env = env;
    t->index = index;
    return t;
}

PRO_INTERNAL int pro_lookup_equal(pro_state* s, pro_ref o1, pro_ref o2)
{
    return (o1->env == o2->env && o1->index == o2->index);
}


PRO_INTERNAL pro_ref_list pro_lookup_list_new(pro_state* s,
    pro_ref value, pro_ref_list next)
{
    pro_ref_list t = malloc(sizeof(*t));
    t->value = value;
    t->next = next;
    return t;
}


PRO_INTERNAL void pro_lookup_list_append(pro_state* s,
    pro_ref_list t, pro_ref value)
{
    pro_ref_list parent = t;
    while (parent->next)
        parent = parent->next;
    parent->next = pro_lookup_list_new(s, value, 0);
}