#include "prosopon/core.h"

#include "pro_env.h"
#include "pro_common.h"
#include "pro_object.h"
#include "pro_lookup.h"
#include "pro_lookup_list.h"


#pragma mark Private

#pragma mark -
#pragma mark Internal


#pragma mark -
#pragma mark Public


PRO_API
pro_error pro_list_create(pro_state_ref s, PRO_OUT pro_ref* msg)
{
    PRO_API_ASSERT_STATE(s);
    
    pro_object* obj = pro_object_new(s, PRO_LIST_TYPE, 1);
    PRO_API_ASSERT(obj, PRO_OUT_OF_MEMORY);
    obj->value.message = 0;
    
    pro_ref ref = pro_lookup_new(s, obj, 1);
    *msg = ref;
    return PRO_OK;
}


PRO_API
pro_error pro_list_length(pro_state_ref s, pro_ref ref,
    PRO_OUT unsigned int* length)
{
    PRO_API_ASSERT_STATE(s);
    PRO_API_ASSERT_TYPE(ref, PRO_LIST_TYPE, PRO_INVALID_OPERATION);
    
    pro_object* obj = pro_dereference(s, ref);
    unsigned int l = pro_lookup_list_length(s, obj->value.message);
    *length = l;
    return PRO_OK;
}


PRO_API
pro_error pro_list_get(pro_state_ref s,
    pro_ref msg, unsigned int idx, PRO_OUT pro_ref* result)
{
    PRO_API_ASSERT_STATE(s);
    PRO_API_ASSERT_TYPE(msg, PRO_LIST_TYPE, PRO_INVALID_OPERATION);

    pro_object* obj = pro_dereference(s, msg);
    pro_ref val = pro_lookup_list_get(s, obj->value.message, idx);

    pro_retain(s, val);
    *result = val;
    return PRO_OK;
}



PRO_API
pro_error pro_list_append(pro_state_ref s,
    pro_ref msg, pro_ref ref, PRO_OUT pro_ref* new_msg_out)
{
    PRO_API_ASSERT_STATE(s);
    PRO_API_ASSERT_TYPE(msg, PRO_LIST_TYPE, PRO_INVALID_OPERATION);
    //PRO_API_ASSERT(msg != *new_msg_out, );
    
    // retain the appended object
    pro_retain(s, ref);
    
    // create the new list
    pro_ref new_msg;
    pro_list_create(s, &new_msg);
    
    // build the new list
    pro_object* obj = pro_dereference(s, msg);
    pro_object* new_obj = pro_dereference(s, new_msg);
    new_obj->value.message = pro_lookup_list_copy(s, obj->value.message);
    if (!new_obj->value.message)
        new_obj->value.message = pro_lookup_list_new(s);
    pro_lookup_list_append(s, new_obj->value.message, ref);
        
    *new_msg_out = new_msg;
    return PRO_OK;
}


