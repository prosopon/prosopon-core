#ifndef prosopon_pro_lookup_h
#define prosopon_pro_lookup_h

#include "prosopon.h"
#include "prosopon_config.h"

struct pro_env;


struct pro_lookup
{
    struct pro_env* env;
    unsigned int index;
};


/**
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

#endif
