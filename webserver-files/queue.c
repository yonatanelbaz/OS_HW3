//
// Created by Yonatan Elbaz on 07/06/2023.
//
#ifndef OS_HW3_QUEUE_C
#define OS_HW3_QUEUE_C

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "queue.h"



//typedef struct queue Queue;

Queue* init(int size){
    Queue* q = (Queue*)malloc(sizeof(Queue));
    q->requests = (int*)malloc(sizeof(int)*size);
    q->size = size;
    q->front = -1;
    q->rear = 0;
    q->empty = true;
    return q;
    /*sem_init(&queue.mutex, 0, 1);
    sem_init(&queue.slots, 0, size);
    sem_init(&queue.items, 0, 0);*/
}

void enqueue(int fd, Queue *q){
    /*sem_wait(&queue.slots);
    sem_wait(&queue.mutex);*/
    if(is_full(q)){
        printf("queue is full\n");
        return;
    }
    printf("enqueue %d\n", fd);
    q->requests[q->rear] = fd;
    q->rear = ((q->rear+1)%q->size);
    if(q->front == -1){
        q->front = q->rear - 1;
    }
    q->empty = false;
    /*sem_post(&queue.mutex);
    sem_post(&queue.items);*/
}

int dequeue(Queue *q){
    /*sem_wait(&queue.items);
    sem_wait(&queue.mutex);*/
    if(q->front == -1 || q->empty){
        printf("queue is empty\n");
        return -1;
    }
    int fd = q->requests[q->front];
    q->requests[q->front] = 0;
    q->front = (q->front+1)%q->size;
    if(q->front == q->rear){
        q->front = -1;
        q->empty = true;
    }
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

bool is_full(Queue *q){
    return q->front == q->rear;
}

bool is_empty(Queue *q){
    return q->empty;
}

int main(){
    Queue* q = init(5);
    Queue* q2 = init(5);
    print_queue(q);
    dequeue(q);
    enqueue(1, q);
    print_queue(q);
    enqueue(2, q);
    print_queue(q);
    enqueue(3, q);
    print_queue(q);
    enqueue(4, q);
    print_queue(q);
    enqueue(5, q);
    print_queue(q);
    enqueue(6,q);
    print_queue(q);
    dequeue(q);
    print_queue(q);
    dequeue(q);
    print_queue(q);
    dequeue(q);
    print_queue(q);
    enqueue(1, q);
    print_queue(q);
    enqueue(2, q);
    print_queue(q);
    enqueue(3, q);
    print_queue(q);
    enqueue(4, q);
    dequeue(q);
    dequeue(q);
    dequeue(q);
    dequeue(q);
    dequeue(q);
    dequeue(q);
    dequeue(q);
    dequeue(q);
    dequeue(q);
    enqueue(4, q);
    print_queue(q);
    enqueue(1, q);
    print_queue(q);
    enqueue(2, q);
    print_queue(q);
    enqueue(3, q);
    print_queue(q);
    enqueue(4, q);
    print_queue(q);
    dequeue(q);
    dequeue(q);
    dequeue(q);
    dequeue(q);
    dequeue(q);
    dequeue(q);
    print_queue(q);
    enqueue(4, q);
    print_queue(q);
    return 0;
}
#endif //OS_HW3_QUEUE_C