#include "pro_constructor.h"

#include "prosopon.h"
#include "pro_env.h"


PRO_API pro_lookup* pro_constructor_create(pro_state* s, pro_constructor* c, void* data)
{
    pro_lookup* lookup= pro_env_next_lookup(s, pro_get_env(s));
    pro_object** obj = pro_env_lookup_value(s, lookup);
    *obj = pro_object_new(s, PRO_CONSTRUCTOR_TYPE);
    (*obj)->value.constructor.constructor = c;
    (*obj)->value.constructor.data = data;
    return lookup;
}


PRO_API pro_lookup* pro_constructor_call(pro_state* s, pro_lookup* constructor, pro_lookup_list* arguments)
{
    return 0;
}