#include "pro_actor.h"

#include "prosopon.h"

#include "pro_lookup.h"
#include "pro_env.h"

#include <stdlib.h>



#pragma mark -
#pragma mark PRO_API


PRO_API pro_lookup* pro_actor_create(pro_state* s)
{
    pro_lookup* lookup = pro_env_next_lookup(s, pro_get_env(s));
    pro_object** obj = pro_env_lookup_value(s, lookup);
    *obj = pro_object_new(s, PRO_ACTOR_TYPE);
    return lookup;
}


PRO_API void pro_send(pro_state* s, pro_lookup* actor, pro_lookup* msg)
{
    pro_object* obj = *pro_env_lookup_value(s, actor);
    pro_behavior* behavior = obj->value.actor.behavior;  
    void* data = obj->value.actor.data;
    behavior(s, actor, msg, data);
}


PRO_API void pro_become(pro_state* s,
    pro_lookup* actor, pro_behavior* new_beh, void* data)
{
    pro_object* obj = *pro_env_lookup_value(s, actor);
    obj->value.actor.behavior = new_beh;
    obj->value.actor.data = data;
}
