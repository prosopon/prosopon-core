#ifndef prosopon_pro_lookup_table_h
#define prosopon_pro_lookup_table_h

#include "prosopon.h"

struct pro_internal_lookup;
struct pro_object;


typedef struct pro_lookup_table pro_lookup_table;
struct pro_lookup_table
{
    struct pro_internal_lookup* value;
    unsigned int size;
};



PRO_INTERNAL pro_lookup_table* pro_lookup_table_new(pro_state_ref s);


PRO_INTERNAL void pro_lookup_table_free(pro_state_ref s, pro_lookup_table* t);


PRO_INTERNAL unsigned int pro_lookup_table_append(pro_state_ref s, pro_lookup_table* t);


PRO_INTERNAL void pro_lookup_table_remove(pro_state_ref s, pro_lookup_table* t,
    unsigned int index);


/**
 */
PRO_INTERNAL struct pro_object** pro_lookup_table_get(pro_state_ref,
    pro_lookup_table* t, unsigned int index);


#endif
