#include "pro_actor.h"

#include "prosopon.h"

#include "pro_lookup.h"
#include "pro_env.h"

#include <stdlib.h>
#include <assert.h>


#pragma mark -
#pragma mark PRO_API


PRO_API pro_lookup* pro_actor_create(pro_state* s, pro_actor_type type)
{
    pro_lookup* lookup = pro_env_next_lookup(s, pro_get_env(s));
    pro_object** obj = pro_env_lookup_value(s, lookup);
    *obj = pro_object_new(s, PRO_ACTOR_TYPE);
    (*obj)->value.actor.type = type;
    return lookup;
}


PRO_API pro_actor_type pro_get_actor_type(pro_state* s, const pro_lookup* actor)
{
    assert(pro_get_type(s, actor) == PRO_ACTOR_TYPE);

    pro_object** obj = pro_env_lookup_value(s, actor);
    return (*obj)->value.actor.type;
}


PRO_API void pro_send(pro_state* s, const pro_lookup* actor, const pro_lookup* msg)
{
    assert(pro_get_type(s, actor) == PRO_ACTOR_TYPE);
    assert(pro_get_type(s, msg) == PRO_MESSAGE_TYPE);

    pro_object* obj = *pro_env_lookup_value(s, actor);
    const pro_behavior* behavior = &obj->value.actor.behavior;  
    behavior->impl(s, actor, msg, behavior->data);
}


PRO_API void pro_become(pro_state* s,
    pro_lookup* actor, pro_behavior new_beh)
{
    assert(pro_get_type(s, actor) == PRO_ACTOR_TYPE);
    
    pro_object* obj = *pro_env_lookup_value(s, actor);
    obj->value.actor.behavior = new_beh;
}
