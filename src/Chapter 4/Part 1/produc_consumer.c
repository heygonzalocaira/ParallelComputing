#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>
#include "timer.h"

#define MaxItems 5 
#define BufferSize 5 
sem_t empty;
sem_t full;
int in = 0, out = 0;
int buffer[BufferSize];


void *producer(void *pno)
{   
    int item;
    for(int i = 0; i < MaxItems; i++) {
        item = rand(); 
        sem_wait(&empty);
        buffer[in] = item;
        printf("Productor %d: Inserta Item %d en %d\n", *((int *)pno),buffer[in],in);
        in = (in+1)%BufferSize;
        sem_post(&full);
    }
    printf("-----------------------------\n");
}
void *consumer(void *cno)
{   
    for(int i = 0; i < MaxItems; i++) {
        sem_wait(&full);
        int item = buffer[out];
        printf("Consumidor %d: Remueve Item %d de %d\n",*((int *)cno),item, out);
        out = (out+1)%BufferSize;
        sem_post(&empty);
    }
    printf("-----------------------------\n");
}

int main()
{   
    double start, finish, elapsed;
    pthread_t pro[3],con[3];
    sem_init(&empty,0,BufferSize);
    sem_init(&full,0,0);

    int a[3] = {1,2,3,}; //Just used for numbering the producer and consumer
     GET_TIME(start);
    for(int i = 0; i < 3; i++) {
        pthread_create(&pro[i], NULL, (void *)producer, (void *)&a[i]);
    }
    for(int i = 0; i < 3; i++) {
        pthread_create(&con[i], NULL, (void *)consumer, (void *)&a[i]);
    }

    for(int i = 0; i < 3; i++) {
        pthread_join(pro[i], NULL);
    }
    for(int i = 0; i < 3; i++) {
        pthread_join(con[i], NULL);
    }
    GET_TIME(finish);
    elapsed = finish - start;
    printf("   El tiempo es = %e secondos\n", elapsed);
    sem_destroy(&empty);
    sem_destroy(&full);
    
    return 0;
    
}