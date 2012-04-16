#ifndef prosopon_pro_lookup_list_h
#define prosopon_pro_lookup_list_h

#include "prosopon.h"



/**
 * A list of lookup values.
 */
typedef struct pro_lookup_list* pro_ref_list;
struct pro_lookup_list
{
    pro_ref_list next;
    pro_ref value;
};

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


/**
 * Deallocates a lookup list.
 */
PRO_INTERNAL void pro_lookup_list_free(pro_state_ref, pro_ref_list t);


#endif
