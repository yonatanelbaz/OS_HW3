//
// Created by Yonatan Elbaz on 07/06/2023.
//
#include <stdio.h>
#include <stdlib.h>

struct queue{
    int *requests;
    int size;
    int front;
    int rear;
    /*sem_t mutex;
    sem_t slots;
    sem_t items;*/
};

typedef struct queue Queue;

void init(int size, Queue *q){
    q->requests = (int*)malloc(sizeof(int)*size);
    q->size = size;
    q->front = -1;
    q->rear = 0;
    /*sem_init(&queue.mutex, 0, 1);
    sem_init(&queue.slots, 0, size);
    sem_init(&queue.items, 0, 0);*/
}

void enqueue(int fd, Queue *q){
    /*sem_wait(&queue.slots);
    sem_wait(&queue.mutex);*/
    if(q->front == q->rear){
        printf("queue is full\n");
        return;
    }
    printf("enqueue %d\n", fd);
    q->requests[q->rear] = fd;
    q->rear = ((q->rear+1)%q->size);
    if(q->front == -1){
        q->front = 0;
    }
    /*sem_post(&queue.mutex);
    sem_post(&queue.items);*/
}

int dequeue(Queue *q){
    /*sem_wait(&queue.items);
    sem_wait(&queue.mutex);*/
    int fd = q->requests[q->front];
    q->requests[q->front] = 0;
    q->front = (q->front+1)%q->size;

    /*sem_post(&queue.mutex);
    sem_post(&queue.slots);*/
    return fd;
}

void print_queue(Queue *q){
    int i;
    printf("queue");
    printf(" front: %d", q->front);
    printf(" rear: %d \n ", q->rear);


    for(i = 0; i < q->size; i++){
        printf("%d ", q->requests[(i + q->front)%q->size]);
    }
    printf("\n");

}

int main(){
    Queue* q;
    init(5, q);
    enqueue(1, q);
    enqueue(2, q);
    enqueue(3, q);
    enqueue(4, q);
    enqueue(5, q);
    enqueue(6,q);
    print_queue(q);
    dequeue(q);
    dequeue(q);
    dequeue(q);
    print_queue(q);
    return 0;
}