#include "pro_message_queue.h"

#include <stdlib.h>


#pragma mark Private

typedef struct pro_message_node pro_message_node;

struct pro_message_node
{
    pro_message_node* next;
    pro_ref message;
    pro_ref actor;
};

static pro_message_node* pro_message_node_new(pro_state_ref s,
    pro_ref message, pro_ref actor, pro_message_node* next)
{
    pro_message_node* t = malloc(sizeof(*t));
    t->message = message;
    t->next = next;
    t->actor = actor;
    return t;
}

#pragma -
#pragma mark Internal

PRO_INTERNAL pro_message_queue* pro_message_queue_new(pro_state_ref s)
{
    pro_message_queue* t = malloc(sizeof(*t));
    t->front = 0;
    return t;
}


PRO_INTERNAL void pro_message_queue_enqueue(pro_state_ref s,
    pro_message_queue* t, pro_ref msg, pro_ref actor) 
{
    if (0 == t->front)
        t->front = pro_message_node_new(s, msg, actor, 0);
    else
    {
        pro_message_node* parent = t->front;
        while (parent->next)
            parent = parent->next;
        parent->next = pro_message_node_new(s, msg, actor, 0); 
    }
}


PRO_INTERNAL pro_ref pro_message_queue_dequeue(pro_state_ref s,
    pro_message_queue* t, PRO_OUT pro_ref* actor)
{
    if (!t->front)
        return 0;
    else
    {
        pro_message_node* front = t->front;
        pro_ref msg = front->message;
        *actor = front->actor;
        t->front = front->next;
        free(front);
        return msg;
    }
}


PRO_INTERNAL int pro_message_queue_is_empty(pro_state_ref s,
    pro_message_queue* t)
{
    return t->front == 0;
}
