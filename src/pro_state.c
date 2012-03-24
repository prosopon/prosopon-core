#include "pro_state.h"

#include "prosopon.h"

#include <stdlib.h>



PRO_API pro_state* pro_state_create(void)
{
    pro_state* s = malloc(sizeof(*s));
    if (s == 0)
        return 0;
    
    s->root_env = s->current_env = pro_env_new(s, 0, 0);
    return s;
}

PRO_API void pro_state_release(pro_state* s)
{
    while (s->current_env) // relase all environments 
        pro_pop_env(s); 
    
    free(s); // free state memory
}


PRO_API pro_env_lookup* pro_get_env(pro_state* s)
{
    return s->current_env;
}


PRO_API void pro_push_env(pro_state* s, pro_env_lookup* env)
{
    pro_env* current_env = s->current_env;
    env->previous = current_env;
    s->current_env = env;
}


PRO_API void pro_pop_env(pro_state* s)
{
    s->current_env = s->current_env->previous;
}
