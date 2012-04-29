#include "test.h"

#include "prosopon/core.h"
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


typedef struct 
{
    int val;
} sample_ud;

#pragma mark -
#pragma mark Tests


static void test_create(void)
{
    sample_ud* ud_data;
    
    pro_ref ud;
    pro_ud_create(state, sizeof(*ud_data), PRO_DEFAULT_UD_DECONSTRUCTOR, &ud);
    CU_ASSERT(PRO_EMPTY_REF != ud);
    
    // test write
    pro_ud_write(state, ud, (void**)&ud_data);
    ud_data->val = 10;
    
    // test read
    sample_ud* read_ud_data;
    pro_ud_read(state, ud, (const void**)&read_ud_data);
    CU_ASSERT(read_ud_data->val == 10);
}

static CU_TestInfo tests[] = {
    {"create", test_create},
    CU_TEST_INFO_NULL,
};


CU_SuiteInfo pro_user_data_test_suite = {
    "pro_message",
    init,
    cleanup,
    tests
};
