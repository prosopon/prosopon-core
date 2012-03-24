#ifndef prosopon_h
#define prosopon_h

#include "prosopon_config.h"

struct pro_env;


/**
 * @mainpage
 *
 * @author Matt Bierner
 * @version 0.1.0
 *
 * @section intro_sec Introduction
 */


/**
 * An opaque structure that holds information about the state of the prosopon
 * machine.
 */
typedef struct pro_state pro_state;

/**
 * An opaque structure used to identify environments.
 */
typedef struct pro_env pro_env_lookup; 

/**
 * An opaque structure used to resolve identifiers.
 */
typedef struct pro_lookup pro_lookup;

/**
 * Function representing the behavior of an actor.
 * 
 * @param t A lookup for self.
 * @param data Additional user defined data passed to the behavior.
 */
typedef void(pro_actor_behavior)(pro_state*, pro_lookup* t, void* data);

/**
 * Function representing the behavior of an actor.
 * 
 * @param data Additional user defined data passed to the constructor.
 *
 * @return A lookup to the constructed object.
 */
typedef pro_lookup*(pro_constructor)(pro_state*, void* data);


#pragma mark Types

/**
 * The basic type of an object.
 */
typedef enum
{
    PRO_NONE_TYPE = 0,
    PRO_MESSAGE_TYPE,
    PRO_ACTOR_TYPE,
    PRO_TYPE_MAX
} pro_type;


#pragma mark State

/**
 * Creates a new execution state.
 *
 * @return The newly created state or null if error.
 */
PRO_API pro_state* (pro_state_create) (void);

/**
 * Releases a state for future collection.
 */
PRO_API void (pro_state_release) (pro_state*);


#pragma mark Environment

/**
 * Creates a new environment with a given parent.
 * 
 * @return The lookup for a newly created environment.
 */
PRO_API pro_env_lookup* (pro_env_create) (pro_state*, pro_env_lookup* parent);

/**
 * @return The lookup for the current environment. 
 */
PRO_API pro_env_lookup* (pro_env_get) (pro_state*);

/**
 * Pushes an environment onto the environment stack
 */
PRO_API void (pro_env_push) (pro_state*, pro_env_lookup*);

/**
 * Pops an environment off the environment stack.
 */
PRO_API void (pro_env_pop) (pro_state*);

/**
 * Release an environment for future collection.
 */
PRO_API void (pro_env_release) (pro_state*, pro_env_lookup*);

/**
 * @return The lookup for the highest resolved lookup for a given name.
 */
PRO_API pro_lookup* (pro_get_binding) (pro_state*, const char* name);

/**
 * @return The primitive type value of a lookup.
 */
PRO_API pro_type (pro_lookup_get_type) (pro_state*, pro_lookup* lookup);

/**
 * Binds a lookup to an identifier name.
 */
PRO_API void (pro_lookup_bind) (pro_state*, const pro_lookup* lookup, const char* id);


#pragma mark Constructor

PRO_API pro_lookup* (pro_constructor_create) (pro_state*, pro_constructor*);


#pragma mark Message

/**
 * Creates a new message in the current environment.
 *
 * @return The lookup for the new message.
 */
PRO_API pro_lookup* (pro_message_create) (pro_state*);

/**
 * @return The number of objects a message contains.
 */
PRO_API unsigned int (pro_message_length) (pro_state*, pro_lookup* lookup);

/**
 * Get a value from a message.
 *
 * @return lookup or null if out of bounds.
 */
PRO_API pro_lookup* (pro_message_get) (pro_state*, unsigned int idx);

/**
 * Appends an value to a message.
 */
PRO_API void (pro_message_append) (pro_state*, pro_lookup* lookup);


#pragma mark Actor

/**
 * Create a new actor object in the current environment.
 *
 * @return The lookup for the new actor.
 */
PRO_API pro_lookup* (pro_actor_create) (pro_state*);

/**
 * Sends a message to an actor.
 */
PRO_API void (pro_send) (pro_state*, pro_lookup* actor, pro_lookup* msg);

/**
 * Specify the behavior for an actor.
 */
PRO_API void (pro_become) (pro_state*, pro_lookup* old, pro_actor_behavior* new_beh);



#endif
