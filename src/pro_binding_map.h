#ifndef prosopon_pro_binding_map_h
#define prosopon_pro_binding_map_h

#include "prosopon.h"

struct pro_lookup_binding;


typedef struct pro_binding_map pro_binding_map;

struct pro_binding_map
{
    struct pro_lookup_binding* value;
};


PRO_INTERNAL pro_binding_map* pro_binding_map_new(pro_state_ref s);

PRO_INTERNAL void pro_binding_map_free(pro_state_ref s, pro_binding_map*);


PRO_INTERNAL pro_ref pro_binding_map_get(pro_state_ref s, pro_binding_map*, const char*);

PRO_INTERNAL void pro_binding_map_put(pro_state_ref s,
    pro_binding_map*, const char*, pro_ref);



#endif
