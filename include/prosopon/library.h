#ifndef prosopon_core_library_h
#define prosopon_core_library_h

#ifdef __cplusplus
extern "C" {
#endif


typedef void(pro_library_init)(pro_state_ref);

/** 
 *
 */
PRO_API
pro_error (pro_library_load) (pro_state_ref, const char* file);


#ifdef __cplusplus
}
#endif

#endif
