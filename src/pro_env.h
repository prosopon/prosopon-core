#ifndef prosopon_pro_env_h
#define prosopon_pro_env_h

#include "prosopon/core.h"


/**
 * Internal data structure that holds scope information.
 */
typedef struct pro_env pro_env;


/**
 * Internal function to create a new environment.
 *
 * @return A newly created environment.
 */
PRO_INTERNAL
pro_env* pro_env_new(pro_state_ref,
    pro_env_ref parent, unsigned int ref_count);

/**
 * Frees the memory for an env.
 */
PRO_INTERNAL
void pro_env_free(pro_state_ref, pro_env*);

/**
 * Retains an pro_env for further use.
 */
PRO_INTERNAL
pro_env* pro_internal_env_retain(pro_state_ref s, pro_env*);

/**
 * Releases a pro_env.
 */
PRO_INTERNAL
void pro_internal_env_release(pro_state_ref s, pro_env*);

/**
 * Releases all of an env's bindings.
 */
PRO_INTERNAL
void pro_env_unbind_all(pro_state_ref, pro_env*);

#endif
