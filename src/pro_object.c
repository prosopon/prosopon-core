#include "pro_object.h"

#include "pro_type.h"
#include "pro_env.h"
#include "pro_object.h"
#include "pro_common.h"
#include "pro_lookup.h"
#include "pro_env_lookup.h"
#include "pro_lookup_list.h"


PRO_INTERNAL
pro_object* pro_object_new(pro_state_ref s,
    pro_type type, unsigned int ref_count)
{
    pro_alloc* alloc;
    pro_get_alloc(s, &alloc);
    pro_object* t = alloc(0, sizeof(*t));
    if (!t) return 0;
    
    t->type = type;
    t->ref_count = ref_count;
    return t;
}


PRO_INTERNAL
void pro_object_free(pro_state_ref s, pro_object* t)
{    
    // Free object data
    switch (t->type)
    {
    case PRO_ACTOR_TYPE:
        pro_release(s, t->value.actor.data);
        pro_env_release(s, t->value.actor.env);
        break;
    case PRO_LIST_TYPE:
        // Release the message list
        pro_lookup_list_free(s, t->value.message);
        break;
    case PRO_UD_TYPE:
        // call the ud deconstructor
        t->value.ud.deconstructor(s, t->value.ud.data);
        break;
    case PRO_CONSTRUCTOR_TYPE:
        pro_release(s, t->value.constructor.data);
        pro_env_release(s, t->value.constructor.env);
        break;
    }
    
    // Free the pro_object
    pro_alloc* alloc;
    pro_get_alloc(s, &alloc);
    alloc(t, 0);
}


PRO_INTERNAL
pro_object* pro_object_retain(pro_state_ref s, pro_object* t)
{
    t->ref_count++;
    return t;
}


PRO_INTERNAL
void pro_object_release(pro_state_ref s, pro_object* t)
{
    if (!t) return;
    assert(t->ref_count > 0);
    
    if (--(t->ref_count) <= 0)
        pro_object_free(s, t);
}


#pragma mark -
#pragma mark Public

PRO_API
pro_error pro_get_type(pro_state_ref s, pro_ref ref,
    PRO_OUT pro_type* type)
{
    PRO_API_ASSERT(s, PRO_INVALID_STATE);
    PRO_API_ASSERT(!pro_lookup_equal(s, ref, PRO_EMPTY_REF), PRO_INVALID_OPERATION);
    
    pro_object* obj = pro_dereference(s, ref);
    *type = obj->type;
    return PRO_OK;
}


PRO_API
pro_error pro_match(pro_state_ref s, pro_ref l1, pro_ref l2, PRO_OUT pro_matching* out)
{
    PRO_API_ASSERT(s, PRO_INVALID_STATE);
        
    // first check if the two lookups are equal
    if (pro_lookup_equal(s, l1, l2))
    {
        *out = PRO_MATCH_SUCCEED;
        return PRO_OK;
    }
    else
    {
        if (pro_lookup_equal(s, l1, PRO_EMPTY_REF) || pro_lookup_equal(s, l2, PRO_EMPTY_REF))
        {
            *out = PRO_MATCH_FAIL;
            return PRO_OK;
        }
        
        PRO_API_ASSERT_TYPE(l1, PRO_ACTOR_TYPE, PRO_INVALID_OPERATION);
        PRO_API_ASSERT_TYPE(l2, PRO_ACTOR_TYPE, PRO_INVALID_OPERATION);
    
        // get the type info for the first object
        pro_actor_type type;
        pro_get_actor_type(s, l1, &type);
        const pro_actor_type_info* info = pro_get_actor_type_info(s, type);
        
        // if no match function, matched fail
        if (!info->match)
        {
            *out = PRO_MATCH_FAIL;
            return PRO_OK;
        }
        
        // get both objects
        pro_object* o1 = pro_dereference(s, l1);
        pro_object* o2 = pro_dereference(s, l2);
        
        // call the actual match function
        *out = info->match(s,
            l1, o1->value.actor.data,
            l2, o2->value.actor.data);
        return PRO_OK;
    }
}


PRO_API
pro_error pro_to_string(pro_state_ref s,
    pro_ref t, PRO_OUT pro_ref* ud)
{
    pro_actor_type type;
    pro_get_actor_type(s, t, &type);
    const pro_actor_type_info* info = pro_get_actor_type_info(s, type);
    pro_object* o = pro_dereference(s, t);
    *ud = info->to_string(s, t, o->value.actor.data);
    return PRO_OK;
}


