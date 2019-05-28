#ifndef QUEUE_H
#define QUEUE_H


#define MAX_SIZE      10  //maximum size of queue

#include <stdio.h>
#include <stdlib.h>
#include "car.h"

struct Queue
{
    struct Car table[MAX_SIZE];
    int head;
    int tail;
    int Size;
};

//initializing default values

void initializeQueue(struct Queue *q)
{
    q->head = 0;
    q->tail = 0;
    q->Size = 0;
}

//getting size of queue

int getQueueSize(struct Queue *q)
{
    return q->Size;
}

//adding car to the queue
//cars with priority at the beggining
//cars without priority at the end

void addCar(struct Queue *q, struct Car c)
{
    if(c.priority == 1)
    {
        q->head = (q->head - 1);
        if(q->head < 0) q->head = MAX_SIZE - 1;
        q->table[q->head] = c;
    }
    else
    {
        q->table[q->tail] = c;
        q->tail = (q->tail + 1) % MAX_SIZE;
    }

    q->Size++;
}

//removing car from queue

struct Car removeCar(struct Queue *q)
{
    struct Car c;
    c = q->table[q->head];
    q->head = (q->head + 1) % MAX_SIZE;
    q->Size--;

    return c;
}

//checking if car at head have priority

int priority(struct Queue *q)
{
    if(q->Size == 0) return 0;
    return q->table[q->head].priority;
}


#endif // QUEUE_H
