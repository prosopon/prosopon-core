#include "test-pro_env.h"

#include "prosopon.h"
#include "pro_state.h"


static pro_state_ref state = 0;


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

#pragma mark Private

static int global = 0;

static pro_ref contructor(pro_state_ref s, pro_ref_list arguments, void* d)
{
    int* val = d;
    global = *val;
    return pro_actor_create(state);
}



#pragma mark -
#pragma mark Tests


static void test_create(void)
{
    /*pro_env_lookup* env = pro_env_create(state, pro_get_env(state));
    pro_push_env(s, env);

    const int old_global = global;

    const int val = 5;
    pro_lookup* c = pro_constructor_create(state, &val);
    pro_constructor_call(s, c, 0);
    
    CU_ASSERT(old_global + 1 == global);
    pro_pop_env();*/
}

static CU_TestInfo tests[] = {
    {"create", test_create},
    CU_TEST_INFO_NULL,
};


CU_SuiteInfo pro_constructor_test_suite = {
    "pro_constructor",
    init,
    cleanup,
    tests
};
