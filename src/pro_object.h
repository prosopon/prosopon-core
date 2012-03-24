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
            pro_behavior* behavior;
            void* data;
        } actor;
        struct
        {
            pro_constructor* constructor;
            void* data;
        } constructor;
        pro_lookup_list* message;
    } value;
};



#endif
