#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include "queue.h"
// gcc -g -Wall -fopenmp -o test omp_message.c
// ./test <n threads> <n>

struct node{
    int src;
    int mesg;
    struct node* next;
};

struct queue{
    int enqueue;
    int dequeue;
    struct node* front;  //frente de la cola
    struct node* tail;
};

struct queue* struct node{
    int src;
    int mesg;
    struct node* next;
};

struct queue{
    int enqueue;
    int dequeue;
    struct node* front;  //frente de la cola
    struct node* tail;
};

struct queue* Allocate(void){
    struct queue* q = malloc(sizeof(struct queue));
    q->enqueue = q->dequeue = 0;
    q->front = NULL;
    q->tail = NULL;
    return q;
}
void Free(struct queue* q){
    struct node* curr_p = q->front;
    struct node* temp_p;

    while(curr_p !=NULL){
        temp_p =curr_p;
        curr_p = curr_p->next;
        free(temp_p);
    }
    q->enqueue = q->dequeue = 0;
    q->front = q->tail = NULL;
}

void enqueue(struct queue* q, int src,int mesg){
    struct node* n_p = malloc(sizeof(struct node));
    n_p->src = src;
    n_p->mesg = mesg;
    n_p->next = NULL;
    if( q->tail == NULL){
        q->front = n_p;
        q->tail = n_p;
    }
    else{
        q->tail->next = n_p;
        q->tail = n_p;
    }
    q->enqueue++;
}

int dequeue(struct queue* q, int* src_p,int* mesg_p){
    struct node* temp_p;
    if( q->front == NULL) return 0;
    *src_p = q->front->src;
    *mesg_p = q->front->mesg;
    temp_p = q->front;

    if ( q->front == q->tail){
        q->front = q->tail = NULL;
    }
    else{
        q->front = temp_p ->next;
    }
    free(temp_p);
    q->dequeue++;
    return 1;
}

void Free(struct queue* q){
    struct node* curr_p = q->front;
    struct node* temp_p;

    while(curr_p !=NULL){
        temp_p =curr_p;
        curr_p = curr_p->next;
        free(temp_p);
    }
    q->enqueue = q->dequeue = 0;
    q->front = q->tail = NULL;
}
void Print(struct queue* q){
    struct node*curr_p = q->front;

    while(curr_p != NULL){
        printf(" src= %d, mesg = %d\n",curr_p->src, curr_p->mesg);
        curr_p = curr_p->next;
    }
    printf("enqueue = %d, dequeue = %d\n",q->enqueue,q->dequeue);
}
void enqueue(struct queue* q, int src,int mesg){
    struct node* n_p = malloc(sizeof(struct node));
    n_p->src = src;
    n_p->mesg = mesg;
    n_p->next = NULL;
    if( q->tail == NULL){
        q->front = n_p;
        q->tail = n_p;
    }
    else{
        q->tail->next = n_p;
        q->tail = n_p;
    }
    q->enqueue++;
}

int dequeue(struct queue* q, int* src_p,int* mesg_p){
    struct node* temp_p;
    if( q->front == NULL) return 0;
    *src_p = q->front->src;
    *mesg_p = q->front->mesg;
    temp_p = q->front;

    if ( q->front == q->tail){
        q->front = q->tail = NULL;
    }
    else{
        q->front = temp_p ->next;
    }
    free(temp_p);
    q->dequeue++;
    return 1;
}


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
