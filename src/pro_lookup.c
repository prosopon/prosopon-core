#include "pro_lookup.h"

#include "pro_state.h"
#include "pro_env.h"
#include "pro_object.h"
#include "pro_env_lookup.h"

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
    const pro_env* env1 = pro_env_dereference(s, o1->env);
    const pro_env* env2 = pro_env_dereference(s, o2->env);
    return (env1 == env2 && o1->index == o2->index);
}

#pragma mark -
#pragma mark Public


PRO_API pro_error pro_retain(pro_state_ref s, pro_ref ref)
{
    if (PRO_EMPTY_REF != ref)
        ref->ref_count++;
    return PRO_OK;
}


PRO_API pro_error pro_release(pro_state_ref s, pro_ref ref)
{
    if (PRO_EMPTY_REF == ref)
        return PRO_OK;
    
    if (--(ref->ref_count) <= 0)
    {    
        pro_alloc* alloc;
        pro_get_alloc(s, &alloc);
        
        pro_object* obj = pro_dereference(s, ref);
        pro_object_release(s, obj, ref);
        
        // release from env
        
        pro_env* env = pro_env_dereference(s, ref->env);
        pro_env_lookup_remove(s, env, ref);
       // pro_env_release(s, ref->env);
        
        alloc(ref, 0);
    }
    
    return PRO_OK;
}