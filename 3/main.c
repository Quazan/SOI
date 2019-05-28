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

int cars_quantity;
int cars_frequency;

struct Car car;

int main()
{

    //creating shm, getting id
    int queueId = shmget(QUEUE_KEY, 3*sizeof(struct Queue), IPC_CREAT|0600);
    int proQueueId = shmget(PROQUEUE_KEY, 3*sizeof(struct ProQueue), IPC_CREAT|0600);
    int pidId = shmget(PID_KEY, 3*sizeof(int) , IPC_CREAT|0600);

    //taking pointers on shm
    struct Queue *queue = (struct Queue*)shmat(queueId, NULL, 0);
    struct ProQueue *proQueue = (struct ProQueue*)shmat(proQueueId, NULL, 0);
    int *pid = (int *)shmat(pidId, NULL, 0);

    //initializing queues in shm
    for(int i = 0; i < 3; i++) initializeQueue(&queue[i]);
    for(int i = 0; i < 3; i++) initializeProQueue(&proQueue[i]);

    //initializing semaphores and setting them up
    int fullId = semget(FULL_KEY, 3, IPC_CREAT|IPC_EXCL|0600);
    if(fullId==-1)
    {
        fullId = semget(FULL_KEY, 3, 0600);
        if(fullId==-1) perror("BLAD FULLID");
    }
    semctl(fullId, 0, SETVAL, (int)0);
    semctl(fullId, 1, SETVAL, (int)0);
    semctl(fullId, 2, SETVAL, (int)0);

    int emptyId = semget(EMPTY_KEY, 3, IPC_CREAT|IPC_EXCL|0600);
    if(emptyId==-1)
    {
        emptyId = semget(EMPTY_KEY, 3, 0600);
        if(emptyId==-1) perror("BLAD EMPTYID");
    }
    semctl(emptyId, 0, SETVAL, MAX_SIZE);
    semctl(emptyId, 1, SETVAL, MAX_SIZE);
    semctl(emptyId, 2, SETVAL, MAX_SIZE);

    int mutexId = semget(MUTEX_KEY, 3, IPC_CREAT|IPC_EXCL|0600);
    if(mutexId==-1)
    {
        mutexId = semget(MUTEX_KEY, 3, 0600);
        if(mutexId==-1) perror("BLAD MUTEXID");
    }
    semctl(mutexId, 0, SETVAL, (int)1);
    semctl(mutexId, 1, SETVAL, (int)1);
    semctl(mutexId, 2, SETVAL, (int)1);

    int proMutexId = semget(PRO_MUTEX_KEY, 3, IPC_CREAT|IPC_EXCL|0600);
    if(proMutexId==-1)
    {
        proMutexId = semget(PRO_MUTEX_KEY, 3, 0600);
        if(proMutexId==-1) perror("BLAD PROMUTEXID");
    }
    semctl(proMutexId, 0, SETVAL, (int)1);
    semctl(proMutexId, 1, SETVAL, (int)1);
    semctl(proMutexId, 2, SETVAL, (int)1);

    //setting number of cars in simulation
    printf("Prosze podac ilosc samochodow w symulacji \n");
    scanf("%d", &cars_quantity);

    //setting frequency of generating cars in 0,x s
    printf("Prosze podac czestotliwosc (x) z jaka maja sie pojawiac samochody (0,x sek) \n");
    scanf("%d", &cars_frequency);

    //main loop for generating cars
    for(int id = 1; id <= cars_quantity; id++)
    {
        car = generateCar(id);
        //semaphores provide that only one process have access to that queue at the time
        down(proMutexId, car.start);
        addCarProQueue(&proQueue[car.start], car); //adding car to adequate queue
        up(proMutexId, car.start);

        printCar(&car);
        usleep(cars_frequency*100000); //waiting for next generating
    }

    //suspension from imidiate ending of simulation
    printf("Aby zakończyć wciśnij  dowolny przycisk + ENTER.\n");
    scanf("%d", &cars_quantity);

    //killing crossA, crossB, crossC
    for(int i = 0; i <= 2; i++)
    {
        kill(pid[i], SIGKILL);
    }

    //deleting shm and sem
    semctl(fullId, 0, IPC_RMID);
    semctl(emptyId, 0, IPC_RMID);
    semctl(mutexId, 0, IPC_RMID);
    semctl(proMutexId, 0, IPC_RMID);
    shmctl(queueId, IPC_RMID, 0);
    shmctl(proQueueId, IPC_RMID, 0);
    shmctl(pidId, IPC_RMID, 0);
    shmdt(queue);
    shmdt(proQueue);
    shmdt(pid);
}
