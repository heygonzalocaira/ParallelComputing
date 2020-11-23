#ifndef QUEUE_H
#define QUEUE_H

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
int search(struct queue* q, int mesg,int* src_p){
    struct node* curr_p = q->front;

    while(curr_p != NULL){
        if (curr_p-> mesg == mesg){
            *src_p = curr_p->src;
            return 1;
        }
        else{
            curr_p = curr_p->next;
        }
    }
    return 0;
}


#endif
