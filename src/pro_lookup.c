#include "pro_lookup.h"

#include "pro_common.h"
#include "pro_state.h"
#include "pro_env.h"
#include "pro_object.h"
#include "pro_env_lookup.h"



#pragma mark Internal

PRO_INTERNAL pro_lookup* pro_lookup_new(pro_state* s,
    pro_env_ref env, unsigned int index, unsigned int ref_count)
{
    pro_alloc* alloc;
    pro_get_alloc(s, &alloc);
    pro_lookup* t = alloc(0, sizeof(*t));
    if (!t) return 0;
    
    t->env = env;
    t->index = index;
    t->ref_count = ref_count;
    return t;
}


PRO_INTERNAL int pro_lookup_equal(pro_state* s,
    const pro_lookup* o1, const pro_lookup* o2)
{
    if (o1 == o2)
        return 1;
    
    if (PRO_EMPTY_REF == o1 || PRO_EMPTY_REF == o2)
        return 0;
    
    if (o1->index != o2->index)
        return 0;
    
    const pro_env* env1 = pro_env_dereference(s, o1->env);
    const pro_env* env2 = pro_env_dereference(s, o2->env);
    return (env1 == env2);
}


#pragma mark -
#pragma mark Public

pro_ref PRO_EMPTY_REF = 0;


PRO_API pro_error pro_retain(pro_state_ref s, pro_ref ref)
{
    if (!pro_lookup_equal(s, ref, PRO_EMPTY_REF))
        ref->ref_count++;
    return PRO_OK;
}


PRO_API pro_error pro_release(pro_state_ref s, pro_ref ref)
{
    if (pro_lookup_equal(s, ref, PRO_EMPTY_REF))
        return PRO_OK;
    
    assert(ref->ref_count > 0);
    
    if (--(ref->ref_count) <= 0)
    {    
        pro_alloc* alloc;
        pro_get_alloc(s, &alloc);
        
        // Release the object
        pro_object* obj = pro_dereference(s, ref);
        pro_object_release(s, obj);
        
        // Remove the ref from the env
        pro_env* env = pro_env_dereference(s, ref->env);
        pro_env_lookup_remove(s, env, ref);
        
        // Release the held environment
        pro_env_release(s, ref->env);

        // Free the pro_ref memory
        alloc(ref, 0);
    }
    
    return PRO_OK;
}

