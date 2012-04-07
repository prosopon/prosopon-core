#ifndef prosopon_pro_type_h
#define prosopon_pro_type_h

#include "prosopon.h"
#include "prosopon_actor_type.h"


typedef struct pro_actor_type_info_list pro_actor_type_info_list;
struct pro_actor_type_info_list
{
    pro_actor_type_info_list* next;
    pro_actor_type type;
    const pro_actor_type_info* value;
};


PRO_INTERNAL void initialize_default_actor_types(pro_state_ref);


PRO_INTERNAL pro_actor_type_info_list* pro_actor_type_info_list_new(pro_state_ref, 
    pro_actor_type type, pro_actor_type_info* value, pro_actor_type_info_list* next);


PRO_INTERNAL const pro_actor_type_info* pro_get_actor_type_info(pro_state_ref, 
    pro_actor_type type);


#endif
