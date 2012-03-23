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


static CU_TestInfo tests[] = {
    {"create", test_create},
    
    CU_TEST_INFO_NULL,
};


CU_SuiteInfo pro_state_test_suite = {
    "pro_state",
    init,
    cleanup,
    tests
};
