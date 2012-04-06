#include "pro_message.h"

#include "prosopon.h"
#include "pro_env.h"
#include "pro_common.h"

#include <assert.h>


#pragma mark Private

#pragma mark -
#pragma mark Internal


#pragma mark -
#pragma mark Public


PRO_API pro_error pro_message_create(pro_state_ref s, PRO_OUT pro_ref* msg)
{
    PRO_API_ASSERT(s, PRO_INVALID_OPERATION);
    pro_env_ref env;
    pro_get_env(s, &env);
    pro_ref ref = pro_env_next_lookup(s, env);
    pro_object** obj = pro_env_lookup_value(s, ref);
    *obj = pro_object_new(s, PRO_MESSAGE_TYPE);
    if (0 == *obj) return PRO_OUT_OF_MEMORY;
    (*obj)->value.message = 0;
    *msg = ref;
    return PRO_OK;
}


PRO_API pro_error pro_message_length(pro_state_ref s, pro_ref ref,
    PRO_OUT unsigned int* length)
{
    PRO_API_ASSERT(s, PRO_INVALID_OPERATION);
    PRO_API_ASSERT(PRO_EMPTY_REF != ref, PRO_INVALID_ARGUMENT);
    
    pro_object** obj = pro_env_lookup_value(s, ref);
    unsigned int l = 0;
    for (pro_ref_list msg = (*obj)->value.message; msg; msg = msg->next)
        ++l;
    *length = l;
    return PRO_OK;
}


PRO_API pro_error pro_message_get(pro_state_ref s,
    pro_ref msg, unsigned int idx, PRO_OUT pro_ref* result)
{
    PRO_API_ASSERT(s, PRO_INVALID_OPERATION);

    pro_type type;
    pro_get_type(s, msg, &type);
    PRO_API_ASSERT(PRO_MESSAGE_TYPE == type, PRO_INVALID_ARGUMENT);

    pro_object** obj = pro_env_lookup_value(s, msg);
    pro_ref_list list = (*obj)->value.message;
    while (idx)
    {
        if (!list)
            break;
        idx--;
        list = list->next;
    }
    *result = list ? list->value : PRO_EMPTY_REF;
    return PRO_OK;
}


PRO_API pro_error pro_message_append(pro_state_ref s,
    pro_ref msg, pro_ref ref)
{
    PRO_API_ASSERT(s, PRO_INVALID_OPERATION);

    pro_type type;
    pro_get_type(s, msg, &type);
    PRO_API_ASSERT(PRO_MESSAGE_TYPE == type, PRO_INVALID_ARGUMENT);
    PRO_API_ASSERT(ref, PRO_INVALID_OPERATION);
    
    pro_object** obj = pro_env_lookup_value(s, msg);
    if (!(*obj)->value.message)
        (*obj)->value.message = pro_lookup_list_new(s, ref, 0);
    else
        pro_lookup_list_append(s, (*obj)->value.message, ref);
    return PRO_OK;
}

