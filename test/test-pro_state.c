#include "test-pro_env.h"

#include "prosopon.h"
#include "pro_state.h"


static pro_state_ref state = 0;


static int init(void)
{
    return 0;
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
    pro_state_create(&state);
    CU_ASSERT(0 != state);
    CU_ASSERT(0 != state->root_env);
    CU_ASSERT(0 != state->stack);
    CU_ASSERT(pro_env_lookup_equal(state, state->root_env, state->stack->value));
}


static void test_get_env(void)
{
    pro_env_ref current;
    pro_get_env(state, &current);
    CU_ASSERT(current->value == state->stack->value->value);
}


static void test_push_pop(void)
{
    pro_env_ref old;
    pro_get_env(state, &old);
    pro_env_ref new;
    pro_env_create(state, old, &new);
    pro_push_env(state, new);
    CU_ASSERT(new->value == state->stack->value->value);
    CU_ASSERT(old->value == state->stack->next->value->value);
    
    pro_pop_env(state);
    CU_ASSERT(old->value == state->stack->value->value);
}

static void test_pop_root(void)
{
    pro_error err = pro_pop_env(state);
    CU_ASSERT(PRO_INVALID_OPERATION == err);
}

static void test_push_self_on_self(void)
{
    pro_env_ref old;
    pro_get_env(state, &old);
    pro_env_ref new;
    pro_env_create(state, old, &new);
    pro_error err = pro_push_env(state, new);
    CU_ASSERT(PRO_OK == err);
    err = pro_push_env(state, new);
    CU_ASSERT(PRO_INVALID_OPERATION == err);
}


static CU_TestInfo tests[] = {
    {"create", test_create},
    {"get env", test_get_env},
    {"push and pop", test_push_pop},
    {"pop_root", test_pop_root},
    {"push_self_on_self", test_push_self_on_self},
    CU_TEST_INFO_NULL,
};


CU_SuiteInfo pro_state_test_suite = {
    "pro_state",
    init,
    cleanup,
    tests
};
