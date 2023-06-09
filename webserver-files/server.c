#include "segel.h"
#include "request.h"
#include "queue.h"

pthread_cond_t cond;
pthread_mutex_t mutex;

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

    if (atoi(argv[2]) <= 0){
        fprintf(stderr, "Thread count must be positive\n");
        exit(1);
    }
}

void* HandleRequest(void* connfd){
    return NULL;
}

void initThreadPool(pthread_t* thread_pool, int num_of_threads){
    int i;
    for(i = 0; i < num_of_threads; i++){
        pthread_t tid;
        pthread_create(&tid, NULL, HandleRequest, NULL);
        thread_pool[i] = tid;
    }
}
int main(int argc, char *argv[])
{
    int listenfd, connfd, port, clientlen, num_of_threads, queue_size;
    struct sockaddr_in clientaddr;
    getargs(&port, argc, argv);

    num_of_threads = atoi(argv[2]);
    queue_size = atoi(argv[3]);

    pthread_cond_init(&cond, NULL);
    pthread_mutex_init(&mutex, NULL);

    pthread_t thread_pool = malloc(sizeof(pthread_t) * num_of_threads);
    initThreadPool(thread_pool, argv[0][2]);
    Queue* q;
    init(queue_size, q);


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


    


 
