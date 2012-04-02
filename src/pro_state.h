#ifndef prosopon_pro_state_h
#define prosopon_pro_state_h

#include "pro_env.h"
#include "pro_library.h"
#include "pro_type.h"

struct pro_state
{
    pro_env* root_env;
    pro_env* current_env;
    
    pro_library_list* libraries;
    
    pro_actor_type_info_list* actor_types;
};






#endif
