#include "pro_lookup.h"

#include "pro_state.h"
#include "pro_env.h"


pro_ref PRO_EMPTY_REF = 0;
pro_env_ref PRO_EMPTY_ENV_REF = 0;


PRO_INTERNAL pro_lookup* pro_lookup_new(pro_state* s,
    pro_env_ref env, unsigned int index, unsigned int ref_count)
{
    pro_alloc* alloc;
    pro_get_alloc(s, &alloc);
    pro_lookup* t = alloc(0, sizeof(*t));
    if (!t) return 0;
    pro_env_retain(s, env);
    t->env = env;
    t->index = index;
    t->ref_count = ref_count;
    return t;
}

PRO_INTERNAL int pro_lookup_equal(pro_state* s,
    const pro_lookup* o1, const pro_lookup* o2)
{
    return (o1->env == o2->env && o1->index == o2->index);
}


PRO_INTERNAL pro_env_lookup* pro_env_lookup_new(pro_state* s,
    struct pro_env* value, unsigned int ref_count)
{
    pro_alloc* alloc;
    pro_get_alloc(s, &alloc);
    pro_env_lookup* t = alloc(0, sizeof(*t));
    if (!t) return 0;
    t->value = pro_internal_env_retain(s, value);
    t->ref_count = ref_count;
    return t;
}


PRO_INTERNAL int pro_env_lookup_equal(pro_state* s,
    const pro_env_lookup* o1, const pro_env_lookup* o2)
{
    return (o1->value == o2->value);
}


PRO_INTERNAL pro_ref_list pro_lookup_list_new(pro_state* s,
    pro_ref value, pro_ref_list next)
{
    pro_alloc* alloc;
    pro_get_alloc(s, &alloc);
    pro_ref_list t = alloc(0, sizeof(*t));
    if (!t) return 0;
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

