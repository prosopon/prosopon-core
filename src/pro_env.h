#ifndef prosopon_pro_env_h
#define prosopon_pro_env_h

#include "prosopon.h"

struct pro_object;


/**
 * Internal data structure that holds scope information.
 */
typedef struct pro_env pro_env;


/**
 * Internal function to create a new environment.
 *
 * @return A newly created environment.
 */
PRO_INTERNAL pro_env* pro_env_new(pro_state_ref,
    pro_env_ref parent, unsigned int ref_count);

/**
 * Frees the memory for an env.
 */
PRO_INTERNAL void pro_env_free(pro_state_ref, pro_env*);

/**
 * Retains an pro_env for further use.
 */
PRO_INTERNAL pro_env* pro_internal_env_retain(pro_state_ref s, pro_env*);

/**
 * Releases a pro_env.
 */
PRO_INTERNAL void pro_internal_env_release(pro_state_ref s, pro_env*);

/**
 * @return An empty lookup in a environment.
 */
PRO_INTERNAL pro_ref pro_env_next_lookup(pro_state_ref,
    pro_env_ref env);

/**
 * @return The object reference that the lookup points to.
 */
PRO_INTERNAL struct pro_object** pro_env_lookup_value(pro_state_ref,
    pro_ref ref);

/**
 * Dereferenaces a pro_ref
 */
PRO_INTERNAL struct pro_object* pro_dereference(pro_state_ref, pro_ref);

/**
 * Removes a lookup from an environment.
 */
PRO_INTERNAL void pro_env_lookup_remove(pro_state_ref, pro_env*, pro_ref ref);

/**
 * Releases all of an env's bindings.
 */
PRO_INTERNAL void pro_env_unbind_all(pro_state_ref, pro_env*);

#endif
