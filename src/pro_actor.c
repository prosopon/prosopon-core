#include "pro_actor.h"

#include "prosopon.h"

#include "pro_lookup.h"
#include "pro_env.h"

#include <stdlib.h>
#include <assert.h>


#pragma mark -
#pragma mark PRO_API


PRO_API pro_ref pro_actor_create(pro_state_ref s, pro_actor_type type)
{
    pro_env_ref current_env;
    pro_get_env(s, &current_env);
    
    pro_ref lookup = pro_env_next_lookup(s, current_env);
    pro_object** obj = pro_env_lookup_value(s, lookup);
    *obj = pro_object_new(s, PRO_ACTOR_TYPE);
    (*obj)->value.actor.type = type;
    (*obj)->value.actor.env = pro_env_create(s, current_env);
    return lookup;
}


PRO_API pro_actor_type pro_get_actor_type(pro_state_ref s, pro_ref actor)
{
    assert(pro_get_type(s, actor) == PRO_ACTOR_TYPE);

    pro_object** obj = pro_env_lookup_value(s, actor);
    return (*obj)->value.actor.type;
}


PRO_API void pro_send(pro_state_ref s, pro_ref actor, pro_ref msg)
{
    assert(pro_get_type(s, actor) == PRO_ACTOR_TYPE);
    assert(pro_get_type(s, msg) == PRO_MESSAGE_TYPE);
    
    pro_object* obj = *pro_env_lookup_value(s, actor);
    pro_push_env(s, obj->value.actor.env);
    const pro_behavior* behavior = &obj->value.actor.behavior;  
    behavior->impl(s, actor, msg, behavior->data);
    pro_pop_env(s);
}


PRO_API void pro_become(pro_state_ref s,
    pro_ref actor, pro_behavior new_beh)
{
    assert(pro_get_type(s, actor) == PRO_ACTOR_TYPE);
    
    pro_object* obj = *pro_env_lookup_value(s, actor);
    obj->value.actor.behavior = new_beh;
}


PRO_API const void* pro_request_actor_data(pro_state_ref s,
    pro_ref actor)
{
    assert(pro_get_type(s, actor) == PRO_ACTOR_TYPE);
    pro_object* obj = *pro_env_lookup_value(s, actor);
    return obj->value.actor.behavior.data;
}