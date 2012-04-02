#include "pro_type.h"

#include "pro_env.h"
#include "pro_state.h"

#include <stdlib.h>
#include <string.h>

#pragma mark Private

static int default_match(pro_state* s,
    const pro_lookup* t, const void* tData, const pro_lookup* o, const void* oData)
{
    return -1;
}

static const char* defaul_to_string(pro_state* s,
    const pro_lookup* t, const void* tData)
{
    return "";
}

static const pro_actor_type_info pro_default_actor_type_info = {
    .match = default_match,
    .to_string = defaul_to_string
};


#pragma mark -
#pragma mark Internal

PRO_INTERNAL pro_actor_type_info_list* pro_actor_type_info_list_new(pro_state* s, 
    pro_actor_type type, pro_actor_type_info* value, pro_actor_type_info_list* next)
{
    pro_actor_type_info_list* t = malloc(sizeof(*t));
    t->type = type;
    t->value = value;
    t->next = next;
    return t;
}


pro_actor_type PRO_DEFAULT_ACTOR_TYPE = 0;


PRO_INTERNAL void initialize_default_actor_types(pro_state* s)
{
    pro_register_actor_type(s, PRO_DEFAULT_ACTOR_TYPE, &pro_default_actor_type_info);
}

PRO_INTERNAL const pro_actor_type_info* pro_get_actor_type_info(pro_state* s, 
    pro_actor_type type)
{
    pro_actor_type_info_list* list = s->actor_types;
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


PRO_API void pro_register_actor_type(pro_state* s,
    const char* identifier, const pro_actor_type_info* info)
{
    pro_actor_type_info_list* list = malloc(sizeof(*list));
    list->value = info;
    list->type = identifier;
    
    if (!s->actor_types)
        s->actor_types = list;
    else
    {
        pro_actor_type_info_list* parent = s->actor_types;
        while (parent->next)
            parent = parent->next;
        parent->next = list;
    }
}


PRO_API pro_type pro_get_type(pro_state* s, const pro_lookup* lookup)
{
    pro_object** obj = pro_env_lookup_value(s, lookup);
    return (*obj)->type;
}


