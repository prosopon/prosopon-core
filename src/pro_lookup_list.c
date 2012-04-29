#include "pro_lookup_list.h"


typedef struct pro_lookup_list_node pro_lookup_list_node;
struct pro_lookup_list_node
{
    pro_lookup_list_node* next;
    pro_ref value;    
};


struct pro_lookup_list
{
    unsigned int size;
    pro_lookup_list_node *front, *back;
};


static pro_lookup_list_node* pro_lookup_list_node_new(pro_state_ref s,
    pro_ref value, pro_lookup_list_node* next)
{
    pro_alloc* alloc;
    pro_get_alloc(s, &alloc);
    pro_lookup_list_node* t = alloc(0, sizeof(*t));
    if (!t) return 0;
    
    t->value = value;
    t->next = next;
    return t;
}


#pragma mark -
#pragma mark Internal 


PRO_INTERNAL
pro_ref_list pro_lookup_list_new(pro_state_ref s)
{
    pro_alloc* alloc;
    pro_get_alloc(s, &alloc);
    pro_ref_list t = alloc(0, sizeof(*t));
    if (!t) return 0;
    
    t->size = 0;
    t->front = t->back = 0;
    return t;
}

PRO_INTERNAL
void pro_lookup_list_free(pro_state_ref s, pro_ref_list t)
{
    if (!t) return;
    
    pro_alloc* alloc;
    pro_get_alloc(s, &alloc);
    
    for (pro_lookup_list_node* item = t->front; item;)
    {
        pro_lookup_list_node* next = item->next;

        // Release the value
        pro_release(s, item->value);
        
        // Free the item structure
        alloc(item, 0);
        
        item = next;
    }
    
    alloc(t, 0);
}



PRO_INTERNAL
void pro_lookup_list_append(pro_state_ref s,
    pro_ref_list t, pro_ref value)
{
    pro_lookup_list_node* node = pro_lookup_list_node_new(s, value, 0);
    if (!t->back)
        t->front = t->back = node;
    else
    {
        t->back->next = node;
        t->back = node;
    }
    
    t->size++;
}


PRO_INTERNAL
unsigned int pro_lookup_list_length(pro_state_ref s, const pro_ref_list t)
{
    return t->size;
}

PRO_INTERNAL
pro_ref pro_lookup_list_get(pro_state_ref s, const pro_ref_list t, unsigned int idx)
{
    if (idx >= t->size)
        return PRO_EMPTY_REF;
        
    pro_lookup_list_node* list = t->front;
    while (idx)
    {
        if (!list)
            break;
        idx--;
        list = list->next;
    }
        
    return list->value;
}


PRO_INTERNAL
pro_ref_list pro_lookup_list_copy(pro_state_ref s, const pro_ref_list cpy)
{
    if (cpy == 0)
        return 0;
    
    unsigned int len = pro_lookup_list_length(s, cpy);
    if (0 == len)
        return 0;
    
    pro_ref_list new_list = pro_lookup_list_new(s);
    for (unsigned int i = 0; i < len; ++i)
    {
        pro_ref val = pro_lookup_list_get(s, cpy, i);
        pro_retain(s, val);
        pro_lookup_list_append(s, new_list, val);
    }
    
    return new_list;
}

