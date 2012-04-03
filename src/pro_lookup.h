#ifndef prosopon_pro_lookup_h
#define prosopon_pro_lookup_h

#include "prosopon.h"

struct pro_env;


typedef struct pro_lookup pro_lookup;
typedef struct pro_env_lookup pro_env_lookup;


struct pro_lookup
{
    pro_env_ref env;
    unsigned int index;
};


struct pro_env_lookup
{
    struct pro_env* value;
};



/**
 * @return A new lookup.
 */
PRO_INTERNAL pro_lookup* pro_lookup_new(pro_state*,
    pro_env_ref env, unsigned int index);

/**
 * Tests if two lookups are equivlent.
 *
 * @return Zero if false or non zero if true.
 */
PRO_INTERNAL int pro_lookup_equal(pro_state*,
    pro_ref, pro_ref);


/**
 * @return A new env lookup.
 */
PRO_INTERNAL pro_env_lookup* pro_env_lookup_new(pro_state*,
    struct pro_env* value);
    


/**
 * @return A new lookup list.
 */
PRO_INTERNAL pro_ref_list pro_lookup_list_new(pro_state*,
    pro_ref value, pro_ref_list next);

/**
 *
 */
PRO_INTERNAL void pro_lookup_list_append(pro_state*,
    pro_ref_list t, pro_ref value);




#endif
