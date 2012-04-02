#ifndef prosopon_pro_library_h
#define prosopon_pro_library_h

#include "prosopon.h"


typedef struct pro_library_list pro_library_list;
struct pro_library_list
{
    pro_library_list* next;
    const char* file;
};


/**
 * @return A new library list.
 */
PRO_INTERNAL pro_library_list* pro_library_list_new(const char* file,
    pro_library_list* next);

/**
 * Has a library been loaded.
 * @return 0 if false else true
 */
PRO_INTERNAL int pro_library_loaded(pro_state*, const char* file);

#endif
