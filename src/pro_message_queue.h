#ifndef prosopon_pro_message_queue_h
#define prosopon_pro_message_queue_h

#include "prosopon/core.h"


/**
 * Message queue data structure.
 */
typedef struct pro_message_queue pro_message_queue;


/**
 * @return A new empty message queue.
 */
PRO_INTERNAL pro_message_queue* pro_message_queue_new(pro_state_ref);

/**
 * Releases the memory for a message queue.
 */
PRO_INTERNAL void pro_message_queue_free(pro_state_ref, pro_message_queue*);

/**
 * Places a message in the message queue.
 */
PRO_INTERNAL void pro_message_queue_enqueue(pro_state_ref, pro_message_queue*,
    pro_ref msg, pro_ref actor);

/**
 * @return The object at the head of the queue or EMPTY_REF if empty.
 */
PRO_INTERNAL pro_ref pro_message_queue_dequeue(pro_state_ref,
    pro_message_queue*, PRO_OUT pro_ref* actor);

/**
 * @return Is the message queue empty.
 */
PRO_INTERNAL int pro_message_queue_is_empty(pro_state_ref,
    pro_message_queue*);


#endif
