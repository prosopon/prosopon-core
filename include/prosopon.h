#ifndef prosopon_h
#define prosopon_h

#include "prosopon_config.h"

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
typedef const struct pro_env_lookup* pro_env_ref; 

extern pro_env_ref PRO_EMPTY_ENV_REF;

/**
 * An opaque reference to an object.
 */
typedef const struct pro_lookup* pro_ref;

extern pro_ref PRO_EMPTY_REF;

/**
 * A list of lookup values.
 */
typedef struct pro_lookup_list* pro_ref_list;
struct pro_lookup_list
{
    pro_ref_list next;
    pro_ref value;
};

/**
 * Function representing the behavior of an actor.
 * 
 * @param t A lookup for self.
 * @param data Additional user defined data passed to the behavior.
 */
typedef void(pro_behavior_impl)(pro_state_ref,
    pro_ref t, pro_ref msg, void* data);

typedef struct pro_behavior pro_behavior;
struct pro_behavior
{
    pro_behavior_impl* impl;
    void* data;
};

/**
 * Function implementing a constructor behavior
 *
 * @return A lookup to the constructed object or null if none.
 */
typedef pro_ref(pro_constructor_impl)(pro_state_ref,
    pro_ref_list arguments, void* data);

typedef struct pro_constructor pro_constructor;
struct pro_constructor
{
    pro_constructor_impl* impl;
    void* data;
};

#pragma mark Types

/**
 * The basic type of an object.
 */
typedef enum
{
    PRO_NONE_TYPE = 0,
    PRO_MESSAGE_TYPE,
    PRO_ACTOR_TYPE,
    PRO_CONSTRUCTOR_TYPE,
    PRO_TYPE_MAX
} pro_type;

typedef const char* pro_actor_type;
extern pro_actor_type PRO_DEFAULT_ACTOR_TYPE;

typedef int(pro_match_impl)(pro_state_ref,
    pro_ref t, const void* tData,
    pro_ref o, const void* oData);

typedef const char*(pro_to_string_impl)(pro_state_ref,
    pro_ref t, const void* tData);

typedef struct pro_actor_type_info pro_actor_type_info;
struct pro_actor_type_info
{
    pro_match_impl* match;
    pro_to_string_impl* to_string;
};

PRO_API void (pro_register_actor_type) (pro_state_ref,
    pro_actor_type, const pro_actor_type_info*);

PRO_API const void* (pro_request_actor_data) (pro_state_ref,
    pro_ref);


#pragma mark State

/**
 * Creates a new execution state.
 *
 * @param[out] state The newly created state.
 *
 * @return
 *   PRO_OK if successful
 *   PRO_OUT_OF_MEMORY if a new state cannot be allocated.
 */
PRO_API pro_error (pro_state_create) (PRO_OUT pro_state_ref* state);

/**
 * Releases a state for future collection.
 */
PRO_API pro_error (pro_state_release) (pro_state_ref);

/**
 *
 */
PRO_API pro_error (pro_run) (pro_state_ref);

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
 */
PRO_API pro_error (pro_push_env) (pro_state_ref, pro_env_ref);

/**
 * Pops an environment off the environment stack.
 */
PRO_API pro_error (pro_pop_env) (pro_state_ref);


#pragma mark Environment

/**
 * Creates a new environment with a given parent.
 * 
 * @param[out] env A referance to the new environment.
 *
 * @return
 *   PRO_OK if successful
 *   PRO_OUT_OF_MEMORY if memory cannot be allocated.
 */
PRO_API pro_error (pro_env_create) (pro_state_ref, pro_env_ref parent,
    PRO_OUT pro_env_ref* env);

/**
 * Release an environment for future collection.
 */
PRO_API pro_error (pro_env_release) (pro_state_ref, pro_env_ref);

/**
 * @param[out] ref The highest resolved referance or PRO_EMPTY_REF if none.
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

/**
 * 
 */
PRO_API int (pro_match) (pro_state_ref, pro_ref, pro_ref);

/**
 * 
 */
PRO_API const char* (pro_to_string)(pro_state_ref,
    pro_ref);


#pragma mark Constructor

/**
 * Creates a new constructor in the current environment.
 *
 * @return The lookup for the new constructor.
 */
PRO_API pro_error (pro_constructor_create) (pro_state_ref, pro_constructor,
    PRO_OUT pro_ref* constructor);

/**
 * Calls a constructor with a list of arguments.
 * 
 * @param constructor The lookup for the constructor.
 * @param arguments A list of lookups to pass to the constructor.
 *
 * @return The result from the constructor.
 */
PRO_API pro_error (pro_constructor_call) (pro_state_ref,
    pro_ref constructor, pro_ref_list arguments, PRO_OUT pro_ref* result);


#pragma mark Message

/**
 * Creates a new message in the current environment.
 *
 * @return The lookup for the new message.
 */
PRO_API pro_error (pro_message_create) (pro_state_ref, PRO_OUT pro_ref* msg);

/**
 * @return The number of objects a message contains.
 */
PRO_API pro_error (pro_message_length) (pro_state_ref, pro_ref,
    PRO_OUT unsigned int* length);

/**
 * Get a value from a message.
 *
 * @return lookup or null if out of bounds.
 */
PRO_API pro_error (pro_message_get) (pro_state_ref, pro_ref, unsigned int index, 
    PRO_OUT pro_ref* result);

/**
 * Appends an value to a message.
 */
PRO_API pro_error (pro_message_append) (pro_state_ref, pro_ref msg, pro_ref value);


#pragma mark Actor

/**
 * Create a new actor object in the current environment.
 *
 * @return The lookup for the new actor.
 */
PRO_API pro_error (pro_actor_create) (pro_state_ref, pro_actor_type type,
    PRO_OUT pro_ref*);

/**
 * @return The type value of a lookup.
 */
PRO_API pro_error (pro_get_actor_type) (pro_state_ref, pro_ref lookup,
    PRO_OUT pro_actor_type*);

/**
 * Sends a message to an actor.
 */
PRO_API pro_error (pro_send) (pro_state_ref,
    pro_ref actor, pro_ref msg);

/**
 * Specify the behavior for an actor.
 */
PRO_API pro_error (pro_become) (pro_state_ref, pro_ref actor, pro_behavior);


#pragma mark Library Loading

#ifdef PRO_DYNAMIC_LIBRARY_LOADING

typedef void(pro_library_init)(pro_state_ref);

/** 
 *
 */
PRO_API void (pro_library_load) (pro_state_ref, const char* file);

#endif


#endif
