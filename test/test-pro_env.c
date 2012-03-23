#include "test-pro_env.h"

#include "prosopon.h"

#include "pro_state.h"
#include "pro_env.h"


static pro_state* state;


static int init(void)
{
    state = pro_state_create();
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
    pro_env* env1 = pro_env_new(state, 0, 0);
    CU_ASSERT(0 != env1);
    CU_ASSERT(0 == env1->previous);
    CU_ASSERT(0 == env1->parent);

    pro_env* env2 = pro_env_new(state, env1, env1);
    CU_ASSERT(0 != env2);
    CU_ASSERT(env1 == env2->previous);
    CU_ASSERT(env1 == env2->parent);
    
    pro_env* env3 = pro_env_new(state, env2, env1);
    CU_ASSERT(env2 == env3->previous);
    CU_ASSERT(env1 == env3->parent);
}


static void test_create_and_release(void)
{
    pro_env* old = state->current_env;
    pro_env_create(state);
    pro_env* new = state->current_env;
    CU_ASSERT(0 != new);
    CU_ASSERT(new->parent == old);
    CU_ASSERT(new->previous == old);
    
    pro_env_release(state);
    pro_env* after_release = state->current_env;
    CU_ASSERT(0 != after_release);
    CU_ASSERT(old == after_release);
}


static CU_TestInfo tests[] = {
    {"new", test_new},
    {"create and release", test_create_and_release},
    CU_TEST_INFO_NULL,
};


CU_SuiteInfo pro_env_test_suite = {
    "pro_env",
    init,
    cleanup,
    tests
};
