#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include "coroutine_int.h"

void MINE_rq_init(struct MINE_rq *rq)
{
    rq->in = 0;
    rq->mask = RINGBUFFER_SIZE - 1;
}

static inline unsigned int MINE__ringbuffer_unused(struct MINE_rq *rq)
{
    return rq->mask ==rq->in;
}

struct task_struct *MINE_dec(struct MINE_rq *rq)
{
    /*從stack後面進來的開始拿*/
    struct task_struct *tmp;
    if (!rq->in)
        return NULL;
    rq->in--;
    tmp = rq->r[rq->in];
    return tmp;
}

int MINE_add(struct MINE_rq *rq, struct task_struct *task)
{
    /*放進stack裡*/
    if (MINE__ringbuffer_unused(rq))
        return -EAGAIN;
    rq->r[rq->in] = task;
    rq->in++;
    return 0;
}
