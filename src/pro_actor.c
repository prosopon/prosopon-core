#include "pro_actor.h"

#include "prosopon.h"

#include "pro_lookup.h"
#include "pro_env.h"
#include "pro_common.h"

#include <stdlib.h>
#include <assert.h>


#pragma mark -
#pragma mark PRO_API


PRO_API pro_error pro_actor_create(pro_state_ref s, pro_actor_type type,
    PRO_OUT pro_ref* out_ref)
{
    PRO_API_ASSERT(s, PRO_INVALID_OPERATION);

    pro_env_ref current_env;
    pro_get_env(s, &current_env);
    
    pro_ref lookup = pro_env_next_lookup(s, current_env);
    pro_object** obj = pro_env_lookup_value(s, lookup);
    *obj = pro_object_new(s, PRO_ACTOR_TYPE);
    (*obj)->value.actor.type = type;
    pro_env_create(s, current_env, &((*obj)->value.actor.env));
    *out_ref = lookup;
    return PRO_OK;
}


PRO_API pro_error pro_get_actor_type(pro_state_ref s, pro_ref actor,
    PRO_OUT pro_actor_type* out_type)
{
    PRO_API_ASSERT(s, PRO_INVALID_OPERATION);
    PRO_API_ASSERT_TYPE(actor, PRO_ACTOR_TYPE, PRO_INVALID_ARGUMENT);

    pro_object** obj = pro_env_lookup_value(s, actor);
    *out_type = (*obj)->value.actor.type;
    return PRO_OK;
}


PRO_API pro_error pro_send(pro_state_ref s, pro_ref actor, pro_ref msg)
{
    PRO_API_ASSERT(s, PRO_INVALID_OPERATION);
    PRO_API_ASSERT_TYPE(actor, PRO_ACTOR_TYPE, PRO_INVALID_ARGUMENT);
    PRO_API_ASSERT_TYPE(msg, PRO_MESSAGE_TYPE, PRO_INVALID_ARGUMENT);

    pro_object* obj = *pro_env_lookup_value(s, actor);
    pro_push_env(s, obj->value.actor.env);
    const pro_behavior* behavior = &obj->value.actor.behavior;  
    behavior->impl(s, actor, msg, behavior->data);
    pro_pop_env(s);
    return PRO_OK;
}


PRO_API pro_error pro_become(pro_state_ref s,
    pro_ref actor, pro_behavior new_beh)
{
    PRO_API_ASSERT(s, PRO_INVALID_OPERATION);
    PRO_API_ASSERT_TYPE(actor, PRO_ACTOR_TYPE, PRO_INVALID_ARGUMENT);
    
    pro_object* obj = *pro_env_lookup_value(s, actor);
    obj->value.actor.behavior = new_beh;
    return PRO_OK;
}


PRO_API const void* pro_request_actor_data(pro_state_ref s,
    pro_ref actor)
{
    pro_type actor_type;
    pro_get_type(s, actor, &actor_type);
    assert(PRO_ACTOR_TYPE == actor_type);
    
    pro_object* obj = *pro_env_lookup_value(s, actor);
    return obj->value.actor.behavior.data;
}