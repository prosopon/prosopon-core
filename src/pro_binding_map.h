#ifndef prosopon_pro_binding_map_h
#define prosopon_pro_binding_map_h

#include "prosopon.h"


/**
 * Binding map data structure.
 */
typedef struct pro_binding_map pro_binding_map;


/**
 * @return A new binding map or null if alloc failed.
 */
PRO_INTERNAL pro_binding_map* pro_binding_map_new(pro_state_ref s);

/**
 * Releases a binding map.
 */
PRO_INTERNAL void pro_binding_map_free(pro_state_ref s, pro_binding_map*);

/**
 * Lookup the bound value for an identifier.
 *
 * @return The result or PRO_EMPTY_REF if not found
 */
PRO_INTERNAL pro_ref pro_binding_map_get(pro_state_ref s, pro_binding_map*,
    const char*);

/**
 * Sets the binding for an identifier.
 */
PRO_INTERNAL void pro_binding_map_put(pro_state_ref s, pro_binding_map*, 
    const char*, pro_ref);



#endif
