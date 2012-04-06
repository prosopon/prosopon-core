#ifndef prosopon_pro_env_stack_h
#define prosopon_pro_env_stack_h

#include "prosopon.h"

struct pro_env_stack_node;


typedef struct pro_env_stack pro_env_stack;

/**
 * Structure for a fifo stack of environments.
 */
struct pro_env_stack
{
    struct pro_env_stack_node* top;
};


/**
 * @return An new, empty environment stack.
 */
PRO_INTERNAL pro_env_stack* pro_env_stack_new(pro_state_ref);

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
