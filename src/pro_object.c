#include "pro_object.h"

#include <stdlib.h>
#include <assert.h>



static int default_compare(pro_state* s, pro_object* t, pro_object* obj)
{
    return t == obj;
}

static int default_match(pro_state* s, pro_object* t, pro_object* obj)
{
    return t == obj;
}


PRO_INTERNAL pro_object* pro_object_create(pro_state* s,
    pro_compare* compare,
    pro_match* match,
    void* data)
{
    pro_object* t = malloc(sizeof(*t));
    t->compare = compare ? compare : default_compare;
    t->match = match ? match : default_match;
    t->data = data;
    return t;
}
