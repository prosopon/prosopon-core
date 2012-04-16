#ifndef prosopon_pro_object_h
#define prosopon_pro_object_h

#include "prosopon.h"

#include "pro_lookup_list.h"


/**
 * Structure that holds data for all objects.
 */
typedef struct pro_object pro_object;
struct pro_object
{
    unsigned int ref_count;
    pro_type type;
    union
    {
        struct
        {
            pro_actor_type type;
            pro_behavior* behavior;
            pro_ref data;
            pro_env_ref env;
        } actor;
        struct
        {
            pro_constructor* constructor;
            pro_ref data;
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
 * Allocates a new pro_object of a given type.
 *
 * @return The newly allocated object
 */
PRO_INTERNAL pro_object* pro_object_new(pro_state_ref,
    pro_type type, unsigned int ref_count);

/**
 * Frees the memory for a pro_object.
 */
PRO_INTERNAL void pro_object_free(pro_state_ref, pro_object*);

/**
 * Retains a pro_object.
 */
PRO_INTERNAL pro_object* pro_object_retain(pro_state_ref, pro_object*);

/**
 * Releases a pro_object.
 *
 * When ref count is zero, object can be released.
 * Caller must not use the object after calling release.
 */
PRO_INTERNAL void pro_object_release(pro_state_ref, pro_object*);


#endif
