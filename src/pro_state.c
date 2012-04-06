#include "pro_state.h"

#include "prosopon.h"
#include "pro_type.h"
#include "pro_common.h"

#include <stdlib.h>
#include <assert.h>

#pragma mark Private


#pragma mark -
#pragma mark Intenal

pro_env_stack* pro_env_stack_new(pro_state_ref state, pro_env_ref value, pro_env_stack* next)
{
    pro_env_stack* t = malloc(sizeof(*t));
    if (!t)
        return 0;
    t->value = value;
    t->next = next;
    return t;
}


#pragma mark -
#pragma mark PRO_API


PRO_API pro_error pro_state_create(PRO_OUT pro_state_ref* out_state)
{
    pro_state_ref s = malloc(sizeof(*s));
    PRO_API_ASSERT(s, PRO_OUT_OF_MEMORY);
        
    pro_env* root_env = pro_env_new(s, 0);
    PRO_API_ASSERT(root_env, PRO_OUT_OF_MEMORY);
    pro_env_stack* stack = pro_env_stack_new(s, pro_env_lookup_new(s, root_env), 0);
    PRO_API_ASSERT(stack, PRO_OUT_OF_MEMORY);

    s->root_env = root_env;
    s->stack = stack;
    s->libraries = 0;
    
    initialize_default_actor_types(s);
    
    *out_state = s;
    return PRO_OK;
}

PRO_API pro_error pro_state_release(pro_state_ref s)
{
    while (s->stack) // relase all environments 
        pro_pop_env(s); 
    
    free(s); // free state memory
    return PRO_OK;
}


PRO_API pro_error pro_get_env(pro_state_ref s, PRO_OUT pro_env_ref* out_env)
{
    PRO_API_ASSERT(s, PRO_INVALID_OPERATION);
    *out_env = pro_env_lookup_new(s, s->stack->value->value);
    return PRO_OK;
}


PRO_API pro_error pro_push_env(pro_state_ref s, pro_env_ref env)
{
    PRO_API_ASSERT(s, PRO_INVALID_OPERATION);
    PRO_API_ASSERT(PRO_EMPTY_ENV_REF != env, PRO_INVALID_ARGUMENT);
    pro_env_ref current_env = s->stack->value;
    PRO_API_ASSERT(env != current_env, PRO_INVALID_OPERATION);
    s->stack = pro_env_stack_new(s, env, s->stack);
    return PRO_OK;
}


PRO_API pro_error pro_pop_env(pro_state_ref s)
{
    PRO_API_ASSERT(s, PRO_INVALID_OPERATION);
    s->stack = s->stack->next;
    return PRO_OK;
}
