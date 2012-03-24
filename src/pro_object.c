#include "pro_object.h"

#include <stdlib.h>
#include <assert.h>


PRO_INTERNAL pro_object* pro_object_new(pro_state* s, pro_type type)
{
    pro_object* t = malloc(sizeof(*t));
    t->type = type;
    return t;
}