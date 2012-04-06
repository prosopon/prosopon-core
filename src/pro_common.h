#ifndef prosopon_pro_common_h
#define prosopon_pro_common_h

#include "prosopon.h"

#include <assert.h>


#ifdef DEBUG
    #define PRO_API_ASSERT(cond, err) if (!(cond)){ assert(0); return (err); }
#else 
    #define PRO_API_ASSERT(cond, err) if (!(cond)) { return (err); } 
#endif

#define PRO_API_ASSERT_TYPE(ref, type, err) {\
        pro_type found_type;\
        pro_error get_err = pro_get_type(s, ref, &found_type);\
        PRO_API_ASSERT(get_err == PRO_OK, (err));\
        PRO_API_ASSERT(found_type == (type), (err));\
    }   

#endif
