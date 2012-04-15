#include "pro_object.h"

#include "pro_type.h"
#include "pro_env.h"
#include "pro_object.h"
#include "pro_common.h"
#include "pro_lookup.h"


PRO_INTERNAL pro_object* pro_object_new(pro_state_ref s,
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

PRO_INTERNAL pro_object* pro_object_retain(pro_state_ref s, pro_object* t)
{
    t->ref_count++;
    return t;
}


PRO_INTERNAL void pro_object_release(pro_state_ref s, pro_object* t)
{
    if (--(t->ref_count) <= 0)
    {
        pro_alloc* alloc;
        pro_get_alloc(s, &alloc);
           
        switch (t->type)
        {
        case PRO_ACTOR_TYPE:
            pro_release(s, t->value.actor.data);
            pro_env_release(s, t->value.actor.env);
            break;
        case PRO_MESSAGE_TYPE:
            for (pro_ref_list msg = t->value.message; msg;)
            {
                pro_release(s, msg->value);
                pro_ref_list old = msg;
                msg = msg->next;
                alloc(old, 0);
            }
            break;
        }

        alloc(t, 0);
    }
}



#pragma mark -
#pragma mark Public

PRO_API pro_error pro_retain(pro_state_ref s, pro_ref ref)
{
    if (PRO_EMPTY_REF != ref)
        ref->ref_count++;
    return PRO_OK;
}


PRO_API pro_error pro_release(pro_state_ref s, pro_ref ref)
{
    if (PRO_EMPTY_REF == ref)
        return PRO_OK;
    
    if (--(ref->ref_count) <= 0)
    {    
        pro_alloc* alloc;
        pro_get_alloc(s, &alloc);
        
        pro_object* obj = pro_dereference(s, ref);
        pro_object_release(s, obj);

        alloc(ref, 0);
    }
    
    return PRO_OK;
}


PRO_API pro_error pro_get_type(pro_state_ref s, pro_ref ref,
    PRO_OUT pro_type* type)
{
    PRO_API_ASSERT(s, PRO_INVALID_OPERATION);
    
    if (PRO_EMPTY_REF == ref)
        return PRO_INVALID_ARGUMENT;
    
    pro_object* obj = pro_dereference(s, ref);
    *type = obj->type;
    return PRO_OK;
}


PRO_API pro_error pro_match(pro_state_ref s, pro_ref l1, pro_ref l2, PRO_OUT pro_matching* out)
{
    PRO_API_ASSERT(s, PRO_INVALID_OPERATION);

    if (pro_lookup_equal(s, l1, l2))
    {
        *out = 1;
        return PRO_OK;
    }
        
    pro_actor_type type;
    pro_get_actor_type(s, l1, &type);
    const pro_actor_type_info* info = pro_get_actor_type_info(s, type);
    pro_object* o1 = pro_dereference(s, l1);
    *out = info->match(s, l1, o1->value.actor.data,l2);
    return PRO_OK;
}


PRO_API char* pro_to_string(pro_state_ref s,
    pro_ref t)
{
    pro_actor_type type;
    pro_get_actor_type(s, t, &type);
    const pro_actor_type_info* info = pro_get_actor_type_info(s, type);
    pro_object** o = pro_env_lookup_value(s, t);
    return info->to_string(s, t, (*o)->value.actor.data);
}