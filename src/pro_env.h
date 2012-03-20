#ifndef prosopon_pro_env_h
#define prosopon_pro_env_h

#include "prosopon.h"
#include "prosopon_config.h"
#include "pro_lookup.h"


typedef struct pro_internal_lookup pro_internal_lookup;


struct pro_env
{
    pro_env* previous; // The previous environment on the stack.
    pro_env* parent; // The parent environment used for delegation.
    
    pro_internal_lookup* table;
};


/**
 * Internal function to create a new environment.
 */
PRO_INTERNAL pro_env* pro_env_new(pro_state* s, pro_env* previous, pro_env* parent);


#endif
