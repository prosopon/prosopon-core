#ifndef prosopon_h
#define prosopon_h

#include "prosopon_config.h"


/**
 * An opaque structure that holds information about the state of the prosopon
 * machine.
 */
typedef struct pro_state pro_state;

/**
 * An opaque structure that holds scope information. 
 */
typedef struct pro_env pro_env;

/**
 * An opaque structure used to resolve identifiers.
 */
typedef struct pro_lookup pro_lookup;

/**
 *
 */
typedef struct pro_actor_behavior pro_actor_behavior;


#pragma mark Types

typedef enum
{
    PRO_NONE_TYPE = 0,
    PRO_MESSAGE_TYPE,
    PRO_ACTOR_TYPE
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
 * Creates a new environment and pushes it onto the top of the env stack.
 */
PRO_API void (pro_env_create) (pro_state*);

/**
 * Release the current environment for future collection and pops it off the stack.
 */
PRO_API void (pro_env_release) (pro_state*);

/**
 * @return The lookup for the highest resolved lookup for a given name.
 */
PRO_API pro_lookup* (pro_env_lookup) (pro_state*, const char* name);

/**
 * @return The primitive type value of a lookup.
 */
PRO_API pro_type (pro_get_type) (pro_state*, pro_lookup* lookup);

/**
 * Binds a lookup to an identifier name.
 */
PRO_API void (pro_lookup_bind) (pro_state*, const pro_lookup* lookup, const char* id);


#pragma mark Constructor

typedef void(pro_constructor)(pro_state*);

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
