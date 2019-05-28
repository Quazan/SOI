#ifndef CAR_H
#define CAR_H
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

struct Car{
    int id;
    int priority;      //0 - no priority 1 - priority
    int start;         //where it begins simulation
    int destination;  //cross where this car leaves simulation
};

//setting values for car

void setCar(struct Car *car, int id, int pri, int start, int dest)
{
    car->id = id;
    car->priority = pri;
    car->start = start;
    car->destination = dest;
}

//getting random number in range <0; mod)

int getRand(int mod)
{
    int val;
    val = rand()%mod;
    return val;
}

//generates Car with id and random values

struct Car generateCar(int id)
{
    struct Car c;
    int priority;
    int start;
    int destinantion;
    time_t tt;
    srand(time(&tt));

    start = getRand(3);
    destinantion = getRand(3);

    if(start == destinantion) destinantion = (destinantion+1)%3; //providing that car have different destination point than start

    //setting priority value 1/20 chances for priority car
    priority = getRand(20);
    if(priority != 1) priority = 0;

    setCar(&c, id, priority, start, destinantion);

    return c;
}

//prints values of car

void printCar(struct Car *c)
{
    printf("id: %d, priority: %d, start: %d, destination: %d\n", c->id, c->priority, c->start, c->destination);
}

#endif // CAR_H


