#include "test.h"

#include "prosopon.h"
#include "pro_state.h"
#include "pro_message.h"

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


#pragma mark -
#pragma mark Tests


static void test_create(void)
{
    pro_ref msg;
    pro_message_create(state, &msg);
    CU_ASSERT(PRO_EMPTY_REF != msg);
    
    pro_type type;
    pro_get_type(state, msg, &type);
    CU_ASSERT(PRO_MESSAGE_TYPE == type);
    
    unsigned int len;
    pro_message_length(state, msg, &len);
    CU_ASSERT(0 == len);
}

static void test_create_invalid(void)
{
    pro_ref msg;
    pro_error err = pro_message_create(state, &msg);
    CU_ASSERT(PRO_INVALID_OPERATION == err);
}

static void test_append(void)
{
    pro_ref msg;
    pro_message_create(state, &msg);
    
    
}


static CU_TestInfo tests[] = {
    {"create", test_create},
    {"create_invalid", test_create_invalid},
    {"append", test_append},
    CU_TEST_INFO_NULL,
};


CU_SuiteInfo pro_message_test_suite = {
    "pro_message",
    init,
    cleanup,
    tests
};
