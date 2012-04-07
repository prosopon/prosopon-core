#include "pro_env_stack.h"

#include <stdlib.h>


#pragma mark Private

typedef struct pro_env_stack_node pro_env_stack_node;

struct pro_env_stack_node
{
    pro_env_stack_node* next;
    pro_env_ref value;
};


static pro_env_stack_node* pro_env_stack_node_new(pro_state_ref s,
    pro_env_ref value, pro_env_stack_node* next)
{
    pro_env_stack_node* t = malloc(sizeof(*t));
    if (!t)
        return 0;
    t->value = value;
    t->next = next;
    return t;
}


#pragma mark -
#pragma mark Internal 

PRO_INTERNAL pro_env_stack* pro_env_stack_new(pro_state_ref s)
{
    pro_env_stack* t = malloc(sizeof(*t));
    if (!t) return 0;
    t->top = 0;
    return t;
}


PRO_INTERNAL void pro_env_stack_push(pro_state_ref s,
    pro_env_stack* t,  pro_env_ref v)
{
    t->top = pro_env_stack_node_new(s, v, t->top);
}


PRO_INTERNAL void pro_env_stack_pop(pro_state_ref s, pro_env_stack* t)
{
    pro_env_stack_node* old = t->top;
    t->top = t->top->next;
    free(old);
}


PRO_INTERNAL pro_env_ref pro_env_stack_top(pro_state_ref s, pro_env_stack* t)
{
    if (!t || !t->top)
        return 0;
    return t->top->value;
}


