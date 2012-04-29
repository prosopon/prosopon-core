#ifndef prosopon_pro_lookup_list_h
#define prosopon_pro_lookup_list_h

#include "prosopon/core.h"



/**
 * A list of lookup values.
 */
typedef struct pro_lookup_list* pro_ref_list;

/**
 * @return A new lookup list.
 */
PRO_INTERNAL
pro_ref_list pro_lookup_list_new(pro_state_ref);

PRO_INTERNAL
pro_ref_list pro_lookup_list_copy(pro_state_ref s, const pro_ref_list cpy);

/**
 * Deallocates a lookup list.
 */
PRO_INTERNAL
void pro_lookup_list_free(pro_state_ref, pro_ref_list t);

/**
 * Appends an referance to a lookup list.
 */
PRO_INTERNAL
void pro_lookup_list_append(pro_state_ref, pro_ref_list t, pro_ref value);


PRO_INTERNAL
unsigned int pro_lookup_list_length(pro_state_ref s, const pro_ref_list t);

PRO_INTERNAL
pro_ref pro_lookup_list_get(pro_state_ref s, const pro_ref_list t, unsigned int idx);


#endif
