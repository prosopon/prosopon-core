#ifndef prosopon_prosopon_interface_type_h
#define prosopon_prosopon_interface_type_h

#ifdef __cplusplus
extern "C" {
#endif

#include "prosopon.h"


typedef pro_matching(pro_match_impl)(pro_state_ref,
    pro_ref t, pro_ref tData,
    pro_ref o, pro_ref oData);

typedef pro_ref(pro_to_string_impl)(pro_state_ref, pro_ref t, pro_ref tData);

typedef struct pro_actor_type_info pro_actor_type_info;
struct pro_actor_type_info
{
    pro_match_impl* match;
    pro_to_string_impl* to_string;
};

PRO_API void (pro_register_actor_type) (pro_state_ref,
    pro_actor_type, const pro_actor_type_info*);

PRO_API pro_error (pro_actor_request_ud) (pro_state_ref,
    pro_ref, PRO_OUT pro_ref* ud);


#ifdef __cplusplus
}
#endif

#endif
