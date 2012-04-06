#include "test-pro_env.h"

#include "prosopon.h"
#include "pro_state.h"

#include <stdlib.h>


static pro_state_ref state = 0;


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

#pragma mark Private

static int global = 0;

static pro_ref sample_contructor(pro_state_ref s, pro_ref_list arguments, void* d)
{
    int* val = d;
    global = *val;
    pro_ref actor;
    pro_actor_create(state, PRO_DEFAULT_ACTOR_TYPE, &actor);
    return actor;
}



#pragma mark -
#pragma mark Tests


static void test_create(void)
{
    pro_env_ref parent;
    pro_get_env(state, &parent);
    pro_env_ref env;
    pro_env_create(state, parent, &env);
    pro_push_env(state, env);

    const int val = 5;
    pro_constructor constructor = {
        .impl = sample_contructor,
        .data = malloc(sizeof(val))
    };
    *((int*)constructor.data) = val;
    
    pro_ref c;
    pro_constructor_create(state, constructor, &c);
    pro_ref_list list;
    
    pro_type type;
    pro_get_type(state, c, &type);
    CU_ASSERT(PRO_CONSTRUCTOR_TYPE == type);
    
    // call the constructor
    pro_ref result;
    pro_constructor_call(state, c, list, &result);
    
    CU_ASSERT(global == val);
    pro_pop_env(state);
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
