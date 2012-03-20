#include "pro_lookup.h"


PRO_INTERNAL int pro_lookup_equal(pro_state* s, const pro_lookup* o1, const pro_lookup* o2)
{
    return (o1->env == o2->env && o1->index == o2->index);
}
