//
// Created by Yonatan Elbaz on 09/06/2023.
//


#ifndef OS_HW3_QUEUE_H
#define OS_HW3_QUEUE_H
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


typedef struct queue{
    int *requests;
    int size;
    int front;
    int rear;
    bool empty;
    /*sem_t mutex;
    sem_t slots;
    sem_t items;*/
}Queue;

void init(int size, Queue *q);
void enqueue(int fd, Queue *q);
int dequeue(Queue *q);
void print_queue(Queue *q);

#endif //OS_HW3_QUEUE_H
