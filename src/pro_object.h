#ifndef prosopon_pro_object_h
#define prosopon_pro_object_h

#include "prosopon.h"


typedef struct pro_object pro_object;


/**
 * A function that compares two given objects.
 *
 * @returns A negitive number if t < obj, 0 if t == obj, and a positive if t > obj 
 */
typedef int(pro_compare)(pro_state*, pro_object* t, pro_object* obj);

/**
 * A function that attempts to match two given objects.
 *
 * @returns Did matching succeed.
 */
typedef int(pro_match)(pro_state*, pro_object*, pro_object*);


typedef unsigned int pro_object_type;


/**
 *
 */
struct pro_object
{
    pro_type type;
    pro_object_type object_type;
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


PRO_INTERNAL pro_object* pro_object_create(pro_state*,
    pro_compare* compare,
    pro_match* match,
    void* data
);

#endif
