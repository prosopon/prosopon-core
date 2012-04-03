#include "pro_library.h"

#include "prosopon.h"
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
    pro_library_list* t = pro_library_list_new(file, 0);
    if (!s->libraries)
        s->libraries = t;
    else
    {
        pro_library_list* parent = s->libraries;
        while (parent->next)
            parent = parent->next;
        parent->next = t;
    }
}

#pragma mark Internal

PRO_INTERNAL pro_library_list* pro_library_list_new(const char* file,
    pro_library_list* next)
{
    pro_library_list* t = malloc(sizeof(*t));
    t->next = next;
    t->file = file;
    return t;
}


PRO_INTERNAL int pro_library_loaded(pro_state_ref s, const char* file)
{
    pro_library_list* library = s->libraries;
    while (library)
    {
        if (strcmp(file, library->file) == 0)
            return 1;
        library = library->next;
    }
    return 0;
}


#pragma mark PRO_API

PRO_API void pro_library_load(pro_state_ref s, const char* file)
{
    if (pro_library_loaded(s, file) != 0)
        return;
    
    void* handle = dlopen(file, RTLD_LOCAL | RTLD_LAZY);
    if (handle)
    {
        initialize_library(s, handle);
        add_loaded_library(s, file);
    }
}
