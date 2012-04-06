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
PRO_INTERNAL pro_lookup* pro_lookup_new(pro_state_ref,
    pro_env_ref env, unsigned int index);

/**
 * Tests if two lookups are equivlent.
 *
 * @return Zero if false or non zero if true.
 */
PRO_INTERNAL int pro_lookup_equal(pro_state_ref,
    const pro_lookup*, const pro_lookup*);


/**
 * @return A new env lookup.
 */
PRO_INTERNAL pro_env_lookup* pro_env_lookup_new(pro_state_ref,
    struct pro_env* value);
    
/**
 * Tests if two env_lookups are equivlent.
 *
 * @return Zero if false or non zero if true.
 */
PRO_INTERNAL int pro_env_lookup_equal(pro_state_ref,
    const pro_env_lookup*, const pro_env_lookup*);
    
    

/**
 * @return A new lookup list.
 */
PRO_INTERNAL pro_ref_list pro_lookup_list_new(pro_state_ref,
    pro_ref value, pro_ref_list next);

/**
 *
 */
PRO_INTERNAL void pro_lookup_list_append(pro_state_ref,
    pro_ref_list t, pro_ref value);




#endif
