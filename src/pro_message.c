#include "pro_message.h"

#include "prosopon.h"

#include "pro_env.h"

#include <assert.h>


#pragma mark Private

#pragma mark -
#pragma mark Internal


#pragma mark -
#pragma mark Public


PRO_API pro_ref pro_message_create(pro_state*s)
{
    pro_ref lookup = pro_env_next_lookup(s, pro_get_env(s));
    pro_object** obj = pro_env_lookup_value(s, lookup);
    *obj = pro_object_new(s, PRO_MESSAGE_TYPE);
    (*obj)->value.message = 0;
    return lookup;
}


PRO_API unsigned int pro_message_length(pro_state* s, const pro_ref lookup)
{
    pro_object** obj = pro_env_lookup_value(s, lookup);
    unsigned int length = 0;
    for (pro_lookup_list* msg = (*obj)->value.message; msg; msg = msg->next)
        ++length;
    return length;
}


PRO_API pro_ref pro_message_get(pro_state* s,
    const pro_ref msg, unsigned int idx)
{
    assert(pro_get_type(s, msg) == PRO_MESSAGE_TYPE);

    pro_object** obj = pro_env_lookup_value(s, msg);
    pro_lookup_list* list = (*obj)->value.message;
    while (idx)
    {
        idx--;
        list = list->next;
    }
    return list ? list->value : 0;
}


PRO_API void pro_message_append(pro_state* s,
    const pro_ref msg, pro_ref lookup)
{
    assert(pro_get_type(s, msg) == PRO_MESSAGE_TYPE);
    assert(lookup);
    
    pro_object** obj = pro_env_lookup_value(s, msg);
    if (!(*obj)->value.message)
        (*obj)->value.message = pro_lookup_list_new(s, lookup, 0);
    else
        pro_lookup_list_append(s, (*obj)->value.message, lookup);
}

