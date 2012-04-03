#include "pro_constructor.h"

#include "prosopon.h"
#include "pro_env.h"


PRO_API pro_ref pro_constructor_create(pro_state_ref s,
    pro_constructor c)
{
    pro_env_ref env;
    pro_get_env(s, &env);
    pro_ref lookup = pro_env_next_lookup(s, env);
    pro_object** obj = pro_env_lookup_value(s, lookup);
    *obj = pro_object_new(s, PRO_CONSTRUCTOR_TYPE);
    (*obj)->value.constructor.constructor = c;
    return lookup;
}


PRO_API pro_ref pro_constructor_call(pro_state_ref s,
    pro_ref constructor, pro_ref_list arguments)
{
    pro_object* obj = *pro_env_lookup_value(s, constructor);
    pro_constructor* c = &obj->value.constructor.constructor;
    return c->impl(s, arguments, c->data);
}