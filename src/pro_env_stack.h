#ifndef prosopon_pro_env_stack_h
#define prosopon_pro_env_stack_h

#include "prosopon.h"


typedef struct pro_env_stack pro_env_stack;


/**
 * @return An new, empty environment stack.
 */
PRO_INTERNAL pro_env_stack* pro_env_stack_new(pro_state_ref);

/**
 * Frees the memory for an env_stack
 */
PRO_INTERNAL void pro_env_stack_free(pro_state_ref, pro_env_stack*);

/**
 * Pushes an environment onto the stack.
 */
PRO_INTERNAL void pro_env_stack_push(pro_state_ref, pro_env_stack*, pro_env_ref);

/**
 * Pops an environemtn off the stack.
 */
PRO_INTERNAL void pro_env_stack_pop(pro_state_ref, pro_env_stack*);

/**
 * @return The top environment of the stack.
 */
PRO_INTERNAL pro_env_ref pro_env_stack_top(pro_state_ref, pro_env_stack*);


#endif
