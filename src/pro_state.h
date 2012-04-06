#ifndef prosopon_pro_state_h
#define prosopon_pro_state_h

#include "pro_env.h"
#include "pro_library.h"
#include "pro_type.h"
#include "pro_message_queue.h"

typedef struct pro_state pro_state;
typedef struct pro_env_stack pro_env_stack;


struct pro_state
{
    pro_env* root_env;
    pro_env_stack* stack;
    
    pro_library_list* libraries;
    
    pro_actor_type_info_list* actor_types;
    
    pro_message_queue* message_queue;
};


struct pro_env_stack
{
    pro_env_stack* next;
    pro_env_ref value;
};


PRO_INTERNAL pro_env_stack* pro_env_stack_new(pro_state_ref state,
    pro_env_ref value, pro_env_stack* next);




#endif
