#ifndef prosopon_pro_behavior_h
#define prosopon_pro_behavior_h

#include "prosopon.h"
#include "prosopon_config.h"
#include "pro_object.h"

typedef struct pro_case pro_case;


struct pro_case 
{
    pro_case* next;
    pro_object* match;
    pro_object* body;
};


typedef struct
{
    pro_case* cases;
} pro_behavior_data;


PRO_INTERNAL pro_object* pro_behavior_create(pro_state*);

PRO_INTERNAL pro_object* pro_behavior_add_case(pro_state*, pro_case*);

PRO_INTERNAL pro_object* pro_behavior_recieve(pro_state*, pro_case*);


#endif
