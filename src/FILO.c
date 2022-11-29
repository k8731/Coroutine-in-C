#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include "coroutine_int.h"

void rq_LIFO_init(struct rq_LIFO* rq) {// initialize
    // setup the ring buffer
    rq->top = 0;
    rq->mask = RINGBUFFER_SIZE - 1;
}

static inline unsigned int __ringbuffer_unused(struct rq_LIFO* rq) {// 環狀緩衝區
    return (rq->top == rq->mask);
}

// enqueue at top
int rq_LIFO_enqueue(struct rq_LIFO* rq, struct task_struct* task) {
    if (__ringbuffer_unused(rq))
        return -EAGAIN;

    rq->r[rq->top] = task;
    rq->top++;

    return 0;
}

// dequeue at top
struct task_struct* rq_LIFO_dequeue(struct rq_LIFO* rq) {
    struct task_struct* rev;

    if (rq->top == 0)
        return NULL;
    rq->top--;
    rev = rq->r[rq->top];
    return rev;
}
