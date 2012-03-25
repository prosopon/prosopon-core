#include "pro_env.h"

#include "prosopon.h"
#include "pro_state.h"

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
    pro_lookup* lookup;
};


/**
 * Creates a new internal lookup.
 *
 * @return The new internal lookup.
 */
static pro_internal_lookup* pro_internal_lookup_new(pro_state* s,
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
static pro_lookup_binding* pro_lookup_binding_new(pro_state* s,
    const char* identifier, pro_lookup* lookup, pro_lookup_binding* next)
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
static pro_internal_lookup* pro_env_get_internal_lookup(pro_state* s, 
    const pro_lookup* lookup)
{
    pro_env* env = lookup->env;
    pro_internal_lookup* internal_lookup = env->lookups;
    for (int index = lookup->index; index > 0; --index)
        internal_lookup = internal_lookup->next;
    return internal_lookup;
}


#pragma mark -
#pragma mark PRO_INTERNAL

PRO_INTERNAL pro_env* pro_env_new(pro_state* s, pro_env* previous, pro_env* parent)
{
    pro_env* e = malloc(sizeof(*e));
    e->previous = previous;
    e->parent = parent;
    return e;
}


PRO_INTERNAL pro_lookup* pro_env_next_lookup(pro_state* s,
    pro_env_lookup* env)
{
    pro_lookup* lookup = pro_lookup_new(s, env, env->size);
    pro_internal_lookup* internal = pro_internal_lookup_new(s, 0, 0);

    if (0 == env->lookups)
        env->lookups = internal;
    else
    {
        pro_internal_lookup* parent = env->lookups;
        while (parent->next)
            parent = parent->next;
        parent->next = internal;
    }
        
    (env->size)++;
    return lookup;
}


PRO_INTERNAL pro_object** pro_env_lookup_value(pro_state* s,
    pro_lookup* lookup)
{
    pro_internal_lookup* internal = pro_env_get_internal_lookup(s, lookup);
    return &(internal->value);
}
    

#pragma mark -
#pragma mark PRO_API

PRO_API pro_env_lookup* pro_env_create(pro_state* s, pro_env_lookup* parent)
{
    return pro_env_new(s, 0, parent);
}


PRO_API void pro_env_release(pro_state* s, pro_env_lookup* env)
{
    // TODO 
}


PRO_API void pro_bind(pro_state* s, pro_lookup* lookup, const char* id)
{
    assert(lookup);
    pro_env* env = pro_get_env(s);
    pro_internal_lookup* internal = pro_env_get_internal_lookup(s, lookup);
    if (internal)
    {
        pro_lookup_binding* binding = pro_lookup_binding_new(s, id, lookup, 0);
        pro_lookup_binding* parent = env->bindings;
        while (parent->next)
            parent = parent->next;
        parent->next = binding;
    }
}


PRO_API pro_lookup* pro_get_binding(pro_state* s,
    pro_env_lookup* env, const char* name)
{
    for (pro_lookup_binding* binding = env->bindings; binding; binding = binding->next)
    {
        char* lookup_identifier = binding->identifier;
        if (lookup_identifier && strcmp(name, lookup_identifier) == 0)
            return binding->lookup;                
    }
    
    if (env->parent)
        return pro_get_binding(s, env->parent, name);
    else
        return 0;
}


