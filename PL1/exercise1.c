#include<stdio.h>
#include<pthread.h>
#include<string.h>
#include<time.h>
#include<unistd.h>
#include<stdlib.h>
#include <signal.h>

#define PHILOSOPHERS    5
#define FORKS           5

pthread_mutex_t fork_mutex[FORKS];
pthread_mutex_t printMutex;

void Print(char * text);
void * Philosopher (void * index);

int main(int arg, char **argv)
{
    printf("-----------------------------\n");
    printf("    Philosopher Problem\n");
    printf("-----------------------------\n\n");

    time_t t;
    srand((unsigned) time(&t));

    pthread_t threadId[PHILOSOPHERS];
    pthread_mutex_init(&printMutex, NULL);
    for(int counter = FORKS; counter > 0; --counter) pthread_mutex_init(&fork_mutex[counter - 1], NULL);

    int threadIndex[5] = {1, 2, 3, 4, 5};
    for(int index = PHILOSOPHERS; index > 0; --index)
    {
        pthread_create(&threadId[index - 1], NULL, Philosopher, &threadIndex[index-1]);
    }

    sleep(60);
    for(int index = PHILOSOPHERS; index > 0; --index)
    {
        pthread_kill( threadId[PHILOSOPHERS - index], SIGUSR1);
        pthread_join(threadId[PHILOSOPHERS - index], NULL);
    }
    for(int counter = FORKS; counter > 0; --counter) pthread_mutex_destroy(&fork_mutex[counter - 1]);

    pthread_mutex_destroy(&printMutex);

    exit(0);
}

void Print(char * text)
{
    pthread_mutex_lock(&printMutex);
    printf("%s", text);
    pthread_mutex_unlock(&printMutex);
}

void * Philosopher (void * index)
{
    int philosopherIndex = *(int *) index;
    char msg[100];

    int leftFork = philosopherIndex, rightFork;
    rightFork = (philosopherIndex == 1) ?  5: (leftFork - 1);
    sprintf(msg, "Philosopher %d Thread started, leftFork %d rightFork %d\n", philosopherIndex, leftFork, rightFork);
    Print(msg);
    sleep(2);

    while(1)
    {
        sprintf(msg, "Philosopher %d Thinking\n", philosopherIndex);
        Print(msg);

        usleep((rand() % 10) * 1000000);

        sprintf(msg, "Philosopher %d Hungry\n", philosopherIndex);
        Print(msg);

        while (1)
        {
            pthread_mutex_lock(&fork_mutex[rightFork-1]);
            if(pthread_mutex_trylock(&fork_mutex[leftFork-1]) != 0)
            {
                pthread_mutex_unlock(&fork_mutex[rightFork-1]);
                sleep(2);
            }
            else{
                sprintf(msg, "Philosopher %d took forks: %d and %d -> start eating\n", philosopherIndex, leftFork, rightFork);
                Print(msg);
                usleep((rand() % 5) * 1000000);

                pthread_mutex_unlock(&fork_mutex[rightFork-1]);
                pthread_mutex_unlock(&fork_mutex[leftFork-1]);
                sprintf(msg, "Philosopher %d Full, Release forks: %d and %d <-  stop eating\n", philosopherIndex, leftFork, rightFork);
                Print(msg);
                break;
            }
        }
    }
}
