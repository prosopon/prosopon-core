#include "pro_env.h"

#include "pro_state.h"
#include "pro_common.h"
#include "pro_object.h"
#include "pro_lookup.h"
#include "pro_env_lookup.h"
#include "pro_binding_map.h"


#pragma mark PRO_INTERNAL


typedef struct pro_internal_lookup pro_internal_lookup;
typedef struct pro_lookup_binding pro_lookup_binding;

/**
 * A structure that holds scope information. 
 */
struct pro_env
{
    unsigned int ref_count;
    
    pro_env_ref parent; /**< The parent environment used for delegation. */
    
    pro_binding_map* bindings;
};


PRO_INTERNAL pro_env* pro_env_new(pro_state_ref s,
    pro_env_ref parent, unsigned int ref_count)
{
    pro_alloc* alloc;
    pro_get_alloc(s, &alloc);
    pro_env* e = alloc(0, sizeof(*e));
    if (!e) return 0;
    
    e->parent = parent;
    e->ref_count = ref_count;
    e->bindings = 0; // allocated when needed.
    return e;
}


PRO_INTERNAL void pro_env_free(pro_state_ref s, pro_env* t)
{
    pro_alloc* alloc;
    pro_get_alloc(s, &alloc);
    
    // free the binding table
    pro_env_unbind_all(s, t);
    
    // release parent env
    pro_env_release(s, t->parent);
    
    // free env structure memory
    alloc(t, 0);
}


PRO_INTERNAL pro_env* pro_internal_env_retain(pro_state_ref s, pro_env* t)
{
    (t->ref_count)++;
    return t;
}


PRO_INTERNAL void pro_internal_env_release(pro_state_ref s, pro_env* t)
{
    if (--(t->ref_count) == 0)
        pro_env_free(s, t);
}


PRO_INTERNAL pro_env_ref pro_env_get_parent(pro_state_ref s, pro_env* t)
{
    return t->parent;
}


PRO_INTERNAL pro_object* pro_dereference(pro_state_ref s, pro_ref ref)
{
    return pro_lookup_equal(s, ref, PRO_EMPTY_REF) ? 0 : ref->obj;
}

PRO_INTERNAL void pro_env_unbind_all(pro_state_ref s, pro_env* t)
{
    pro_binding_map* bindings = t->bindings;
    if (bindings)
    {
        t->bindings = 0; // prevents double release of bindings
        pro_binding_map_free(s, bindings);
    }
}


#pragma mark -
#pragma mark PRO_API

PRO_API pro_error pro_env_create(pro_state_ref s, pro_env_ref parent,
    PRO_OUT pro_env_ref* env_out)
{
    PRO_API_ASSERT(s, PRO_INVALID_OPERATION);
    
    // create the new env
    pro_env_retain(s, parent);
    pro_env* env = pro_env_new(s, parent, 1);
    PRO_API_ASSERT(env, PRO_OUT_OF_MEMORY);

    // create a new env_ref for the new env
    pro_env_lookup* env_lookup = pro_env_lookup_new(s, env, 1);
    PRO_API_ASSERT(env_lookup, PRO_OUT_OF_MEMORY);    
    *env_out = env_lookup; 
    
    return PRO_OK;
}


PRO_API pro_error pro_bind(pro_state_ref s, pro_ref ref, const char* id)
{
    PRO_API_ASSERT(s, PRO_INVALID_OPERATION);
    PRO_API_ASSERT(PRO_EMPTY_REF != ref, PRO_INVALID_ARGUMENT);
    PRO_API_ASSERT(id, PRO_INVALID_OPERATION);

    // retain bound value
    pro_retain(s, ref);
    
    // get the current environment
    pro_env_ref env_ref;
    pro_get_env(s, &env_ref);
    pro_env* env = pro_env_dereference(s, env_ref);

    // bind the ref in the current environment
    if (!env->bindings)
        env->bindings = pro_binding_map_new(s);
    pro_binding_map_put(s, env->bindings, id, ref);
    
    // release hold on current environment
    pro_env_release(s, env_ref);
    
    return PRO_OK;
}


PRO_API pro_error pro_get_binding(pro_state_ref s,
    pro_env_ref env_ref, const char* name,  PRO_OUT pro_ref* ref)
{
    PRO_API_ASSERT(s, PRO_INVALID_OPERATION);
    PRO_API_ASSERT(PRO_EMPTY_ENV_REF != env_ref, PRO_INVALID_ARGUMENT);

    pro_env* env = pro_env_dereference(s, env_ref);
    
    // attempt to get value from binding map
    pro_binding_map* binding_map = env->bindings;
    if (binding_map)
    {
        pro_ref val = pro_binding_map_get(s, binding_map, name);
        if (!pro_lookup_equal(s, val, PRO_EMPTY_REF))
        {
            *ref = val;
            return PRO_OK;
        }
    }
    
    if (env->parent) // try to check parent
        return pro_get_binding(s, env->parent, name, ref);
    else // no binding found, return empty ref
    {
        *ref = PRO_EMPTY_REF;
        return PRO_OK;
    }
}

