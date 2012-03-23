#include "CUnit/Basic.h"


static CU_SuiteInfo suites[] = {
    CU_SUITE_INFO_NULL
};


int main(int argc, char** argv)
{
    // initialize the CUnit test registry
    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    // add suites to the registry
    if (CUE_SUCCESS != CU_register_suites(suites))
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    // Run all tests using the CUnit Basic interface
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}