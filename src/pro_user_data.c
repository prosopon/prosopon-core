#include "prosopon/core.h"

#include "pro_common.h"
#include "pro_object.h"
#include "pro_common.h"
#include "pro_env.h"
#include "pro_lookup.h"


#pragma mark Private


#pragma mark -
#pragma mark Public

PRO_API void PRO_DEFAULT_UD_DECONSTRUCTOR(pro_state_ref s, void* data)
{
    pro_alloc* alloc;
    pro_get_alloc(s, &alloc);
    alloc(data, 0);
}

PRO_API
pro_error pro_ud_create(pro_state_ref s,
    size_t size, pro_ud_deconstructor* deconstructor, PRO_OUT pro_ref* t)
{
    PRO_API_ASSERT(s, PRO_INVALID_STATE);

    pro_alloc* alloc;
    pro_get_alloc(s, &alloc);
    
    pro_object* obj = obj = pro_object_new(s, PRO_UD_TYPE, 1);
    obj->value.ud.size = size;
    obj->value.ud.data = size > 0 ? alloc(0, size) : 0;
    obj->value.ud.deconstructor = deconstructor;
    
    pro_ref ref = pro_lookup_new(s, obj, 1);
    *t = ref;
    return PRO_OK;
}


PRO_API
pro_error pro_ud_read(pro_state_ref s, pro_ref t,
    PRO_OUT const void** ptr)
{
    PRO_API_ASSERT(s, PRO_INVALID_STATE);
    PRO_API_ASSERT_TYPE(t, PRO_UD_TYPE, PRO_INVALID_OPERATION);
    
    pro_object* obj = pro_dereference(s, t);
    *ptr = obj->value.ud.data;
    return PRO_OK;
}


PRO_API
pro_error pro_ud_write(pro_state_ref s, pro_ref t, PRO_OUT void** ptr)
{
    PRO_API_ASSERT(s, PRO_INVALID_STATE);
    PRO_API_ASSERT_TYPE(t, PRO_UD_TYPE, PRO_INVALID_OPERATION);

    pro_object* obj = pro_dereference(s, t);
    *ptr = obj->value.ud.data;
    return PRO_OK;
}
