#include "pro_env.h"

#include "prosopon.h"
#include "pro_state.h"
#include "pro_common.h"

#include <stdlib.h>
#include <string.h>
#include <assert.h>


#pragma private


/**
 * A structure used to store a list of lookups internally.
 */
struct pro_internal_lookup
{
    pro_internal_lookup* next; /**< next entry in the lookup table. */
    pro_object* value;
};


/**
 *
 */
struct pro_lookup_binding
{
    pro_lookup_binding* next;
    char* identifier;
    pro_ref lookup;
};


/**
 * Creates a new internal lookup.
 *
 * @return The new internal lookup.
 */
static pro_internal_lookup* pro_internal_lookup_new(pro_state_ref s,
    pro_object* value, pro_internal_lookup* next)
{
    pro_internal_lookup* internal = malloc(sizeof(*internal));
    internal->value = value;
    internal->next = next;
    return internal;
}


/**
 * Creates a new internal lookup.
 *
 * @return The new internal lookup.
 */
static pro_lookup_binding* pro_lookup_binding_new(pro_state_ref s,
    const char* identifier, pro_ref lookup, pro_lookup_binding* next)
{
    pro_lookup_binding* internal = malloc(sizeof(*internal));
    internal->lookup = lookup;
    internal->next = next;
    internal->identifier = malloc(sizeof(*internal->identifier) * strlen(identifier));
    strcpy(internal->identifier, identifier);
    return internal;
 }
 

/**
 * Given a external lookup object, gets the corresponding internal lookup.
 *
 * @return The internal lookup.
 */
static pro_internal_lookup* pro_env_get_internal_lookup(pro_state_ref s, 
    pro_ref lookup)
{
    pro_env_ref env = lookup->env;
    pro_internal_lookup* internal_lookup = env->value->lookups;
    for (int index = lookup->index; index > 0; --index)
        internal_lookup = internal_lookup->next;
    return internal_lookup;
}


#pragma mark -
#pragma mark PRO_INTERNAL

PRO_INTERNAL pro_env* pro_env_new(pro_state_ref s, pro_env_ref parent)
{
    pro_env* e = malloc(sizeof(*e));
    memset(e, 0, sizeof(*e));
    e->parent = parent;
    return e;
}


PRO_INTERNAL pro_ref pro_env_next_lookup(pro_state_ref s,
    pro_env_ref env)
{
    pro_lookup* lookup = pro_lookup_new(s, env, env->value->size);
    pro_internal_lookup* internal = pro_internal_lookup_new(s, 0, 0);

    if (0 == env->value->lookups)
        env->value->lookups = internal;
    else
    {
        pro_internal_lookup* parent = env->value->lookups;
        while (parent->next)
            parent = parent->next;
        parent->next = internal;
    }
        
    (env->value->size)++;
    return lookup;
}


PRO_INTERNAL pro_object** pro_env_lookup_value(pro_state_ref s,
    pro_ref lookup)
{
    pro_internal_lookup* internal = pro_env_get_internal_lookup(s, lookup);
    return &(internal->value);
}
    

#pragma mark -
#pragma mark PRO_API

PRO_API pro_error pro_env_create(pro_state_ref s, pro_env_ref parent,
    PRO_OUT pro_env_ref* env_out)
{
    if (!s) return PRO_INVALID_OPERATION;
    pro_env_lookup* env = pro_env_lookup_new(s, pro_env_new(s, parent));
    if (0 == env) return PRO_OUT_OF_MEMORY;
    *env_out = env; 
    return PRO_OK;
}


PRO_API pro_error pro_env_release(pro_state_ref s, pro_env_ref env)
{
    // TODO
    return PRO_OK;
}


PRO_API pro_error pro_bind(pro_state_ref s, pro_ref ref, const char* id)
{
    PRO_API_ASSERT(s, PRO_INVALID_OPERATION);
    PRO_API_ASSERT(PRO_EMPTY_REF != ref, PRO_INVALID_ARGUMENT);
    PRO_API_ASSERT(id, PRO_INVALID_OPERATION);

    pro_env_ref env_ref;
    pro_get_env(s, &env_ref);
    pro_env* env = env_ref->value;
    
    pro_internal_lookup* internal = pro_env_get_internal_lookup(s, ref);
    if (internal)
    {
        pro_lookup_binding* binding = pro_lookup_binding_new(s, id, ref, 0);
        if (0 == env->bindings)
            env->bindings = binding;
        else
        {
            pro_lookup_binding* parent = env->bindings;
            while (parent->next)
                parent = parent->next;
            parent->next = binding;
        }
    }
    
    return PRO_OK;
}


PRO_API pro_error pro_get_binding(pro_state_ref s,
    pro_env_ref env, const char* name,  PRO_OUT pro_ref* ref)
{
    for (pro_lookup_binding* binding = env->value->bindings; binding; binding = binding->next)
    {
        char* lookup_identifier = binding->identifier;
        if (lookup_identifier && strcmp(name, lookup_identifier) == 0)
        {
            *ref = binding->lookup;                
            return PRO_OK;
        }
    }
    
    if (env->value->parent)
        return pro_get_binding(s, env->value->parent, name, ref);
    else
    {
        *ref = PRO_EMPTY_REF;
        return PRO_OK;
    }
}


