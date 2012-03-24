#include "pro_actor.h"

#include "prosopon.h"
#include "prosopon_config.h"
#include "pro_lookup.h"

#include <stdlib.h>


PRO_API pro_lookup* pro_actor_create(pro_state* s)
{
    pro_lookup* lookup = malloc(sizeof(*lookup));
    return lookup;
}

PRO_API void pro_send(pro_state* s, pro_lookup* actor, pro_lookup* msg)
{

}


PRO_API void pro_become(pro_state* s,
    pro_lookup* actor, pro_behavior* new_beh, void* data)
{

}
