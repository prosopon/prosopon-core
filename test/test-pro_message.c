#include "test.h"

#include "prosopon.h"
#include "pro_state.h"
#include "pro_lookup.h"
#include "test-pro_alloc.h"

#include <stdlib.h>


static pro_state_ref state = 0;


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


static void test_create(void)
{
    pro_ref msg;
    pro_list_create(state, &msg);
    CU_ASSERT(PRO_EMPTY_REF != msg);
    
    pro_type type;
    pro_get_type(state, msg, &type);
    CU_ASSERT(PRO_LIST_TYPE == type);
    
    unsigned int len;
    pro_list_length(state, msg, &len);
    CU_ASSERT(0 == len);
}

static void test_create_invalid(void)
{
    pro_ref msg;
    pro_error err = pro_list_create(0, &msg);
    CU_ASSERT(PRO_INVALID_OPERATION == err);
}

static void test_append(void)
{
    pro_ref msg;
    pro_list_create(state, &msg);
    
    pro_ref actor1, actor2;
    pro_actor_create(state, PRO_DEFAULT_ACTOR_TYPE, 0, PRO_EMPTY_REF, &actor1);
    pro_actor_create(state, PRO_DEFAULT_ACTOR_TYPE, 0, PRO_EMPTY_REF, &actor2);

    pro_list_append(state, msg, actor1, &msg);
    pro_list_append(state, msg, actor2, &msg);

    unsigned int len;
    pro_list_length(state, msg, &len);
    CU_ASSERT(2 == len);
    
    pro_ref first, second;
    pro_list_get(state, msg, 0, &first);
    pro_list_get(state, msg, 1, &second);
    
    CU_ASSERT(pro_lookup_equal(state, first, actor1));
    CU_ASSERT(pro_lookup_equal(state, second, actor2));
}

static void test_append_invalid(void)
{
    pro_error err;
    pro_ref msg;
    pro_list_create(state, &msg);
    
    pro_ref actor;
    pro_actor_create(state, PRO_DEFAULT_ACTOR_TYPE, 0, PRO_EMPTY_REF, &actor);

    // bad state
    err = pro_list_append(0, msg, actor, &msg);
    CU_ASSERT(PRO_INVALID_OPERATION == err);
    
    // bad value
    err = pro_list_append(state, msg, PRO_EMPTY_REF, &msg);
    CU_ASSERT(PRO_INVALID_ARGUMENT == err);
    
    // bad msg
    err = pro_list_append(state, PRO_EMPTY_REF, actor, &msg);
    CU_ASSERT(PRO_INVALID_ARGUMENT == err);
}

static void test_length_invalid(void)
{
    pro_error err;
    pro_ref msg;
    pro_list_create(state, &msg);
    
    unsigned int len;
    
    // bad state
    err = pro_list_length(0, msg, &len);
    CU_ASSERT(PRO_INVALID_OPERATION == err);
    
    // bad msg
    err = pro_list_length(state, PRO_EMPTY_REF, &len);
    CU_ASSERT(PRO_INVALID_ARGUMENT == err);
}

static void test_get_invalid(void)
{
    pro_error err;
    pro_ref msg;
    pro_list_create(state, &msg);
    
    pro_ref result;
    
    // bad state
    err = pro_list_get(0, msg, 0, &result);
    CU_ASSERT(PRO_INVALID_OPERATION == err);
    
    // bad msg
    err = pro_list_get(state, PRO_EMPTY_REF, 0, &result);
    CU_ASSERT(PRO_INVALID_ARGUMENT == err);
}

static void test_get_out_of_bounds(void)
{
    pro_ref msg;
    pro_list_create(state, &msg);
    
    pro_ref actor;
    pro_actor_create(state, PRO_DEFAULT_ACTOR_TYPE, 0, PRO_EMPTY_REF, &actor);

    pro_ref get;
    pro_list_get(state, msg, 1, &get);
    CU_ASSERT(PRO_EMPTY_REF == get);
}



static CU_TestInfo tests[] = {
    {"create", test_create},
    {"create_invalid", test_create_invalid},
    {"append", test_append},
    {"append_invalid", test_append_invalid},
    {"length_invalid", test_length_invalid},
    {"get_invalid", test_get_invalid},
    {"get_out_of_bounds", test_get_out_of_bounds},
    CU_TEST_INFO_NULL,
};


CU_SuiteInfo pro_message_test_suite = {
    "pro_message",
    init,
    cleanup,
    tests
};
