#include "pro_lookup.h"

#include "pro_common.h"
#include "pro_state.h"
#include "pro_env.h"
#include "pro_object.h"
#include "pro_env_lookup.h"



#pragma mark Internal

PRO_INTERNAL pro_lookup* pro_lookup_new(pro_state* s,
    struct pro_object* obj, unsigned int ref_count)
{
    pro_alloc* alloc;
    pro_get_alloc(s, &alloc);
    pro_lookup* t = alloc(0, sizeof(*t));
    if (!t) return 0;
    
    t->ref_count = ref_count;
    t->obj = obj;
    return t;
}


PRO_INTERNAL int pro_lookup_equal(pro_state* s,
    const pro_lookup* o1, const pro_lookup* o2)
{
    if (o1 == o2)
        return 1;
    
    if (PRO_EMPTY_REF == o1 || PRO_EMPTY_REF == o2)
        return 0;
    
    return (o1->obj == o2->obj);
}

PRO_INTERNAL void pro_lookup_free(pro_state_ref s, pro_lookup* t)
{
    pro_alloc* alloc;
    pro_get_alloc(s, &alloc);
    
    // Release the object
    pro_object* obj = pro_dereference(s, t);
    pro_object_release(s, obj);

    // Free the lookup memory
    alloc(t, 0);
}


PRO_INTERNAL pro_object* pro_dereference(pro_state_ref s, pro_ref ref)
{
    return pro_lookup_equal(s, ref, PRO_EMPTY_REF) ? 0 : ref->obj;
}


#pragma mark -
#pragma mark Public

pro_ref PRO_EMPTY_REF = 0;


PRO_API pro_error pro_retain(pro_state_ref s, pro_ref ref)
{
    if (!pro_lookup_equal(s, ref, PRO_EMPTY_REF))
        ref->ref_count++;
    return PRO_OK;
}


PRO_API pro_error pro_release(pro_state_ref s, pro_ref ref)
{
    // release PRO_EMPTY_REF is a valid noop
    if (pro_lookup_equal(s, ref, PRO_EMPTY_REF))
        return PRO_OK;
    
    assert(ref->ref_count > 0);
    
    if (--(ref->ref_count) <= 0)
        pro_lookup_free(s, ref);
    
    return PRO_OK;
}

