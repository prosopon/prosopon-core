#include "test.h"


int main(int argc, char** argv)
{
    CU_SuiteInfo suites[] = {
        pro_state_test_suite,
        pro_env_test_suite,
        pro_constructor_test_suite,
        pro_message_test_suite,
        pro_user_data_test_suite,
        CU_SUITE_INFO_NULL
    };
    
    // initialize the CUnit test registry
    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    // add suites to the registry
    if (CUE_SUCCESS != CU_register_suites(suites))
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    // run all tests
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}