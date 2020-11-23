#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include "queue.h"
// gcc -g -Wall -fopenmp -o test omp_message.c
// ./test <n threads> <n>
void Send_msg(struct queue* msg_queue[],int my_rank);
void Try_receive(struct queue* q,int my_rank);
int Done(struct queue* q,int done_sending);

int main(int argc,char* argv[]){
    int thread_count;
    int send_max;
    struct queue ** msg_queues;
    int done_sending = 0;
    
    thread_count = strtol(argv[1],NULL,10);
    send_max = strtol(argv[2],NULL,10);

    msg_queues = (struct queue**)malloc(thread_count*sizeof(struct node *));
    
#   pragma omp parallel num_threads(thread_count) \
        default(none) shared(msg_queues,send_max,done_sending)
    {
        int my_rank = omp_get_thread_num();
        int sent_msgs;
        srandom(my_rank);
        msg_queues[my_rank] = Allocate();

#       pragma omp barrier
        for(sent_msgs = 0; sent_msgs < send_max ; sent_msgs++){
            Send_msg(msg_queues,my_rank);
            Try_receive(msg_queues[my_rank],my_rank);
        }

#       pragma omp atomic
        done_sending++;
        
        while(!Done(msg_queues[my_rank],done_sending))
            Try_receive(msg_queues[my_rank],my_rank);
        Free(msg_queues[my_rank]);
        free(msg_queues[my_rank]);
    }
    

    free(msg_queues);
    
    return 0;
}

void Send_msg(struct queue* msg_queues[],int my_rank){
    int thread_count = omp_get_num_threads();
    int mesg =random()%100;
    int dest = random() % thread_count;
# pragma omp critical
    enqueue(msg_queues[dest],my_rank,mesg);
    printf("hilo %d enviado %d para %d\n",my_rank,mesg,dest);
}

void Try_receive(struct queue* q,int my_rank){
    int src, mesg;

    int queue_size =q->enqueue-q->dequeue;

    if(queue_size == 0) return;
    else if(queue_size==1)
#       pragma omp critical
        dequeue(q,&src,&mesg);
    else
        dequeue(q,&src,&mesg);
    printf("Hilo %d > recibido %d de %d\n",my_rank,mesg,src);

}

int Done(struct queue* q,int done_sending){
    int thread_count = omp_get_num_threads();
    int queue_size = q->enqueue - q->dequeue;

    if (queue_size == 0 && done_sending == thread_count)    
        return 1;
    else   
        return 0;
}
