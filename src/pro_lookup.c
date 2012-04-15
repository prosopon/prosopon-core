#include "pro_lookup.h"

#include "pro_state.h"
#include "pro_env.h"


pro_ref PRO_EMPTY_REF = 0;


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


