#ifndef prosopon_pro_state_h
#define prosopon_pro_state_h

#include "prosopon.h"

struct pro_message_queue;
struct pro_env_stack;
struct pro_library_list;
struct pro_actor_type_info_list;


typedef struct pro_state pro_state;


/**
 * Structure that holds information about the state of the Prosopon machine
 */
typedef struct pro_global_state pro_global_state;
struct pro_global_state
{
    pro_alloc* alloc;
    
    pro_state* main;

    struct pro_library_list* libraries;
    
    struct pro_actor_type_info_list* actor_types;
    
    struct pro_message_queue* message_queue;
};


/**
 * Structure that holds information about an execution environment in the Prosopon
 * machine.
 */
struct pro_state
{
    pro_global_state* global;
    
    pro_env_ref root_env;
    struct pro_env_stack* stack;
};

PRO_INTERNAL pro_state* pro_state_fork(pro_state*);


PRO_INTERNAL struct pro_library_list* pro_state_get_libraries(pro_state*);
PRO_INTERNAL void pro_state_set_libraries(pro_state*, struct pro_library_list* val);

PRO_INTERNAL struct pro_actor_type_info_list* pro_state_get_actor_type_info(pro_state*);
PRO_INTERNAL void pro_state_set_actor_type_info(pro_state*,
    struct pro_actor_type_info_list* val);


#endif
