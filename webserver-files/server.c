#include "segel.h"
#include "request.h"

// 
// server.c: A very, very simple web server
//
// To run:
//  ./server <portnum (above 2000)>
//
// Repeatedly handles HTTP requests sent to this port number.
// Most of the work is done within routines written in request.c
//

// HW3: Parse the new arguments too
void getargs(int *port, int argc, char *argv[])
{
    if (argc < 2) {
	fprintf(stderr, "Usage: %s <port>\n", argv[0]);
	exit(1);
    }
    *port = atoi(argv[1]);
}

struct {
    int *requests;
    int size;
    int front;
    int rear;
    /*sem_t mutex;
    sem_t slots;
    sem_t items;*/
}queue;

void init(int size){
    queue.requests = (int*)malloc(sizeof(int)*size);
    queue.size = size;
    queue.front = 0;
    queue.rear = 0;
    /*sem_init(&queue.mutex, 0, 1);
    sem_init(&queue.slots, 0, size);
    sem_init(&queue.items, 0, 0);*/
}

void enqueue(int fd){
    /*sem_wait(&queue.slots);
    sem_wait(&queue.mutex);*/
    if(queue.front == queue.rear){
        printf("queue is full\n");
        return;
    }
    queue.requests[queue.rear] = fd;
    queue.rear = (queue.rear+1)%queue.size;
    /*sem_post(&queue.mutex);
    sem_post(&queue.items);*/
}

int dequeue(){
    /*sem_wait(&queue.items);
    sem_wait(&queue.mutex);*/
    int fd = queue.requests[queue.front];
    queue.front = (queue.front+1)%queue.size;
    /*sem_post(&queue.mutex);
    sem_post(&queue.slots);*/
    return fd;
}

int main(int argc, char *argv[])
{
    int listenfd, connfd, port, clientlen;
    struct sockaddr_in clientaddr;

    getargs(&port, argc, argv);

    // 
    // HW3: Create some threads...
    //

    listenfd = Open_listenfd(port);
    while (1) {
	clientlen = sizeof(clientaddr);
	connfd = Accept(listenfd, (SA *)&clientaddr, (socklen_t *) &clientlen);

	// 
	// HW3: In general, don't handle the request in the main thread.
	// Save the relevant info in a buffer and have one of the worker threads 
	// do the work. 
	// 
	requestHandle(connfd);

	Close(connfd);
    }

}


    


 
