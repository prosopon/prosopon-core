#ifndef prosopon_h
#define prosopon_h

#ifdef __cplusplus
extern "C" {
#endif

#include "prosopon_config.h"

#include <stddef.h>


/**
 * @mainpage
 *
 * @author Matt Bierner
 * @version 0.1.0
 *
 * @section intro_sec Introduction
 */ 

/**
 * Marks a parameter as output. 
 */
#define PRO_OUT



#pragma mark Error

typedef enum
{
    PRO_OK = 0,
    PRO_OUT_OF_MEMORY,
    PRO_LIBRARY_LOAD_ERROR,
    PRO_INVALID_ARGUMENT,
    PRO_INVALID_OPERATION,
    PRO_INVALID_VALUE,
    PRO_CONSTRUCTOR_ERROR,
    PRO_BEHAVIOR_ERROR,
    PRO_ERROR_MAX
} pro_error;


/**
 * An opaque reference to state of the prosopon machine.
 */
typedef struct pro_state* pro_state_ref;

/**
 * An opaque reference to an environment.
 */
typedef struct pro_env_lookup* pro_env_ref; 

extern pro_env_ref PRO_EMPTY_ENV_REF;

/**
 * An opaque reference to an object.
 */
typedef struct pro_lookup* pro_ref;

extern pro_ref PRO_EMPTY_REF;


/**
 * Function representing the behavior of an actor.
 * 
 * @param t A lookup for self.
 * @param data Additional user defined data passed to the behavior.
 */
typedef void(pro_behavior)(pro_state_ref,
    pro_ref t, pro_ref msg, pro_ref ud);

/**
 * Function implementing a constructor behavior
 *
 * @return A lookup to the constructed object or null if none.
 */
typedef pro_ref(pro_constructor)(pro_state_ref,
    pro_ref arguments, pro_ref ud);


#pragma mark Types

/**
 * The basic type of an object.
 */
typedef enum
{
    PRO_NONE_TYPE = 0,
    PRO_LIST_TYPE,
    PRO_ACTOR_TYPE,
    PRO_CONSTRUCTOR_TYPE,
    PRO_UD_TYPE,
    PRO_TYPE_MAX
} pro_type;

/**
 *
 */
typedef const char* pro_actor_type;
extern pro_actor_type PRO_DEFAULT_ACTOR_TYPE;


#pragma mark State

/**
 * Function to allocate and deallocate memory
 * 
 * Based on lua_Alloc
 */
typedef void*(pro_alloc)(void* ptr, size_t nsize);

/**
 * Creates a new execution state.
 *
 * @param[out] state The newly created state.
 *
 * @return
 *   PRO_OK if successful
 *   PRO_OUT_OF_MEMORY if a new state cannot be allocated.
 */
PRO_API pro_error (pro_state_create) (pro_alloc* alloc, PRO_OUT pro_state_ref* state);

/**
 * Releases a state for future collection.
 *
 * @return
 *   PRO_OK if successful
 *   PRO_INVALID_OPERATION if the state is not valid.
 */
PRO_API pro_error (pro_state_release) (pro_state_ref);

/**
 * 
 */
PRO_API pro_error (pro_run) (pro_state_ref);

/**
 * Gets the allocator used for a given state.
 *
 * @param[out] alloc The memory allocator
 *
 * @return
 */
PRO_API pro_error (pro_get_alloc) (pro_state_ref, PRO_OUT pro_alloc** alloc);

/**
 * @param[out] env The reference to the current environment. 
 *
 * @return
 *   PRO_OK if successful
 *   PRO_INVALID_OPERATION if the state is not valid.
 */
PRO_API pro_error (pro_get_env) (pro_state_ref, PRO_OUT pro_env_ref* env);

/**
 * Pushes an environment onto the environment stack
 *
 * @return
 *   PRO_OK if successful
 *   PRO_INVALID_OPERATION if state is not valid or pushing an env onto itself.
 */
PRO_API pro_error (pro_push_env) (pro_state_ref, pro_env_ref);

/**
 * Pops an environment off the environment stack.
 *
 * @return
 *   PRO_OK if successful
 *   PRO_INVALID_OPERATION if state is not valid or popping the root environment.
 */
PRO_API pro_error (pro_pop_env) (pro_state_ref);


#pragma mark Environment

/**
 * Creates a new environment with a given parent.
 * 
 * @param[out] env A reference to the new environment.
 *
 * @return
 *   PRO_OK if successful
 *   PRO_OUT_OF_MEMORY if memory cannot be alocated.
 */
PRO_API pro_error (pro_env_create) (pro_state_ref, pro_env_ref parent,
    PRO_OUT pro_env_ref* env);

/**
 */
PRO_API pro_error (pro_env_retain) (pro_state_ref, pro_env_ref);

/**
 * Release an environment for future collection.
 */
PRO_API pro_error (pro_env_release) (pro_state_ref, pro_env_ref);

/**
 */
PRO_API pro_error (pro_retain) (pro_state_ref, pro_ref);

/**
 * Release an referenced object for future collection.
 */
PRO_API pro_error (pro_release) (pro_state_ref, pro_ref);

/**
 * @param[out] ref The highest resolved reference or PRO_EMPTY_REF if none.
 *
 * @return
 *   PRO_OK if successful
 */
PRO_API pro_error (pro_get_binding) (pro_state_ref,
    pro_env_ref env, const char* name, PRO_OUT pro_ref* ref);

/**
 * @param[out] type The type value of a lookup.
 * 
 * @return 
 */
PRO_API pro_error (pro_get_type) (pro_state_ref,
    pro_ref lookup, PRO_OUT pro_type* type);

/**
 * Binds a lookup to an identifier name.
 *
 * @return
 *   PRO_OK if successful
 *   PRO_INVALID_OPERATION if the state is not valid or id is null.
 *   PRO_OUT_OF_MEMORY if memory cannot be allocated.
 *   PRO_INVALID_ARGUMENT If ref is invalid.
 */
PRO_API pro_error (pro_bind) (pro_state_ref, pro_ref ref, const char* id);

typedef enum 
{
    PRO_MATCH_FAIL = 0,
    PRO_MATCH_SUCCEED,
    PRO_MATCH_CONTINUE
} pro_matching;

/**
 * Attempts to match two identifiers.
 *
 * @param[out] match One if the two items match else zero.
 *
 * @return
 *   PRO_OK if successful
 *   PRO_INVALID_OPERATION if the state is not valid or id is null.
 */
PRO_API pro_error (pro_match) (pro_state_ref, pro_ref, pro_ref, PRO_OUT pro_matching* match);

/**
 * 
 */
PRO_API char* (pro_to_string)(pro_state_ref,
    pro_ref);


#pragma mark Constructor

/**
 * Creates a new constructor in the current environment.
 *
 * @param impl A function that constructs an object and returns a referance.
 *   Must not be null.
 * @param ud User data that is passed to the constructor function. May be null.
 * 
 * @param[out] out_ref A reference to the new constructor.
 *
 * @return
 *   PRO_OK if successful
 *   PRO_INVALID_OPERATION if the state is not valid or id is null.
 *   PRO_INVALID_ARGUMENT if impl is null.
 */
PRO_API pro_error (pro_constructor_create) (pro_state_ref,
    pro_constructor* impl, pro_ref ud, PRO_OUT pro_ref* out_ref);

/**
 * Calls a constructor with a list of arguments.
 * 
 * @param constructor The reference to the constructor to call.
 * @param arguments A list of lookups to pass to the constructor.
 *
 * @param[out] result The result from the constructor.
 *
 * @return
 *   PRO_OK if successful
 *   PRO_INVALID_OPERATION if the state is not valid.
 *   PRO_INVALID_ARGUMENT if constructor does reference a constructor.
 *   PRO_CONSTRUCTOR_ERROR if the constructor call fails.
 */
PRO_API pro_error (pro_constructor_call) (pro_state_ref,
    pro_ref constructor, pro_ref arguments, PRO_OUT pro_ref* result);


#pragma mark List

/**
 * Creates a new list in the current environment.
 *
 * @param[out] msg A referance to the new list.
 *
 * @return
 *   PRO_OK if successful
 *   PRO_INVALID_OPERATION if the state is not valid.
 *   PRO_OUT_OF_MEMORY if memory allocation fails.
 */
PRO_API pro_error (pro_list_create) (pro_state_ref, PRO_OUT pro_ref* msg);

/**
 * Get the number of objects in a list.
 *
 * @param[out] length The number of objects a list contains.
 *
 * @return
 *   PRO_OK if successful
 *   PRO_INVALID_OPERATION if the state is not valid.
 *   PRO_INVALID_ARGUMENT if msg does not reference a list.
 */
PRO_API pro_error (pro_list_length) (pro_state_ref, pro_ref msg,
    PRO_OUT unsigned int* length);

/**
 * Get a value from a list.
 *
 * @param[out] result Referance at index or PRO_EMPTY_REF if out of bounds.
 *
 * @return
 *   PRO_OK if successful
 *   PRO_INVALID_OPERATION if the state is not valid.
 *   PRO_INVALID_ARGUMENT if msg does not reference a list.
 */
PRO_API pro_error (pro_list_get) (pro_state_ref,
    pro_ref msg, unsigned int index, PRO_OUT pro_ref* result);

/**
 * Appends an value to a list to create a new list.
 *
 * Does not modify the origin list.
 *
 * @return
 *   PRO_OK if successful
 *   PRO_INVALID_OPERATION if the state is not valid.
 *   PRO_INVALID_ARGUMENT if msg does not reference a list or value does
 *     not reference a valid object.
 */
PRO_API pro_error (pro_list_append) (pro_state_ref,
    pro_ref msg, pro_ref value, PRO_OUT pro_ref* new_msg);


#pragma mark User Data

/**
 * Function called before a user data object is destroyed.
 *
 * @param data A pointer to the allocated data.
 */
typedef void(pro_ud_deconstructor) (pro_state_ref, void* data);

extern pro_ud_deconstructor* PRO_DEFAULT_UD_DECONSTRUCTOR;

/**
 * Creates a new user data object with a given size and deconstructor.
 *
 * @param[out] out_ref A reference to the new user data object.
 */
PRO_API pro_error (pro_ud_create) (pro_state_ref,
    size_t size, pro_ud_deconstructor* deconstructor, PRO_OUT pro_ref* out_ref);

/**
 * Requests access to user data data for reading.
 */
PRO_API pro_error (pro_ud_read) (pro_state_ref, pro_ref, PRO_OUT const void**);

/**
 * Requests access to user data data for writting.
 */
PRO_API pro_error (pro_ud_write) (pro_state_ref, pro_ref, PRO_OUT void**);


#pragma mark Actor

/**
 * Create a new actor object in the current environment.
 *
 * @return The lookup for the new actor.
 */
PRO_API pro_error (pro_actor_create) (pro_state_ref, pro_actor_type type,
    pro_behavior beh, pro_ref data, PRO_OUT pro_ref*);

/**
 * @param[out] type The actor type value of a lookup.
 */
PRO_API pro_error (pro_get_actor_type) (pro_state_ref, pro_ref,
    PRO_OUT pro_actor_type*);

PRO_API pro_error pro_actor_request_ud(pro_state_ref s,
    pro_ref actor, PRO_OUT pro_ref* ud);

/**
 * Sends a list to an actor.
 */
PRO_API pro_error (pro_send) (pro_state_ref, pro_ref actor, pro_ref msg);

/**
 * Specify the behavior for an actor.
 */
PRO_API pro_error (pro_become) (pro_state_ref, pro_ref actor, pro_ref new_beh);


#pragma mark Library Loading

#ifdef PRO_DYNAMIC_LIBRARY_LOADING

typedef void(pro_library_init)(pro_state_ref);

/** 
 *
 */
PRO_API pro_error (pro_library_load) (pro_state_ref, const char* file);

#endif



#ifdef __cplusplus
}
#endif

#endif
