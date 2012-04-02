#include "pro_object.h"

#include "pro_type.h"
#include "pro_env.h"
#include "pro_object.h"

#include <stdlib.h>
#include <assert.h>


PRO_INTERNAL pro_object* pro_object_new(pro_state* s, pro_type type)
{
    pro_object* t = malloc(sizeof(*t));
    t->type = type;
    return t;
}



PRO_API int pro_match(pro_state* s, const pro_lookup* l1, const pro_lookup* l2)
{
    pro_actor_type type = pro_get_actor_type(s, l1);
    const pro_actor_type_info* info = pro_get_actor_type_info(s, type);
    pro_object** o1 = pro_env_lookup_value(s, l1);
    pro_object** o2 = pro_env_lookup_value(s, l2);
    return info->match(s,
        l1, (*o1)->value.actor.behavior.data,
        l2, (*o2)->value.actor.behavior.data);
}


PRO_API const char* pro_to_string(pro_state* s,
    const pro_lookup* t)
{
    return "test";
}