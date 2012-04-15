#ifndef prosopon_pro_lookup_h
#define prosopon_pro_lookup_h

#include "prosopon.h"


/**
 * Internal structure for pro_ref. 
 */
typedef struct pro_lookup pro_lookup;
struct pro_lookup
{
    unsigned ref_count;
    pro_env_ref env;
    unsigned int index;
};


/**
 * @return A new lookup.
 */
PRO_INTERNAL pro_lookup* pro_lookup_new(pro_state_ref,
    pro_env_ref env, unsigned int index, unsigned int ref_count);

/**
 * Tests if two lookups are equivlent.
 *
 * @return Zero if false or non zero if true.
 */
PRO_INTERNAL int pro_lookup_equal(pro_state_ref,
    const pro_lookup*, const pro_lookup*);
    


#endif
