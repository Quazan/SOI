#ifndef CROSSMONITOR_H
#define CROSSMONITOR_H

#include "queue.h"
#include "car.h"
#include <stdlib.h>
#include "monitor.h"

class crossMonitor : Monitor
{
    Queue buffer;
    Condition full;
    Condition empt;

public:
        crossMonitor();
        void add(Car c);
        Car take();
        int getSize();
        int checkPriority();
};

crossMonitor::crossMonitor()
{
    initializeQueue(&buffer);//initialize buffer with default values
}

void crossMonitor::add(Car c)//add car to buffer
{
    enter();
    if(getQueueSize(&buffer) == MAX_SIZE)//if queueu is full then wait untill it's not full
    {
        wait(full);
    }
    addCar(&buffer, c);//add car to buffer
    if(getQueueSize(&buffer) == 1)//if queue is no loner empty then signal that is no longer empty
    {
        signal(empt);
    }
    leave();
}

Car crossMonitor::take() //return car from buffer
{
    Car c;
    enter();
    if(getQueueSize(&buffer) == 0)//if queue is empty then wait untill it's not empty
    {
        wait(empt);
    }
    c = removeCar(&buffer);//take car from buffer
    if(getQueueSize(&buffer) == MAX_SIZE - 1)//if queue is no longer empty then signal it
    {
        signal(full);
    }
    leave();
    return c;
}

int crossMonitor::getSize()//return size of buffer in monitor
{
    return getQueueSize(&buffer);
}

int crossMonitor::checkPriority()//return 1 if first element have priority or 0 if not
{
    return priority(&buffer);
}


#endif // CROSSMONITOR_H
