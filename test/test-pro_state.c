#include "test-pro_env.h"

#include "prosopon.h"
#include "pro_state.h"


static pro_state* state = 0;


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


static void test_create(void)
{
    pro_state* s = pro_state_create();
    CU_ASSERT(0 != s);
    CU_ASSERT(0 != s->root_env);
    CU_ASSERT(0 != s->current_env);
    CU_ASSERT(s->root_env == s->current_env);
}


static void test_get_env(void)
{
    pro_env* current = pro_get_env(state);
    CU_ASSERT(current == state->current_env);
}


static void test_push_pop(void)
{
    pro_env* old = pro_get_env(state);
    pro_env* new = pro_env_create(state, old);
    pro_push_env(state, new);
    CU_ASSERT(old == new->previous);
    
    pro_pop_env(state);
    CU_ASSERT(pro_get_env(state) == old);
}


static CU_TestInfo tests[] = {
    {"create", test_create},
    {"get env", test_get_env},
    {"push and pop", test_push_pop},

    CU_TEST_INFO_NULL,
};


CU_SuiteInfo pro_state_test_suite = {
    "pro_state",
    init,
    cleanup,
    tests
};
