#include "pro_type.h"

#include "pro_env.h"
#include "pro_state.h"

#include <stdlib.h>
#include <string.h>

#pragma mark Private

static int default_match(pro_state_ref s,
    pro_ref t, pro_ref tData, pro_ref o)
{
    return -1;
}

static const char* defaul_to_string(pro_state_ref s,
    pro_ref t, pro_ref tData)
{
    return "";
}

static const pro_actor_type_info pro_default_actor_type_info = {
    .match = default_match,
    .to_string = defaul_to_string
};


#pragma mark -
#pragma mark Internal

PRO_INTERNAL pro_actor_type_info_list* pro_actor_type_info_list_new(pro_state_ref s, 
    pro_actor_type type, pro_actor_type_info* value, pro_actor_type_info_list* next)
{
    pro_actor_type_info_list* t = malloc(sizeof(*t));
    if (!t) return 0;
    t->type = type;
    t->value = value;
    t->next = next;
    return t;
}


pro_actor_type PRO_DEFAULT_ACTOR_TYPE = 0;


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


PRO_API void pro_register_actor_type(pro_state_ref s,
    const char* identifier, const pro_actor_type_info* info)
{
    pro_actor_type_info_list* list = malloc(sizeof(*list));
    list->value = info;
    list->type = identifier;
    
    pro_actor_type_info_list* types = pro_state_get_actor_type_info(s);
    
    if (!types)
       pro_state_set_actor_type_info(s, list);
    else
    {
        pro_actor_type_info_list* parent = types;
        while (parent->next)
            parent = parent->next;
        parent->next = list;
    }
}

