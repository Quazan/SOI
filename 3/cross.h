#ifndef CROSS_H
#define CROSS_H

//defining keys for shm and sem

#define QUEUE_KEY 1600
#define PROQUEUE_KEY 1608
#define FULL_KEY 1602
#define EMPTY_KEY 1603
#define MUTEX_KEY 1604
#define PRO_MUTEX_KEY 1605
#define PID_KEY 1701

static struct sembuf buf;

//operation down on semaphore

void down(int semid, int semnum)
{
    buf.sem_num = semnum;
    buf.sem_op = -1;
    buf.sem_flg = 0;
    if (semop(semid, &buf, 1) == -1)
    {
        perror("Opuszczenie semafora");
        exit(1);
    }
}

//operation up on semaphore

void up(int semid, int semnum)
{
    buf.sem_num = semnum;
    buf.sem_op = 1;
    buf.sem_flg = 0;
    if (semop(semid, &buf, 1) == -1)
    {
        perror("Podnoszenie semafora");
        exit(1);
    }
}


#endif // CROSS_H
