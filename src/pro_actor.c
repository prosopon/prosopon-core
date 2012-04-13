#include "prosopon.h"

#include "pro_lookup.h"
#include "pro_env.h"
#include "pro_common.h"
#include "pro_message_queue.h"
#include "pro_state.h"

#include <stdlib.h>


#pragma mark -
#pragma mark PRO_API


PRO_API pro_error pro_actor_create(pro_state_ref s, pro_actor_type type,
    pro_behavior beh, pro_ref data, PRO_OUT pro_ref* out_ref)
{
    PRO_API_ASSERT(s, PRO_INVALID_OPERATION);

    pro_env_ref current_env;
    pro_get_env(s, &current_env);
    
    pro_ref lookup = pro_env_next_lookup(s, current_env);
    pro_object** obj = pro_env_lookup_value(s, lookup);
    *obj = pro_object_new(s, PRO_ACTOR_TYPE);
    (*obj)->value.actor.type = type;
    pro_env_create(s, current_env, &((*obj)->value.actor.env));
    
    (*obj)->value.actor.behavior = beh;
    (*obj)->value.actor.data = data;
    
    *out_ref = lookup;
    return PRO_OK;
}


PRO_API pro_error pro_get_actor_type(pro_state_ref s, pro_ref actor,
    PRO_OUT pro_actor_type* out_type)
{
    PRO_API_ASSERT(s, PRO_INVALID_OPERATION);
    PRO_API_ASSERT_TYPE(actor, PRO_ACTOR_TYPE, PRO_INVALID_ARGUMENT);

    pro_object* obj = pro_dereference(s, actor);
    *out_type = obj->value.actor.type;
    return PRO_OK;
}


PRO_API pro_error pro_send(pro_state_ref s, pro_ref actor, pro_ref msg)
{
    PRO_API_ASSERT(s, PRO_INVALID_OPERATION);
    PRO_API_ASSERT_TYPE(actor, PRO_ACTOR_TYPE, PRO_INVALID_ARGUMENT);
    PRO_API_ASSERT_TYPE(msg, PRO_MESSAGE_TYPE, PRO_INVALID_ARGUMENT);

    pro_message_queue_enqueue(s, s->global->message_queue, msg, actor);
    return PRO_OK;
}


PRO_API pro_error pro_become(pro_state_ref s,
    pro_ref actor, pro_ref new_beh)
{
    PRO_API_ASSERT(s, PRO_INVALID_OPERATION);
    PRO_API_ASSERT_TYPE(actor, PRO_ACTOR_TYPE, PRO_INVALID_ARGUMENT);
    PRO_API_ASSERT_TYPE(new_beh, PRO_ACTOR_TYPE, PRO_INVALID_ARGUMENT);

    //actor->env = new_beh->env;
    //actor->index = new_beh->index;
    pro_object** current_obj = pro_env_lookup_value(s, actor);
    pro_object* new_actor_obj = pro_dereference(s, new_beh);
    *current_obj = new_actor_obj;
    
    return PRO_OK;
}


PRO_API pro_error pro_actor_request_ud(pro_state_ref s,
    pro_ref actor, PRO_OUT pro_ref* ud)
{
   // PRO_API_ASSERT(s, PRO_INVALID_OPERATION);
   // PRO_API_ASSERT_TYPE(actor, PRO_ACTOR_TYPE, PRO_INVALID_ARGUMENT);
    
    pro_object* obj = pro_dereference(s, actor);
    *ud = obj->value.actor.data;
    return PRO_OK;
}

