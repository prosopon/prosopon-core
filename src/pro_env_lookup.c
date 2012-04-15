#include "pro_env_lookup.h"

#include "pro_common.h"
#include "pro_env.h"


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


PRO_INTERNAL pro_env* pro_env_dereference(pro_state_ref s, pro_env_ref env_ref)
{
    return env_ref->value;
}


#pragma mark -
#pragma mark Public

PRO_API pro_error pro_env_retain(pro_state_ref s, pro_env_ref env_ref)
{
    PRO_API_ASSERT(s, PRO_INVALID_OPERATION);
    env_ref->ref_count++;
    return PRO_OK;
}


PRO_API pro_error pro_env_release(pro_state_ref s, pro_env_ref env_ref)
{    
    if (--(env_ref->ref_count) <= 0)
    {
        pro_alloc* alloc;
        pro_get_alloc(s, &alloc);
        
        // Release the env
        pro_env* env = pro_env_dereference(s, env_ref);
        pro_internal_env_release(s, env);
        
        // Free the pro_env_ref memory
        alloc(env_ref, 0);
    }
    
    return PRO_OK;
}
