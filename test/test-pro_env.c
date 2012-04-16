#include "test.h"

#include "prosopon.h"

#include "pro_state.h"
#include "pro_env.h"
#include "pro_lookup.h"
#include "pro_env_lookup.h"

#include "test-pro_alloc.h"


static pro_state_ref state;


static int init(void)
{
    pro_state_create(test_alloc, &state);
    return state == 0;
}

static int cleanup(void)
{
    pro_state_release(state);
    state = 0;
    return 0;
}


#pragma mark -
#pragma mark Tests

static void test_new(void)
{
    pro_env* env1 = pro_env_new(state, 0, 1);
    CU_ASSERT(0 != env1);
    CU_ASSERT(0 == env1->parent);
    
    pro_internal_env_retain(state, env1);
    pro_env* env2 = pro_env_new(state, pro_env_lookup_new(state, env1, 1), 1);
    CU_ASSERT(0 != env2);
    CU_ASSERT(env1 == env2->parent->value);
}


static void test_create(void)
{
    pro_env_ref old;
    pro_get_env(state, &old);
    
    pro_env_ref new;
    pro_env_create(state, old, &new);
    CU_ASSERT(0 != new);
    CU_ASSERT(new->value->parent == old);
}


static void test_create_invalid(void)
{
    pro_env_ref old;
    pro_get_env(state, &old);
    
    pro_env_ref new;
    pro_error err = pro_env_create(0, old, &new);
    CU_ASSERT(PRO_INVALID_OPERATION == err);
}


static void test_bind(void)
{
    pro_env_ref old;
    pro_get_env(state, &old);
    pro_env_ref new;
    pro_env_create(state, old, &new);
    pro_push_env(state, new);
    
    pro_ref ref;
    pro_actor_create(state, PRO_DEFAULT_ACTOR_TYPE, 0, PRO_EMPTY_REF, &ref);
    pro_bind(state, ref, "id");
    
    pro_ref found;
    pro_get_binding(state, new, "id", &found);
    CU_ASSERT(found->env == ref->env && found->index == ref->index);
    
    pro_error err = pro_get_binding(0, new, "id", &found);
    CU_ASSERT(PRO_INVALID_OPERATION == err);
    
    pro_pop_env(state);
}

static void test_bind_parent(void)
{
    // create env parent.
    pro_env_ref old;
    pro_get_env(state, &old);
    pro_env_ref parent;
    pro_env_create(state, old, &parent);
    pro_push_env(state, parent);
    
    // setup an actor in env parent bound to id
    pro_ref ref;
    pro_actor_create(state, PRO_DEFAULT_ACTOR_TYPE, 0, PRO_EMPTY_REF, &ref);
    pro_bind(state, ref, "id");
    
    // Push a env top onto the stack with parent parent 
    pro_env_ref top;
    pro_env_create(state, parent, &top);
    pro_push_env(state, top);
    
    // try getting the binding in the env top
    pro_ref found;
    pro_get_binding(state, top, "id", &found);
    CU_ASSERT(found->env == ref->env && found->index == ref->index);
    
    pro_pop_env(state);
    pro_pop_env(state);
}

static void test_bind_hide(void)
{
    // create env parent.
    pro_env_ref old;
    pro_get_env(state, &old);
    pro_env_ref parent;
    pro_env_create(state, old, &parent);
    pro_push_env(state, parent);
    
    // setup an actor in env parent bound to id
    pro_ref ref;
    pro_actor_create(state, PRO_DEFAULT_ACTOR_TYPE, 0, PRO_EMPTY_REF, &ref);
    pro_bind(state, ref, "id");
    
    // Push a env top onto the stack with parent parent 
    pro_env_ref top;
    pro_env_create(state, parent, &top);
    pro_push_env(state, top);
    
    // setup an actor in top env parent bound to id
    pro_ref top_ref;
    pro_actor_create(state, PRO_DEFAULT_ACTOR_TYPE, 0, PRO_EMPTY_REF, &top_ref);
    pro_bind(state, top_ref, "id");
    
    // try getting the binding in the env top
    pro_ref found;
    pro_get_binding(state, top, "id", &found);
    CU_ASSERT(found->env == top_ref->env && found->index == top_ref->index);
    
    pro_pop_env(state);
    pro_pop_env(state);
}


static CU_TestInfo tests[] = {
    {"new", test_new},
    {"create", test_create},
    {"create_invalid", test_create_invalid},
    {"bind", test_bind},
    {"bind_parent", test_bind_parent},
    {"bind_hide", test_bind_hide},
    CU_TEST_INFO_NULL,
};


CU_SuiteInfo pro_env_test_suite = {
    "pro_env",
    init,
    cleanup,
    tests
};
