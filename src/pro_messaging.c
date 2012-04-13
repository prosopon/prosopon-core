#include "pro_messaging.h"

#include "pro_object.h"
#include "pro_env.h"


PRO_INTERNAL void pro_deliver_message(pro_state_ref s,
    pro_ref actor, pro_ref message)
{
    pro_object* actor_obj = pro_dereference(s, actor);
    pro_behavior* behavior = actor_obj->value.actor.behavior;
    if (behavior)
    {
        // setup environment
        pro_push_env(s, actor_obj->value.actor.env);
        
        // call the behavior
        behavior(s, actor, message, actor_obj->value.actor.data);
        
        // teardown the environment
        pro_pop_env(s);
    }
}

