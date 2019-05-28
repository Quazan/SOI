#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/sem.h>

#include "car.h"
#include "queue.h"
#include "cross.h"
#include "proQueue.h"
#define CROSS_ID      0
#define CROSS_WITH    2

struct Car car;

int main()
{
    //getting id of shm
    int queueId = shmget(QUEUE_KEY, 3*sizeof(struct Queue), 0600);
    int proQueueId = shmget(PROQUEUE_KEY, 3*sizeof(struct ProQueue), 0600);
    int pidId = shmget(PID_KEY, 3*sizeof(int) , 0600);

    //getting pointers on structures in shm
    struct Queue *queue = (struct Queue*)shmat(queueId, NULL, 0);
    struct ProQueue *proQueue = (struct ProQueue*)shmat(proQueueId, NULL, 0);
    int *pid = (int *)shmat(pidId, NULL, 0);

    //setting pid for this process
    pid[CROSS_ID] = getpid();

    //getting semaphores
    int fullId = semget(FULL_KEY, 3, 0600);
    int emptyId = semget(EMPTY_KEY, 3, 0600);
    int mutexId = semget(MUTEX_KEY, 3, 0600);
    int proMutexId = semget(PRO_MUTEX_KEY, 3, 0600);

    //main loop for cross process
    while(1)
    {
        if(getProQueueSize(&proQueue[CROSS_ID]) == 0 && getQueueSize(&queue[CROSS_WITH]) == 0) continue;  //if there is no car in both queues do nothing

        down(proMutexId, CROSS_ID);
        down(mutexId, CROSS_WITH);
        if(proPriority(&proQueue[CROSS_ID]) || (getProQueueSize(&proQueue[CROSS_ID]) > 0 && priority(&queue[CROSS_WITH]) == 0)) //choosing queue to remove car
        {
            up(mutexId, CROSS_WITH);

            car = removeCarProQueue(&proQueue[CROSS_ID]);   //removing car from production queue

            up(proMutexId, CROSS_ID);
        }
        else
        {
            up(proMutexId, CROSS_ID);
            up(mutexId, CROSS_WITH);
            down(fullId, CROSS_WITH);
            down(mutexId, CROSS_WITH);

            car = removeCar(&queue[CROSS_WITH]); //removing car from queue between crosses

            up(mutexId, CROSS_WITH);
            up(emptyId, CROSS_WITH);
        }

        printf("Samochod o id %d wjezdza na skrzyzowanie ", car.id);

        sleep(1);

        if(car.destination == CROSS_ID)
        {
            printf("i konczy jazde\n");
            continue;  //removing car from simulation if
        }

        down(emptyId, CROSS_ID);
        down(mutexId, CROSS_ID);

        addCar(&queue[CROSS_ID], car);  //adding car to queue for another cross process

        up(mutexId, CROSS_ID);
        up(fullId, CROSS_ID);
        printf("i zjezdza do skrzyzowania B\n");
    }
}
