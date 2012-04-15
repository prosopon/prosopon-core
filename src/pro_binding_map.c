#include "pro_binding_map.h"

#include <string.h>


#pragma mark Private

typedef struct pro_lookup_binding pro_lookup_binding;

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
 */
static pro_lookup_binding* pro_lookup_binding_new(pro_state_ref s,
    const char* identifier, pro_ref lookup, pro_lookup_binding* next)
{
    pro_alloc* alloc;
    pro_get_alloc(s, &alloc);
    pro_lookup_binding* internal = alloc(0, sizeof(*internal));
    if (!internal) return 0;
    
    internal->lookup = lookup;
    internal->next = next;
    internal->identifier = alloc(0, sizeof(*internal->identifier) * (strlen(identifier) + 1));
    if (!internal->identifier)
    {
        alloc(internal, 0);
        return 0;
    }
    
    strcpy(internal->identifier, identifier);
    return internal;
}

static void pro_lookup_binding_free(pro_state_ref s,
    pro_lookup_binding* t)
{
    pro_alloc* alloc;
    pro_get_alloc(s, &alloc);
    
    alloc(t->identifier, 0);
    alloc(t, 0);
}

#pragma mark -
#pragma mark Internal

PRO_INTERNAL pro_binding_map* pro_binding_map_new(pro_state_ref s)
{
    pro_alloc* alloc;
    pro_get_alloc(s, &alloc);
    pro_binding_map* t = alloc(0, sizeof(*t));
    if (!t) return 0;
    
    t->value = 0;
    return t;
}


PRO_INTERNAL void pro_binding_map_free(pro_state_ref s, pro_binding_map* t)
{
    pro_alloc* alloc;
    pro_get_alloc(s, &alloc);
    
    for (pro_lookup_binding* binding = t->value; binding; )
    {
        pro_lookup_binding* next = binding->next;
        pro_release(s, binding->lookup);
        pro_lookup_binding_free(s, binding);
        binding = next;
    }
    
    alloc(t, 0);
}


PRO_INTERNAL pro_ref pro_binding_map_get(pro_state_ref s, pro_binding_map* t, const char* name)
{
    for (pro_lookup_binding* binding = t->value; binding; binding = binding->next)
    {
        const char* lookup_identifier = binding->identifier;
        if (lookup_identifier && strcmp(name, lookup_identifier) == 0)
        {
            pro_retain(s, binding->lookup);
            return binding->lookup;
        }
    }
    
    return PRO_EMPTY_REF;
}


PRO_INTERNAL void pro_binding_map_put(pro_state_ref s, pro_binding_map* t, const char* name, pro_ref ref)
{
    pro_lookup_binding* binding = pro_lookup_binding_new(s, name, ref, 0);
    pro_lookup_binding* parent = t->value;
    if (!parent)
        t->value = binding;
    else
    {
        while (parent->next)
            parent = parent->next;
        parent->next = binding;
    }
}

