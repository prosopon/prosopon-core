#ifndef prosopon_config_h
#define prosopon_config_h


#if __llvm__ || __GNUC__ >= 4
    /**
     * Marks a symbol as part of the public prosopon API. 
     */
    #define PRO_API	extern __attribute__ ((visibility ("default")))
   
    /**
     * Marks a data symbol as part of the public prosopon API. 
     */
    #define PRO_API_DATA extern __attribute__ ((visibility ("default")))

    /**
     * Marks a symbol as part of the public prosopon core library API. 
     */
    #define PRO_LIBCORE extern __attribute__ ((visibility ("default")))

    /**
     * Marks a symbol as part of the public prosopon interpreter library API. 
     */
    #define PRO_INTERPRETER extern __attribute__ ((visibility ("default")))

    /**
     * Marks a symbol as internal, not intended for public use.
     */
    #define PRO_INTERNAL extern __attribute__ ((visibility ("hidden")))
#else
    #define PRO_API	extern
    #define PRO_LIBCORE extern
    #define PRO_INTERPRETER extern
    #define PRO_INTERNAL
extern
#endif


#endif
