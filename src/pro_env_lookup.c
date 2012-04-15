#include "pro_env_lookup.h"

#include "pro_env.h"
#include "pro_state.h"


pro_env_ref PRO_EMPTY_ENV_REF = 0;

PRO_INTERNAL pro_env_lookup* pro_env_lookup_new(pro_state_ref s,
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


PRO_INTERNAL int pro_env_lookup_equal(pro_state_ref s,
    const pro_env_lookup* o1, const pro_env_lookup* o2)
{
    return (o1->value == o2->value);
}