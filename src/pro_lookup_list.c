#include "pro_lookup_list.h"


PRO_INTERNAL
pro_ref_list pro_lookup_list_new(pro_state_ref s,
    pro_ref value, pro_ref_list next)
{
    pro_alloc* alloc;
    pro_get_alloc(s, &alloc);
    pro_ref_list t = alloc(0, sizeof(*t));
    if (!t) return 0;
    
    t->value = value;
    t->next = next;
    return t;
}


PRO_INTERNAL
void pro_lookup_list_append(pro_state_ref s,
    pro_ref_list t, pro_ref value)
{
    pro_ref_list parent = t;
    while (parent->next)
        parent = parent->next;
    parent->next = pro_lookup_list_new(s, value, 0);
}


PRO_INTERNAL
void pro_lookup_list_free(pro_state_ref s, pro_ref_list t)
{
    pro_alloc* alloc;
    pro_get_alloc(s, &alloc);
    
    for (pro_ref_list item = t; item;)
    {
        pro_ref_list next = item->next;

        // Release the value
        pro_release(s, item->value);
        
        // Free the item structure
        alloc(item, 0);
        
        item = next;
    }
}
