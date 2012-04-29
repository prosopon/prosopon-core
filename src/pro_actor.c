#include "prosopon/prosopon.h"

#include "pro_lookup.h"
#include "pro_env.h"
#include "pro_common.h"
#include "pro_message_queue.h"
#include "pro_state.h"
#include "pro_object.h"


#pragma mark -
#pragma mark PRO_API


PRO_API
pro_error pro_actor_create(pro_state_ref s, pro_actor_type type,
    pro_behavior beh, pro_ref data, PRO_OUT pro_ref* out_ref)
{
    PRO_API_ASSERT_STATE(s);
    
    // Get the current environment
    pro_env_ref current_env;
    pro_get_env(s, &current_env);
    
    // Create a new object
    pro_object* obj = pro_object_new(s, PRO_ACTOR_TYPE, 1);
    
    // Set actor values
    obj->value.actor.type = type;    
    
    obj->value.actor.behavior = beh;
    pro_retain(s, data);
    obj->value.actor.data = data;

    // Create a new env for this actor with current_env as parent
    pro_env_create(s, current_env, &(obj->value.actor.env));
    
    // Release current env
    pro_env_release(s, current_env);

    pro_ref lookup = pro_lookup_new(s, obj, 1);
    *out_ref = lookup;
    return PRO_OK;
}


PRO_API
pro_error pro_get_actor_type(pro_state_ref s, pro_ref actor,
    PRO_OUT pro_actor_type* out_type)
{
    PRO_API_ASSERT_STATE(s);
    PRO_API_ASSERT_TYPE(actor, PRO_ACTOR_TYPE, PRO_INVALID_OPERATION);

    pro_object* obj = pro_dereference(s, actor);
    *out_type = obj->value.actor.type;
    return PRO_OK;
}


PRO_API
pro_error pro_send(pro_state_ref s, pro_ref actor, pro_ref msg)
{
    PRO_API_ASSERT_STATE(s);
    
    if (pro_lookup_equal(s, msg, PRO_EMPTY_REF))
        return PRO_OK; // TODO, is this the expected behavior?
    
    PRO_API_ASSERT_TYPE(msg, PRO_LIST_TYPE, PRO_INVALID_OPERATION);
    
    if (pro_lookup_equal(s, actor, PRO_EMPTY_REF))
        return PRO_OK; // sends to PRO_EMPTY_REF act as a sink
        
    PRO_API_ASSERT_TYPE(actor, PRO_ACTOR_TYPE, PRO_INVALID_OPERATION);
    pro_message_queue_enqueue(s, s->global->message_queue, msg, actor);

    return PRO_OK;
}


PRO_API
pro_error pro_become(pro_state_ref s,
    pro_ref actor, pro_ref new_beh)
{
    PRO_API_ASSERT_STATE(s);
    PRO_API_ASSERT_TYPE(actor, PRO_ACTOR_TYPE, PRO_INVALID_OPERATION);
    PRO_API_ASSERT_TYPE(new_beh, PRO_ACTOR_TYPE, PRO_INVALID_OPERATION);

    pro_object* current_obj = pro_dereference(s, actor);
    pro_object_release(s, current_obj);
    
    pro_object* new_actor_obj = pro_dereference(s, new_beh);
    actor->obj = pro_object_retain(s, new_actor_obj);
    
    return PRO_OK;
}


PRO_API
pro_error pro_actor_request_ud(pro_state_ref s,
    pro_ref actor, PRO_OUT pro_ref* ud)
{
    PRO_API_ASSERT_STATE(s);
    PRO_API_ASSERT_TYPE(actor, PRO_ACTOR_TYPE, PRO_INVALID_OPERATION);
    
    pro_object* obj = pro_dereference(s, actor);
    *ud = obj->value.actor.data;
    return PRO_OK;
}

