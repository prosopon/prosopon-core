#include "pro_state.h"

#include "prosopon.h"
#include "pro_type.h"
#include "pro_common.h"
#include "pro_messaging.h"
#include "pro_env_stack.h"
#include "pro_message_queue.h"
#include "pro_env.h"
#include "pro_lookup.h"
#include "pro_env_lookup.h"
#include "pro_library.h"


#pragma mark Private

static pro_state* pro_state_new(pro_global_state* g)
{
    pro_state_ref s = g->alloc(0, sizeof(*s));
    if (!s) return 0;
    s->global = g;
    return s;
}

static pro_global_state* pro_global_state_new(pro_alloc* alloc)
{
    pro_global_state* g = alloc(0, sizeof(*g));
    if (!g) return 0;
    
    g->alloc = alloc;

    pro_state* s = pro_state_new(g);
    if (!s)
    {
        alloc(g, 0);
        return 0;
    }
    
    g->main = s;
    g->libraries = 0;
    g->message_queue = pro_message_queue_new(s);
    g->actor_types = 0;
    
    return g;
}


static void pro_global_state_free(pro_state_ref s, pro_global_state* g)
{
    pro_alloc* alloc = g->alloc;
    
    if (g->libraries)
        pro_library_list_free(s, g->libraries);
    if (g->actor_types)
        pro_actor_type_info_list_free(s, g->actor_types);
    if (g->message_queue)
        pro_message_queue_free(s, g->message_queue);
    
    // Free global state structure memory
    alloc(g, 0);
}


#pragma mark -
#pragma mark Internal

PRO_INTERNAL pro_state* pro_state_fork(pro_state* s)
{
    pro_state* n = pro_state_new(s->global);
    
    pro_env_ref root_env;
    pro_get_env(s, &root_env);
    
    pro_env_stack* stack = pro_env_stack_new(n);
    pro_env_stack_push(n, stack, root_env);
    
    n->root_env = root_env;
    n->stack = stack;
    
    return n;
}


PRO_INTERNAL struct pro_library_list* pro_state_get_libraries(pro_state* s)
{
    return s->global->libraries;
}

PRO_INTERNAL void pro_state_set_libraries(pro_state* s,
    struct pro_library_list* val)
{
    s->global->libraries = val;
}

PRO_INTERNAL struct pro_actor_type_info_list* pro_state_get_actor_type_info(pro_state* s)
{
    return s->global->actor_types;
}

PRO_INTERNAL void pro_state_set_actor_type_info(pro_state* s,
    struct pro_actor_type_info_list* val)
{
    s->global->actor_types = val;
}


#pragma mark -
#pragma mark Public

PRO_API pro_error pro_state_create(pro_alloc* alloc, PRO_OUT pro_state_ref* out_state)
{
    // Allocate and setup the global state
    pro_global_state* g = pro_global_state_new(alloc);
    PRO_API_ASSERT(g, PRO_OUT_OF_MEMORY);
    
    // Setup the main state
    pro_state* s = g->main;
    
    pro_env* env = pro_env_new(s, PRO_EMPTY_ENV_REF, 1);
    pro_env_ref root_env = pro_env_lookup_new(s, env, 1);
    PRO_API_ASSERT(root_env, PRO_OUT_OF_MEMORY);
    pro_env_stack* stack = pro_env_stack_new(s);
    pro_env_stack_push(s, stack, root_env);
    PRO_API_ASSERT(stack, PRO_OUT_OF_MEMORY);

    s->root_env = root_env;
    s->stack = stack;
    
    initialize_default_actor_types(s);
    
    *out_state = s;
    return PRO_OK;
}


PRO_API pro_error pro_state_release(pro_state_ref s)
{
    PRO_API_ASSERT(s, PRO_INVALID_OPERATION);
    
    pro_alloc* alloc;
    pro_get_alloc(s, &alloc);
    
    // relase all environments
    while (!pro_env_lookup_equal(s, pro_env_stack_top(s, s->stack), s->root_env))  
        pro_pop_env(s); 
    
    // Release the root environment.
    pro_env* root_env = pro_env_dereference(s, s->root_env);
    pro_env_release(s, s->root_env);
    pro_env_free(s, root_env);

    // free global state
    if (s == s->global->main)
        pro_global_state_free(s, s->global);
    
    // free state structure memory

    alloc(s, 0);
    return PRO_OK;
}


PRO_API pro_error pro_get_alloc(pro_state_ref s, PRO_OUT pro_alloc** alloc)
{
    *alloc = s->global->alloc;
    return PRO_OK;
}


PRO_API pro_error pro_run(pro_state_ref s)
{
    while (!pro_message_queue_is_empty(s, s->global->message_queue))
    {
        pro_ref actor;
        pro_ref msg = pro_message_queue_dequeue(s, s->global->message_queue, &actor);
        pro_deliver_message(s, actor, msg);
        pro_release(s, msg);
        pro_release(s, actor);
    }
    return PRO_OK;
}


PRO_API pro_error pro_get_env(pro_state_ref s, PRO_OUT pro_env_ref* out_env)
{
    PRO_API_ASSERT(s, PRO_INVALID_OPERATION);
    
    pro_env_ref env = pro_env_stack_top(s, s->stack);
    *out_env = pro_env_lookup_new(s, pro_internal_env_retain(s, env->value), 1);
    return PRO_OK;
}


PRO_API pro_error pro_push_env(pro_state_ref s, pro_env_ref env)
{
    PRO_API_ASSERT(s, PRO_INVALID_OPERATION);
    PRO_API_ASSERT(PRO_EMPTY_ENV_REF != env, PRO_INVALID_ARGUMENT);
   
    // error if pushing env onto self.
    pro_env_ref current_env = pro_env_stack_top(s, s->stack);
    PRO_API_ASSERT(!pro_env_lookup_equal(s, env, current_env), PRO_INVALID_OPERATION);
    
    // actually push env onto stack
    pro_env_retain(s, env);
    pro_env_stack_push(s, s->stack, env);
    return PRO_OK;
}


PRO_API pro_error pro_pop_env(pro_state_ref s)
{
    PRO_API_ASSERT(s, PRO_INVALID_OPERATION);
    // Dont allow popping the root.
    PRO_API_ASSERT(!(pro_env_lookup_equal(s, pro_env_stack_top(s, s->stack), s->root_env)),
        PRO_INVALID_OPERATION);
    
    pro_env_ref old = pro_env_stack_top(s, s->stack);
    pro_env_stack_pop(s, s->stack);
    
    // release previous stack entry.
    pro_env_release(s, old);
    
    return PRO_OK;
}
