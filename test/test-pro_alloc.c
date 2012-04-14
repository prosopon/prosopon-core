#include "test-pro_alloc.h"

#include <stdlib.h>



void* test_alloc(void* ptr, size_t nsize)
{
    if (0 == nsize)
    {
        free(ptr);
        return 0;
    }
    else
        return realloc(ptr, nsize);
}


void* test_bad_alloc(void* ptd, size_t nsize)
{
    return 0; 
}
