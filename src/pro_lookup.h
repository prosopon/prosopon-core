#ifndef prosopon_pro_lookup_h
#define prosopon_pro_lookup_h

#include "prosopon.h"

struct pro_object;


/**
 * Internal structure for pro_ref. 
 */
typedef struct pro_lookup pro_lookup;
struct pro_lookup
{
    unsigned ref_count; /**< The lookup's reference count */
    struct pro_object* obj;   
};


/**
 * @return A new lookup.
 */
PRO_INTERNAL pro_lookup* pro_lookup_new(pro_state_ref,
    struct pro_object* obj, unsigned int ref_count);

/**
 * Frees the memory for a lookup.
 */
PRO_INTERNAL void pro_lookup_free(pro_state_ref, pro_lookup*);

/**
 * Tests if two lookups are equivlent.
 *
 * @return Zero if false or non zero if true.
 */
PRO_INTERNAL int pro_lookup_equal(pro_state_ref,
    const pro_lookup*, const pro_lookup*);
    


#endif
