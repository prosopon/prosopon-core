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
    union
    {
        struct
        {
            pro_actor_type type;
            pro_behavior behavior;
            pro_ref data;
            pro_env_ref env;
        } actor;
        struct
        {
            pro_constructor constructor;
        } constructor;
        pro_ref_list message;
        struct
        {
            void* data;
            size_t size;
            pro_ud_deconstructor* deconstructor;
        } ud;
    } value;
};


/**
 *
 */
PRO_INTERNAL pro_object* pro_object_new(pro_state_ref, pro_type type);

#endif
