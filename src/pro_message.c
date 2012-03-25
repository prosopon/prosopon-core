#include "pro_message.h"

#include "prosopon.h"

#include "pro_env.h"


#pragma mark -
#pragma mark PRO_API


PRO_API pro_lookup* pro_message_create(pro_state*s)
{
    pro_lookup* lookup = pro_env_next_lookup(s, pro_get_env(s));
    pro_object** obj = pro_env_lookup_value(s, lookup);
    *obj = pro_object_new(s, PRO_MESSAGE_TYPE);
    return lookup;
}


PRO_API unsigned int pro_message_length(pro_state* s, pro_lookup* lookup)
{
    pro_object** obj = pro_env_lookup_value(s, lookup);
    unsigned int length = 0;
    for (pro_lookup_list* msg = (*obj)->value.message; msg; msg = msg->next)
        length++;
    return length;
}


PRO_API pro_lookup* pro_message_get(pro_state* s, pro_lookup* lookup, unsigned int idx)
{
    pro_object** obj = pro_env_lookup_value(s, lookup);
    pro_lookup_list* list = (*obj)->value.message;
    while (idx)
    {
        idx--;
        list = list->next;
    }
    return list ? list->value : 0;
}


PRO_API void pro_message_append(pro_state* s, pro_lookup* lookup)
{

}