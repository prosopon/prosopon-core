#ifndef prosopon_prosopon_interface_type_h
#define prosopon_prosopon_interface_type_h

#include "prosopon.h"


typedef int(pro_match_impl)(pro_state_ref,
    pro_ref t, const void* tData,
    pro_ref o, const void* oData);

typedef const char*(pro_to_string_impl)(pro_state_ref,
    pro_ref t, const void* tData);

typedef struct pro_actor_type_info pro_actor_type_info;
struct pro_actor_type_info
{
    pro_match_impl* match;
    pro_to_string_impl* to_string;
};

PRO_API void (pro_register_actor_type) (pro_state_ref,
    pro_actor_type, const pro_actor_type_info*);

PRO_API const void* (pro_request_actor_data) (pro_state_ref,
    pro_ref);

#endif
