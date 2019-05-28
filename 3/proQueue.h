#ifndef PROQUEUE_H
#define PROQUEUE_H

#define PRO_MAX_SIZE      2000 //maximum size of queue

#include <stdio.h>
#include <stdlib.h>
#include "car.h"

struct ProQueue
{
    struct Car table[PRO_MAX_SIZE];
    int head;
    int tail;
    int Size;
};

//initializing default values

void initializeProQueue(struct ProQueue *q)
{
    q->head = 0;
    q->tail = 0;
    q->Size = 0;
}

//getting size of queue

int getProQueueSize(struct ProQueue *q)
{
    return q->Size;
}

//adding car to the queue
//cars with priority at the beggining
//cars without priority at the end

void addCarProQueue(struct ProQueue *q, struct Car c)
{
    if(c.priority == 1)
    {
        q->head = (q->head - 1);
        if(q->head < 0) q->head = PRO_MAX_SIZE - 1;
        q->table[q->head] = c;
    }
    else
    {
        q->table[q->tail] = c;
        q->tail = (q->tail + 1) % PRO_MAX_SIZE;
    }

    q->Size++;
}

//removing car from queue

struct Car removeCarProQueue(struct ProQueue *q)
{
    struct Car c;
    c = q->table[q->head];
    q->head = (q->head + 1) % PRO_MAX_SIZE;
    q->Size--;

    return c;
}

//checking if car at head have priority

int proPriority(struct ProQueue *q)
{
    if(q->Size == 0) return 0;
    return q->table[q->head].priority;
}

#endif // PROQUEUE_H

