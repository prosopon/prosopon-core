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
        pro_env_release(s); 
    
    free(s); // free state memory
}