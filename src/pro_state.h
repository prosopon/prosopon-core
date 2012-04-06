#ifndef prosopon_pro_state_h
#define prosopon_pro_state_h

#include "pro_env.h"
#include "pro_library.h"
#include "pro_type.h"

struct pro_message_queue;
struct pro_env_stack;

typedef struct pro_state pro_state;


/**
 * Structure that holds information about the state of the Prosopon machine
 */
struct pro_state
{
    pro_env_ref root_env;
    struct pro_env_stack* stack;
    
    pro_library_list* libraries;
    
    pro_actor_type_info_list* actor_types;
    
    struct pro_message_queue* message_queue;
};

#endif
