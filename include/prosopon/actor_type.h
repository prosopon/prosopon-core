#ifndef prosopon_prosopon_interface_type_h
#define prosopon_prosopon_interface_type_h

#ifdef __cplusplus
extern "C" {
#endif

#include <prosopon/core.h>


/**
 * Primitive function implementation for matching between two objects.
 */
typedef pro_matching(pro_match_impl)(pro_state_ref,
    pro_ref t, pro_ref tData,
    pro_ref o, pro_ref oData);

/**
 * Primitive function implementation to get the string representation of an actor.
 *
 * @return A reference to a user data object containing a string.
 */
typedef pro_ref(pro_to_string_impl)(pro_state_ref, pro_ref t, pro_ref tData);

/**
 * Information about an actor type.
 */
typedef struct 
{
    pro_match_impl* match;
    pro_to_string_impl* to_string;
} pro_actor_type_info;


/**
 * 
 */
PRO_API void (pro_register_actor_type) (pro_state_ref,
    pro_actor_type, const pro_actor_type_info*);

PRO_API
pro_error (pro_actor_request_ud) (pro_state_ref,
    pro_ref, PRO_OUT pro_ref* ud);


#ifdef __cplusplus
}
#endif

#endif
