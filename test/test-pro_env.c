#include "test-pro_env.h"

#include "prosopon.h"

#include "pro_state.h"
#include "pro_env.h"


static pro_state_ref state;


static int init(void)
{
    pro_state_create(&state);
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
    pro_env* env1 = pro_env_new(state, 0);
    CU_ASSERT(0 != env1);
    CU_ASSERT(0 == env1->parent);

    pro_env* env2 = pro_env_new(state, pro_env_lookup_new(state, env1));
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
    pro_env_create(state, old, &new);
    CU_ASSERT(0 != new);
    CU_ASSERT(new->value->parent == old);
}


static CU_TestInfo tests[] = {
    {"new", test_new},
    {"create", test_create},
    {"create_invalid", test_create_invalid},

    CU_TEST_INFO_NULL,
};


CU_SuiteInfo pro_env_test_suite = {
    "pro_env",
    init,
    cleanup,
    tests
};
