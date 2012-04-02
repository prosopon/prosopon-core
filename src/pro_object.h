#ifndef prosopon_pro_object_h
#define prosopon_pro_object_h

#include "prosopon.h"


typedef struct pro_object pro_object;


/**
 *
 */
struct pro_object
{
    
    pro_type type;
    pro_actor_type actor_type;
    union
    {
        struct
        {
            pro_actor_type type;
            pro_behavior behavior;
            pro_env_ref env;
        } actor;
        struct
        {
            pro_constructor constructor;
        } constructor;
        pro_lookup_list* message;
    } value;
};

/**
 *
 */
PRO_INTERNAL pro_object* pro_object_new(pro_state*, pro_type type);

#endif
