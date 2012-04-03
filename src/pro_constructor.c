#include "pro_constructor.h"

#include "prosopon.h"
#include "pro_env.h"


PRO_API pro_error pro_constructor_create(pro_state_ref s,
    pro_constructor c, PRO_OUT pro_ref* constructor)
{
    pro_env_ref env;
    pro_get_env(s, &env);
    pro_ref lookup = pro_env_next_lookup(s, env);
    pro_object** obj = pro_env_lookup_value(s, lookup);
    *obj = pro_object_new(s, PRO_CONSTRUCTOR_TYPE);
    (*obj)->value.constructor.constructor = c;
    *constructor = lookup;
    return PRO_OK;
}


PRO_API pro_error pro_constructor_call(pro_state_ref s,
    pro_ref constructor, pro_ref_list arguments, PRO_OUT pro_ref* result)
{
    if (!s) return PRO_INVALID_OPERATION;
    if (PRO_EMPTY_REF == constructor) return PRO_INVALID_ARGUMENT;
    
    pro_object* obj = *pro_env_lookup_value(s, constructor);
    pro_constructor* c = &obj->value.constructor.constructor;
    pro_ref out = c->impl(s, arguments, c->data);
    if (PRO_EMPTY_REF == out) return PRO_CONSTRUCTOR_ERROR;
    *result = out;
    return PRO_OK;
}