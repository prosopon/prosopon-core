#include "pro_lookup_table.h"

#include "pro_object.h"



typedef struct pro_internal_lookup pro_internal_lookup;

/**
 * A structure used to store a list of lookups internally.
 */
struct pro_internal_lookup
{
    pro_internal_lookup* next; /**< next entry in the lookup table. */
    unsigned int index;
    pro_object* value;
};


/**
 * Creates a new internal lookup.
 *
 * @return The new internal lookup.
 */
static pro_internal_lookup* pro_internal_lookup_new(pro_state_ref s,
    pro_object* value, pro_internal_lookup* next, unsigned int index)
{
    pro_alloc* alloc;
    pro_get_alloc(s, &alloc);
    pro_internal_lookup* internal = alloc(0, sizeof(*internal));
    if (!internal) return 0;

    internal->value = value;
    internal->next = next;
    internal->index = index;
    return internal;
}



#pragma mark -
#pragma mark Internal

PRO_INTERNAL pro_lookup_table* pro_lookup_table_new(pro_state_ref s)
{
    pro_alloc* alloc;
    pro_get_alloc(s, &alloc);
    pro_lookup_table* t = alloc(0, sizeof(*t));
    if (!t) return 0;
    
    t->value = 0;
    t->size = 0;
    return t;
}


PRO_INTERNAL void pro_lookup_table_free(pro_state_ref s, pro_lookup_table* t)
{
    pro_alloc* alloc;
    pro_get_alloc(s, &alloc);
    
    for (pro_internal_lookup* internal = t->value; internal;)
    {
        pro_internal_lookup* next = internal->next;
        alloc(internal, 0);
        internal = next;
    }
    
    alloc(t, 0);
}


PRO_INTERNAL unsigned int pro_lookup_table_append(pro_state_ref s, pro_lookup_table* t)
{
    unsigned int index = t->size;
    pro_internal_lookup* internal = pro_internal_lookup_new(s, 0, 0, index);
    if (!internal) return 0;
    
    pro_internal_lookup* parent = t->value;
    if (!parent)
        t->value = internal;
    else
    {
        while (parent->next)
            parent = parent->next;
        parent->next = internal;
    }
        
    (t->size)++;
    return index;
}


PRO_INTERNAL void pro_lookup_table_remove(pro_state_ref s, pro_lookup_table* t,
    unsigned int index)
{
    pro_internal_lookup* parent = 0;
    for (pro_internal_lookup* internal = t->value; internal; internal = internal->next)
    {
        if (internal->index == index)
        {
            pro_internal_lookup* next = internal->next;
            
            // Remove the node from the list.
            if (0 == parent)
                t->value = next;
            else
                parent->next = next;
            
            // Free the node memory
            pro_alloc* alloc;
            pro_get_alloc(s, &alloc);
            alloc(internal, 0);
            
            return;
        }
        parent = internal;
    }
}



PRO_INTERNAL pro_object** pro_lookup_table_get(pro_state_ref s,
    pro_lookup_table* t, unsigned int index)
{
    for (pro_internal_lookup* internal = t->value; internal; internal = internal->next)
        if (internal->index == index)
            return &(internal->value);
    return 0;
}

