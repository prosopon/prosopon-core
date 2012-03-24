#ifndef prosopon_pro_env_h
#define prosopon_pro_env_h

#include "prosopon.h"

#include "pro_lookup.h"
#include "pro_object.h"



typedef struct pro_env pro_env;


typedef struct pro_internal_lookup pro_internal_lookup;

/**
 * A structure that holds scope information. 
 */
struct pro_env
{
    pro_env* previous; // The previous environment on the stack.
    pro_env* parent; // The parent environment used for delegation.
    
    unsigned int size;
    pro_internal_lookup* table;
};


/**
 * Internal function to create a new environment.
 */
PRO_INTERNAL pro_env* pro_env_new(pro_state*,
    pro_env* previous, pro_env* parent);

/**
 */
PRO_INTERNAL pro_lookup* pro_env_next_lookup(pro_state*,
    pro_env_lookup* env);

/**
 */
PRO_INTERNAL pro_object** pro_env_lookup_value(pro_state*,
    pro_lookup* lookup);



#endif
