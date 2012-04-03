#ifndef prosopon_pro_env_h
#define prosopon_pro_env_h

#include "prosopon.h"

#include "pro_lookup.h"
#include "pro_object.h"



typedef struct pro_env pro_env;


typedef struct pro_internal_lookup pro_internal_lookup;
typedef struct pro_lookup_binding pro_lookup_binding;

/**
 * A structure that holds scope information. 
 */
struct pro_env
{
    pro_env_ref parent; /**< The parent environment used for delegation. */
    
    pro_internal_lookup* lookups;
    unsigned int size;

    pro_lookup_binding* bindings;
};


/**
 * Internal function to create a new environment.
 *
 * @return A newly created environment.
 */
PRO_INTERNAL pro_env* pro_env_new(pro_state_ref, pro_env_ref parent);

/**
 * @return An empty lookup in a environment.
 */
PRO_INTERNAL pro_ref pro_env_next_lookup(pro_state_ref,
    pro_env_ref env);

/**
 * @return The object reference that the lookup points to.
 */
PRO_INTERNAL pro_object** pro_env_lookup_value(pro_state_ref,
    pro_ref lookup);



#endif
