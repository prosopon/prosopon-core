#include "prosopon.h"

#include "pro_env.h"
#include "pro_common.h"
#include "pro_object.h"


PRO_API pro_error pro_constructor_create(pro_state_ref s,
    pro_constructor* c, pro_ref ud, PRO_OUT pro_ref* constructor)
{
    PRO_API_ASSERT(s, PRO_INVALID_OPERATION);
    PRO_API_ASSERT(c, PRO_INVALID_ARGUMENT);

    pro_env_ref current_env;
    pro_get_env(s, &current_env);
    pro_ref lookup = pro_env_next_lookup(s, current_env);
    pro_object** obj = pro_env_lookup_value(s, lookup);
    
    *obj = pro_object_new(s, PRO_CONSTRUCTOR_TYPE, 1);
    (*obj)->value.constructor.constructor = c;
   
    pro_retain(s, ud);
    (*obj)->value.constructor.data = ud;
    
    // Create a new env for this actor with current_env as parent
    pro_env_create(s, current_env, &((*obj)->value.constructor.env));
    
    // Release current env
    pro_env_release(s, current_env);
    
    *constructor = lookup;
    return PRO_OK;
}


PRO_API pro_error pro_constructor_call(pro_state_ref s,
    pro_ref constructor, pro_ref arguments, PRO_OUT pro_ref* result)
{
    PRO_API_ASSERT(s, PRO_INVALID_OPERATION);
    PRO_API_ASSERT_TYPE(constructor, PRO_CONSTRUCTOR_TYPE, PRO_INVALID_ARGUMENT);
    
    pro_object* obj = pro_dereference(s, constructor);
    
    pro_env_ref env;
    pro_env_create(s, obj->value.constructor.env, &env);
    pro_push_env(s, env);
    pro_env_release(s, env);
    
    pro_constructor* impl = obj->value.constructor.constructor;
    pro_ref ud = obj->value.constructor.data;
    pro_ref out = impl(s, arguments, ud);
    PRO_API_ASSERT(PRO_EMPTY_REF != out, PRO_CONSTRUCTOR_ERROR);
    
    pro_pop_env(s);
    
    *result = out;
    return PRO_OK;
}