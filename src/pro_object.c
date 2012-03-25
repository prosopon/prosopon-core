#include "pro_object.h"

#include <stdlib.h>
#include <assert.h>


PRO_INTERNAL pro_object* pro_object_new(pro_state* s, pro_type type)
{
    pro_object* t = malloc(sizeof(*t));
    t->type = type;
    return t;
}



PRO_API int pro_match(pro_state* s, const pro_lookup* l1, const pro_lookup* l2)
{
    return 0; // TODO
}