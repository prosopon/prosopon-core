#include "pro_type.h"

#include "pro_env.h"
#include "pro_state.h"

#include <string.h>


#pragma mark Private

static pro_matching default_match(pro_state_ref s,
    pro_ref t, pro_ref tData, pro_ref o)
{
    return PRO_MATCH_FAIL;
}

static char* default_to_string(pro_state_ref s,
    pro_ref t, pro_ref tData)
{
    return 0;
}

static const pro_actor_type_info pro_default_actor_type_info = {
    .match = default_match,
    .to_string = default_to_string
};


#pragma mark -
#pragma mark Internal

PRO_INTERNAL pro_actor_type_info_list* pro_actor_type_info_list_new(pro_state_ref s, 
    pro_actor_type type, const pro_actor_type_info* value, pro_actor_type_info_list* next)
{
    pro_alloc* alloc;
    pro_get_alloc(s, &alloc);
    pro_actor_type_info_list* t = alloc(0, sizeof(*t));
    if (!t) return 0;
    
    t->type = type;
    t->value = value;
    t->next = next;
    return t;
}

PRO_INTERNAL void pro_actor_type_info_list_free(pro_state_ref s,
    pro_actor_type_info_list* t)
{
    pro_alloc* alloc;
    pro_get_alloc(s, &alloc);
    
    for (pro_actor_type_info_list* info = t; info;)
    {
        pro_actor_type_info_list* next = info->next;
        alloc(info, 0);
        info = next;
    }
}


PRO_INTERNAL void initialize_default_actor_types(pro_state_ref s)
{
    pro_register_actor_type(s, PRO_DEFAULT_ACTOR_TYPE, &pro_default_actor_type_info);
}


PRO_INTERNAL const pro_actor_type_info* pro_get_actor_type_info(pro_state_ref s, 
    pro_actor_type type)
{
    pro_actor_type_info_list* list = pro_state_get_actor_type_info(s);
    while (list)
    {
        if (list->type == 0)
        {
            if (type == 0)
                return list->value;
        }
        else if (strcmp(list->type, type) == 0)
            return list->value;
        
        list = list->next;
    }
    return 0;
}


#pragma mark -
#pragma mark Public

pro_actor_type PRO_DEFAULT_ACTOR_TYPE = 0;


PRO_API void pro_register_actor_type(pro_state_ref s,
    const char* identifier, const pro_actor_type_info* info)
{
    pro_alloc* alloc;
    pro_get_alloc(s, &alloc);
    pro_actor_type_info_list* list = pro_actor_type_info_list_new(s, identifier, info, 0);
    
    pro_actor_type_info_list* parent = pro_state_get_actor_type_info(s);
    
    if (!parent)
       pro_state_set_actor_type_info(s, list);
    else
    {
        while (parent->next)
            parent = parent->next;
        parent->next = list;
    }
}

