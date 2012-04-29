#include "pro_env_stack.h"


#pragma mark Private

typedef struct pro_env_stack_node pro_env_stack_node;

struct pro_env_stack_node
{
    pro_env_stack_node* next;
    pro_env_ref value;
};


/**
 * Structure for a fifo stack of environments.
 */
struct pro_env_stack
{
    struct pro_env_stack_node* top;
};


static pro_env_stack_node* pro_env_stack_node_new(pro_state_ref s,
    pro_env_ref value, pro_env_stack_node* next)
{
    pro_alloc* alloc;
    pro_get_alloc(s, &alloc);
    pro_env_stack_node* t = alloc(0, sizeof(*t));
    if (!t) return 0;
    
    t->value = value;
    t->next = next;
    return t;
}


#pragma mark -
#pragma mark Internal 

PRO_INTERNAL
pro_env_stack* pro_env_stack_new(pro_state_ref s)
{
    pro_alloc* alloc;
    pro_get_alloc(s, &alloc);
    pro_env_stack* t = alloc(0, sizeof(*t));
    if (!t) return 0;
    
    t->top = 0;
    return t;
}


PRO_INTERNAL
void pro_env_stack_free(pro_state_ref s, pro_env_stack* t)
{
    pro_alloc* alloc;
    pro_get_alloc(s, &alloc);

    for (pro_env_stack_node* item = t->top; item;)
    {
        pro_env_stack_node* next = item->next;
        alloc(item, 0);
        item = next;
    }
    
    alloc(t, 0);
}


PRO_INTERNAL
void pro_env_stack_push(pro_state_ref s,
    pro_env_stack* t,  pro_env_ref v)
{
    t->top = pro_env_stack_node_new(s, v, t->top);
}


PRO_INTERNAL
void pro_env_stack_pop(pro_state_ref s, pro_env_stack* t)
{
    pro_env_stack_node* old = t->top;
    t->top = t->top->next;
    
    // free old top node
    pro_alloc* alloc;
    pro_get_alloc(s, &alloc);
    alloc(old, 0);
}


PRO_INTERNAL
pro_env_ref pro_env_stack_top(pro_state_ref s, pro_env_stack* t)
{
    return t && t->top ? t->top->value : 0;
}


