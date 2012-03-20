#include "pro_behavior.h"


static void behavior_eval(pro_state* s, pro_object* t)
{
    pro_behavior_data* data = t->data;
    pro_case* c = data->cases;
    while (c)
    {
        //c->match->match(s, c->match);
        
    }
}

PRO_INTERNAL pro_object* pro_behavior_create(pro_state* s)
{
    return 0;
}

PRO_INTERNAL pro_object* pro_behavior_add_case(pro_state* s, pro_case* c)
{
    return 0;
}