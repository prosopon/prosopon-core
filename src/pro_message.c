#include "prosopon.h"

#include "pro_env.h"
#include "pro_common.h"
#include "pro_object.h"
#include "pro_lookup.h"


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
    pro_env_release(s, env);
    
    pro_object** obj = pro_env_lookup_value(s, ref);
    *obj = pro_object_new(s, PRO_MESSAGE_TYPE, 1);
    PRO_API_ASSERT(*obj, PRO_OUT_OF_MEMORY);
    (*obj)->value.message = 0;
    
    *msg = ref;
    return PRO_OK;
}


PRO_API pro_error pro_message_length(pro_state_ref s, pro_ref ref,
    PRO_OUT unsigned int* length)
{
    PRO_API_ASSERT(s, PRO_INVALID_OPERATION);
    PRO_API_ASSERT_TYPE(ref, PRO_MESSAGE_TYPE, PRO_INVALID_ARGUMENT);
    
    pro_object* obj = pro_dereference(s, ref);
    unsigned int l = 0;
    for (pro_ref_list msg = obj->value.message; msg; msg = msg->next)
        ++l;
    *length = l;
    return PRO_OK;
}


PRO_API pro_error pro_message_get(pro_state_ref s,
    pro_ref msg, unsigned int idx, PRO_OUT pro_ref* result)
{
    PRO_API_ASSERT(s, PRO_INVALID_OPERATION);
    PRO_API_ASSERT_TYPE(msg, PRO_MESSAGE_TYPE, PRO_INVALID_ARGUMENT);

    pro_object* obj = pro_dereference(s, msg);
    pro_ref_list list = obj->value.message;
    while (idx)
    {
        if (!list)
            break;
        idx--;
        list = list->next;
    }
        
    if (list)
    {
        pro_retain(s, list->value);
        *result = list->value;
    }
    else
        *result = PRO_EMPTY_REF;
    return PRO_OK;
}



PRO_API pro_error pro_message_append(pro_state_ref s,
    pro_ref msg, pro_ref ref, PRO_OUT pro_ref* new_msg_out)
{
    PRO_API_ASSERT(s, PRO_INVALID_OPERATION);
    PRO_API_ASSERT_TYPE(msg, PRO_MESSAGE_TYPE, PRO_INVALID_ARGUMENT);
    PRO_API_ASSERT(ref, PRO_INVALID_ARGUMENT);
    PRO_API_ASSERT(msg != *new_msg_out, PRO_INVALID_ARGUMENT);
    
    // retain the appended object
    pro_retain(s, ref);
    
    // create the new message
    pro_ref new_msg;
    pro_message_create(s, &new_msg);
    
    pro_object* obj = pro_dereference(s, msg);
    pro_object* new_obj = pro_dereference(s, new_msg);

    pro_ref_list val = obj->value.message;
    if (!val)
        new_obj->value.message = pro_lookup_list_new(s, ref, 0);
    else
    {
        pro_retain(s, val->value);
        new_obj->value.message = pro_lookup_list_new(s, val->value, 0);
        
        while ((val = val->next))
        {
            pro_retain(s, val->value);
            pro_lookup_list_append(s, new_obj->value.message, val->value);
        }
        pro_lookup_list_append(s, new_obj->value.message, ref);
    }
    
    *new_msg_out = new_msg;
    return PRO_OK;
}


