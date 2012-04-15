#ifndef prosopon_pro_lookup_list_h
#define prosopon_pro_lookup_list_h

#include "prosopon.h"


/**
 * @return A new lookup list.
 */
PRO_INTERNAL pro_ref_list pro_lookup_list_new(pro_state_ref,
    pro_ref value, pro_ref_list next);

/**
 * Appends an referance to a lookup list.
 */
PRO_INTERNAL void pro_lookup_list_append(pro_state_ref,
    pro_ref_list t, pro_ref value);


#endif
