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

void transfer(Queue *q1, Queue *q2){
    while(!is_empty(q1)){
        enqueue(dequeue(q1), q2);
    }
}

void enqueue(int fd, Queue *q){
    /*sem_wait(&queue.slots);
    sem_wait(&queue.mutex);*/
    if(is_full(q)){
        printf("queue is full\n");
        return;
    }
    q->requests[q->rear] = fd;
    q->rear = ((q->rear+1)%q->size);
    if(q->front == -1){
        q->front = (q->rear-1)%q->size;
    }
    q->empty = false;
    /*sem_post(&queue.mutex);
    sem_post(&queue.items);*/
}

int dequeue(Queue *q){
    if(q->empty){
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

/*int main(){
    //test the transfer function
    Queue *q1 = init(5);
    Queue *q2 = init(10);
    enqueue(18, q2);
    enqueue(19, q2);
    enqueue(1, q1);
    enqueue(2, q1);
    enqueue(3, q1);
    enqueue(4, q1);
    enqueue(5, q1);
    print_queue(q1);
    transfer(q1, q2);
    print_queue(q2);
    enqueue(6, q2);
    enqueue(7, q2);
    enqueue(8, q2);
    enqueue(9, q2);
    enqueue(10, q2);
    print_queue(q2);
    enqueue(9, q2);
    print_queue(q2);
    return 0;
}*/

    
#endif //OS_HW3_QUEUE_C