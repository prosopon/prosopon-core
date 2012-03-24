#include "pro_env.h"

#include "prosopon.h"
#include "pro_state.h"

#include <stdlib.h>
#include <string.h>
#include <assert.h>


struct pro_internal_lookup
{
    pro_internal_lookup* next;
    char* identifier;
    pro_lookup* lookup;
};


PRO_INTERNAL pro_env* pro_env_new(pro_state* s, pro_env* previous, pro_env* parent)
{
    pro_env* e = malloc(sizeof(*e));
    e->previous = previous;
    e->parent = parent;
    return e;
}


PRO_API pro_env_lookup* pro_env_create(pro_state* s, pro_env_lookup* parent)
{
    return pro_env_new(s, 0, parent);
}


PRO_API pro_env_lookup* pro_env_get(pro_state* s)
{
    return s->current_env;
}


PRO_API void pro_env_push(pro_state* s, pro_env_lookup* env)
{
    pro_env* current_env = s->current_env;
    env->previous = current_env;
    s->current_env = env;
}


PRO_API void pro_env_pop(pro_state* s)
{
    s->current_env = s->current_env->previous;
}


PRO_API void pro_env_release(pro_state* s, pro_env_lookup* env)
{
    // TODO 
}


PRO_API void pro_lookup_bind(pro_state* s, const pro_lookup* lookup, const char* id)
{
    assert(lookup);
    for (pro_internal_lookup* internal = lookup->env->table; internal; internal = internal->next)
    {
        if (pro_lookup_equal(s, internal->lookup, lookup))
        {
            free(internal->identifier);
            internal->identifier = malloc(sizeof(*internal->identifier) * strlen(id));
            strcpy(internal->identifier, id);
        }
    }
}
