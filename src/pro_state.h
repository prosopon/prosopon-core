#ifndef prosopon_pro_state_h
#define prosopon_pro_state_h

#include "prosopon/core.h"

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
    pro_alloc* alloc; /**< Allocator used to allocate and free all memory. */
    
    pro_state* main; /**< The main state. */

    struct pro_library_list* libraries; /**< The set of loaded libraries. */
    
    struct pro_actor_type_info_list* actor_types; /**< The set of actor types. */
    
    struct pro_message_queue* message_queue; /**< The global message queue. */
};


/**
 * Structure that holds information about an execution environment in the
 * Prosopon machine.
 */
struct pro_state
{
    pro_global_state* global; /**< The global state. */
    
    pro_env_ref root_env; /**< The root environment for the state. */
    struct pro_env_stack* stack; /**< The env stack for this state. */
};



/**
 * @return The list of loaded libraries.
 */
PRO_INTERNAL
struct pro_library_list* pro_state_get_libraries(pro_state*);

/**
 *
 */
PRO_INTERNAL
void pro_state_set_libraries(pro_state*, struct pro_library_list* val);

PRO_INTERNAL
struct pro_actor_type_info_list* pro_state_get_actor_type_info(pro_state*);

PRO_INTERNAL
void pro_state_set_actor_type_info(pro_state*,
    struct pro_actor_type_info_list* val);

PRO_INTERNAL
pro_state* pro_state_fork(pro_state*);

#endif
