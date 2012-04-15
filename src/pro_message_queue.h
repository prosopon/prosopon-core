#ifndef prosopon_pro_message_queue_h
#define prosopon_pro_message_queue_h

#include "pro_object.h"

typedef struct pro_message_queue pro_message_queue;

struct pro_message_queue;


struct pro_message_queue
{
    struct pro_message_node* front;
};


PRO_INTERNAL pro_message_queue* pro_message_queue_new(pro_state_ref);

PRO_INTERNAL void pro_message_queue_free(pro_state_ref, pro_message_queue*);

PRO_INTERNAL void pro_message_queue_enqueue(pro_state_ref, pro_message_queue*,
    pro_ref msg, pro_ref actor);
    
PRO_INTERNAL pro_ref pro_message_queue_dequeue(pro_state_ref,
    pro_message_queue*, PRO_OUT pro_ref* actor);

PRO_INTERNAL int pro_message_queue_is_empty(pro_state_ref,
    pro_message_queue*);


#endif
