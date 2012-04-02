#ifndef prosopon_pro_lookup_h
#define prosopon_pro_lookup_h

#include "prosopon.h"

struct pro_env;


struct pro_lookup
{
    struct pro_env* env;
    unsigned int index;
};


/**
 * @return A new lookup.
 */
PRO_INTERNAL pro_lookup* pro_lookup_new(pro_state*,
    struct pro_env* env, unsigned int index);

/**
 * Tests if two lookups are equivlent.
 *
 * @return Zero if false or non zero if true.
 */
PRO_INTERNAL int pro_lookup_equal(pro_state*,
    const pro_lookup*, const pro_lookup*);



/**
 * @return A new lookup list.
 */
PRO_INTERNAL pro_lookup_list* pro_lookup_list_new(pro_state*,
    pro_lookup* value, pro_lookup_list* next);

/**
 *
 */
PRO_INTERNAL void pro_lookup_list_append(pro_state*,
    pro_lookup_list* t, pro_lookup* value);




#endif
