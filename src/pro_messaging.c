#include "pro_messaging.h"

#include "pro_object.h"
#include "pro_env.h"


PRO_INTERNAL void pro_deliver_message(pro_state_ref s,
    pro_ref actor, pro_ref message)
{
    pro_object* actor_obj = *pro_env_lookup_value(s, actor);
    pro_push_env(s, actor_obj->value.actor.env);
    const pro_behavior* behavior = &actor_obj->value.actor.behavior;  
    behavior->impl(s, actor, message, behavior->data);
    pro_pop_env(s);
}

