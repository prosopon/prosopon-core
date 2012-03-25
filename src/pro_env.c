#include "pro_env.h"

#include "prosopon.h"
#include "pro_state.h"

#include <stdlib.h>
#include <string.h>
#include <assert.h>


#pragma private

struct pro_internal_lookup
{
    pro_internal_lookup* next;
    char* identifier;
    pro_object* value;
};


static pro_internal_lookup* pro_env_get_internal_lookup(pro_state* s, 
    const pro_lookup* lookup)
{
    pro_env* env = lookup->env;
    pro_internal_lookup* internal_lookup = env->table;
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
    pro_internal_lookup* internal = malloc(sizeof(*internal));
    internal->next = 0;
    internal->identifier = 0;
    internal->value = 0;

    pro_lookup* lookup = pro_lookup_new(s, env, env->size);

    if (0 == env->table)
        env->table = internal;
    else
    {
        pro_internal_lookup* parent = env->table;
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


PRO_API void pro_bind(pro_state* s, const pro_lookup* lookup, const char* id)
{
    assert(lookup);
    pro_internal_lookup* internal = pro_env_get_internal_lookup(s, lookup);
    if (internal)
    {
        free(internal->identifier);
        internal->identifier = malloc(sizeof(*internal->identifier) * strlen(id));
        strcpy(internal->identifier, id);
    }
}


PRO_API pro_lookup* pro_get_binding(pro_state* s,
    pro_env_lookup* env, const char* name)
{
    pro_internal_lookup* lookup = env->table;
    unsigned int index = 0;
    while (lookup)
    {
        char* lookup_identifier = lookup->identifier;
        if (lookup_identifier && strcmp(name, lookup_identifier) == 0)
            return pro_lookup_new(s, env, index);
        else
            lookup = lookup->next;
        index++;
    }
    
    if (env->parent)
        return pro_get_binding(s, env->parent, name);
    else
        return 0;
}


