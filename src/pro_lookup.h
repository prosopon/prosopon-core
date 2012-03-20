#ifndef prosopon_pro_lookup_h
#define prosopon_pro_lookup_h

#include "prosopon.h"
#include "prosopon_config.h"


struct pro_lookup
{
    pro_env* env;
    unsigned int index;
};


/**
 * Tests if two lookups are equivlent.
 *
 * @return Zero if false or non zero if true.
 */
PRO_INTERNAL int pro_lookup_equal(pro_state*, const pro_lookup*, const pro_lookup*);

#endif
