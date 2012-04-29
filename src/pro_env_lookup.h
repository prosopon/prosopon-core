#ifndef prosopon_pro_env_lookup_h
#define prosopon_pro_env_lookup_h

#include "prosopon/core.h"

struct pro_env;


/**
 * Internal structure for pro_env_ref
 */
typedef struct pro_env_lookup pro_env_lookup;


/**
 * @return A new env lookup.
 */
PRO_INTERNAL
pro_env_lookup* pro_env_lookup_new(pro_state_ref,
    struct pro_env* value, unsigned int ref_count);

/**
 * Frees an the memory for an env_lookup
 */
PRO_INTERNAL
void pro_env_lookup_free(pro_state_ref, pro_env_lookup*);

/**
 * Tests if two env_lookups are equivlent.
 *
 * @return Zero if false or non zero if true.
 */
PRO_INTERNAL
int pro_env_lookup_equal(pro_state_ref,
    const pro_env_lookup*, const pro_env_lookup*);

/**
 * Dereferenaces a pro_env_ref
 */
PRO_INTERNAL
struct pro_env* pro_env_dereference(pro_state_ref, pro_env_ref);


#endif
