#include "pro_library.h"

#include "prosopon/core.h"
#include "pro_state.h"

#include <dlfcn.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>


#pragma mark Private

static void initialize_library(pro_state_ref s, void* lib_handle)
{
    assert(lib_handle);
    pro_library_init* init = dlsym(lib_handle, "prosopon_library_initialization");
    if (!init)
    {
        fprintf(stderr, "%s\n", dlerror());
        exit(EXIT_FAILURE);
    }
    init(s);
}

static void add_loaded_library(pro_state_ref s, const char* file)
{
    pro_library_list* libraries = pro_state_get_libraries(s);
    pro_state_set_libraries(s, pro_library_list_new(s, file, libraries));
}

#pragma mark -
#pragma mark Internal

PRO_INTERNAL
pro_library_list* pro_library_list_new(pro_state_ref s,
    const char* file, pro_library_list* next)
{
    pro_alloc* alloc;
    pro_get_alloc(s, &alloc);
    pro_library_list* t = alloc(0, sizeof(*t));
    if (!t) return 0;
    
    t->next = next;
    t->file = file;
    return t;
}


PRO_INTERNAL
int pro_library_loaded(pro_state_ref s, const char* file)
{
    for (pro_library_list* library = pro_state_get_libraries(s); library; library = library->next)
        if (strcmp(file, library->file) == 0)
            return 1;
    return 0;
}

PRO_INTERNAL
void pro_library_list_free(pro_state_ref s, pro_library_list* libraries)
{
    pro_alloc* alloc;
    pro_get_alloc(s, &alloc);
    
    for (pro_library_list* lib = libraries; lib; )
    {
        pro_library_list* next = lib->next;
        alloc(lib, 0);
        lib = next;
    }
}



#pragma mark PRO_API

PRO_API
pro_error pro_library_load(pro_state_ref s, const char* file)
{
    if (pro_library_loaded(s, file) != 0)
        return PRO_OK;
    
    void* handle = dlopen(file, RTLD_LOCAL | RTLD_LAZY);
    if (handle)
    {
        initialize_library(s, handle);
        add_loaded_library(s, file);
    }
    else
        return PRO_LIBRARY_LOAD_ERROR;
    
    return PRO_OK;
}
