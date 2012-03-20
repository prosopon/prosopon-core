#ifndef prosopon_pro_object_h
#define prosopon_pro_object_h

#include "prosopon.h"
#include "prosopon_config.h"

typedef struct pro_object pro_object;


/**
 * A funciton that evaluates a given object. 
 */
typedef void(pro_eval)(pro_state*, pro_object* t);

/**
 * A function that compares two given objects.
 *
 * @returns A negitive number if t < obj, 0 if t == obj, and a positive if t > obj 
 */
typedef int(pro_compare)(pro_state*, pro_object* t, pro_object* obj);

/**
 * A function that attempts to match two given objects.
 *
 * @returns Did matching succeed.
 */
typedef int(pro_match)(pro_state*, pro_object*, pro_object*);


/**
 *
 */
struct pro_object
{
    pro_type type;
    pro_eval* eval;
    pro_compare* compare;
    pro_match* match;
    void* data;
};


PRO_INTERNAL pro_object* pro_object_create(pro_state*,
    pro_eval* eval,
    pro_compare* compare,
    pro_match* match,
    void* data);

/**
 * @see pro_eval
 */
PRO_INTERNAL void pro_object_eval(pro_state*, pro_object*);

/**
 * @see pro_eval
 */
PRO_INTERNAL void pro_object_eval(pro_state*, pro_object*);


#endif
