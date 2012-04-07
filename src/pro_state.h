#ifndef prosopon_pro_state_h
#define prosopon_pro_state_h

#include "prosopon.h"

struct pro_message_queue;
struct pro_env_stack;
struct pro_library_list;
struct pro_actor_type_info_list;



/**
 * Structure that holds information about the state of the Prosopon machine
 */
typedef struct pro_state pro_state;
struct pro_state
{
    pro_env_ref root_env;
    struct pro_env_stack* stack;
    
    struct pro_library_list* libraries;
    
    struct pro_actor_type_info_list* actor_types;
    
    struct pro_message_queue* message_queue;
};

#endif
