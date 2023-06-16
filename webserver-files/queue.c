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
    printf("enqueued %d\n", fd);
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
        q->rear = 0;
        q->empty = true;
    }
    printf("dequeued %d\n", fd);
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

void increase_queue_size(Queue *q){
    int i;
    int count = 0;
    int new_front = -1;
    int *new_requests = (int*)malloc(sizeof(int)* (q->size+1));
    if(!q->empty) {
        new_front = 0;
        for (i = 0; i < q->size; i++) {
            new_requests[i] = q->requests[(i + q->front) % (q->size)];
            if (new_requests[i] != 0) {
                count++;
            }
        }
    }
    q->rear = count;
    q->front = new_front;
    q->size++;
    int* temp = q->requests;
    q->requests = new_requests;
    free(temp);
}

bool is_full(Queue *q){
    return q->front == q->rear;
}

bool is_empty(Queue *q){
    return q->empty;
}


    
#endif //OS_HW3_QUEUE_C