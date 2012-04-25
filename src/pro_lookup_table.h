#ifndef prosopon_pro_lookup_table_h
#define prosopon_pro_lookup_table_h

#include "prosopon.h"

struct pro_object;


/**
 * Lookup table structure. 
 */
typedef struct pro_lookup_table pro_lookup_table;


/**
 * @return A new, empty lookup table.
 */
PRO_INTERNAL pro_lookup_table* pro_lookup_table_new(pro_state_ref s);

/**
 * Releases a lookup table's memory
 */
PRO_INTERNAL void pro_lookup_table_free(pro_state_ref s, pro_lookup_table* t);

/**
 * Reserves a slot for an item in the lookup table and returns the index of this spot.
 */
PRO_INTERNAL unsigned int pro_lookup_table_add(pro_state_ref s, pro_lookup_table* t);


/**
 * Removes a slot from the lookup table.
 */
PRO_INTERNAL void pro_lookup_table_remove(pro_state_ref s, pro_lookup_table* t,
    unsigned int index);


/**
 */
PRO_INTERNAL struct pro_object** pro_lookup_table_get(pro_state_ref,
    pro_lookup_table* t, unsigned int index);



#ifdef DEBUG
PRO_INTERNAL void pro_lookup_table_print(pro_state_ref, pro_lookup_table* t);
#endif

#endif
