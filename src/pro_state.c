#include "pro_state.h"

#include "prosopon.h"
#include "pro_type.h"

#include <stdlib.h>
#include <assert.h>

#pragma mark Private

#pragma mark -
#pragma mark Intenal


#pragma mark -
#pragma mark PRO_API


PRO_API pro_state* pro_state_create(void)
{
    pro_state* s = malloc(sizeof(*s));
    if (s == 0)
        return 0;
    
    s->root_env = s->current_env = pro_env_new(s, 0, 0);
    s->libraries = 0;
    
    initialize_default_actor_types(s);
    return s;
}

PRO_API void pro_state_release(pro_state* s)
{
    while (s->current_env) // relase all environments 
        pro_pop_env(s); 
    
    free(s); // free state memory
}


PRO_API pro_env_ref pro_get_env(pro_state* s)
{
    return s->current_env;
}


PRO_API void pro_push_env(pro_state* s, pro_env_ref env)
{
    pro_env* current_env = s->current_env;
    assert(env);
    assert(env != current_env);
    env->previous = current_env;
    s->current_env = env;
}


PRO_API void pro_pop_env(pro_state* s)
{
    s->current_env = s->current_env->previous;
}
