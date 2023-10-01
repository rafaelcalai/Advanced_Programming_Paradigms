#include<stdio.h>
#include<pthread.h>
#include<time.h>
#include<unistd.h>
#include<stdlib.h>


#define CHAIRS 5
#define BARBERS 2


void * Costumer(void * data);
void * Barber(void * data);


pthread_mutex_t queue;
pthread_cond_t waiting;
int chairsAvailable = CHAIRS;


int main (int arg, char **args)
{
    printf("*----------------------*\n");
    printf("*---- BARBER SHOP -----*\n");
    printf("*----------------------*\n\n");

    time_t t;
    srand(time(&t));
    pthread_mutex_init(&queue, NULL);
    pthread_cond_init(&waiting, NULL);

    pthread_t threadBarber[BARBERS];
    pthread_t threadCostumer;

    int costumerId = 1;
    pthread_create(&threadCostumer, NULL, Costumer, &costumerId);
    sleep(1);

    int barberId[BARBERS];
    for(int id = BARBERS; id > 0; --id)
    {
        barberId[BARBERS - id] = BARBERS - id + 1;
        pthread_create(&threadBarber[BARBERS - id], NULL, Barber, &barberId[BARBERS - id]);
    }

    pthread_join(threadCostumer,NULL);
    for(int id = BARBERS; id > 0; --id)
    {
        pthread_join(threadBarber[BARBERS - id], NULL);
    }

    pthread_mutex_destroy(&queue);
    pthread_cond_destroy(&waiting);

    return 0;
}


void * Costumer(void * data){
    printf("Costumer Thread %d Started\n", *(int *)data);

    while(1)
    {
        pthread_mutex_lock(&queue);
        if(chairsAvailable == 0)
        {
            printf("COSTUMER -> New costumer arrived, but there is no chair available.\n");
        }
        else
        {
            if(--chairsAvailable == 4)
            {
                pthread_cond_signal(&waiting);
            }
            printf("COSTUMER -> New costumer arrived, queue %d/%d.\n", CHAIRS - chairsAvailable, CHAIRS);
        }
        pthread_mutex_unlock(&queue);
        usleep( 1000000 * (rand() % 5 + 1));
    }
}


void * Barber(void * data){
    printf("Barber Thread %d Started\n", *(int *)data);
    while(1)
    {
        pthread_mutex_lock(&queue);
        while(chairsAvailable == CHAIRS)
        {
            printf("BARBER%d -> No costumers waiting, let´s take a nap.\n", *(int *)data);
            pthread_cond_wait(&waiting, &queue);
        }
        chairsAvailable++;
        printf("BARBER%d  -> Next costumer to cut hair, queue %d/%d\n", *(int *)data,  CHAIRS - chairsAvailable, CHAIRS);
        pthread_mutex_unlock(&queue);

        usleep( 1000000 * (rand() % 10 + 1));
        printf("BARBER%d  -> Haircut finished. \n", *(int *)data);
    }
}
