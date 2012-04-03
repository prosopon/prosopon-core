#include "pro_state.h"

#include "prosopon.h"
#include "pro_type.h"

#include <stdlib.h>
#include <assert.h>

#pragma mark Private


#pragma mark -
#pragma mark Intenal

struct pro_env_stack
{
    pro_env_stack* next;
    pro_env_ref value;
};

pro_env_stack* pro_env_stack_new(pro_state_ref state, pro_env_ref value, pro_env_stack* next)
{
    pro_env_stack* t = malloc(sizeof(*t));
    t->value = value;
    t->next = next;
    return t;
}


#pragma mark -
#pragma mark PRO_API


PRO_API pro_state_ref pro_state_create(void)
{
    pro_state_ref s = malloc(sizeof(*s));
    if (s == 0)
        return 0;
    
    s->root_env = pro_env_new(s, 0);
    s->stack = pro_env_stack_new(s, pro_env_lookup_new(s, s->root_env), 0);
    s->libraries = 0;
    
    initialize_default_actor_types(s);
    return s;
}

PRO_API void pro_state_release(pro_state_ref s)
{
    while (s->stack) // relase all environments 
        pro_pop_env(s); 
    
    free(s); // free state memory
}


PRO_API pro_env_ref pro_get_env(pro_state_ref s)
{
    return pro_env_lookup_new(s, s->stack->value->value);
}


PRO_API void pro_push_env(pro_state_ref s, pro_env_ref env)
{
    pro_env_ref current_env = s->stack->value;
    assert(env);
    assert(env != current_env);
    s->stack = pro_env_stack_new(s, env, s->stack);
}


PRO_API void pro_pop_env(pro_state_ref s)
{
    s->stack = s->stack->next;
}
