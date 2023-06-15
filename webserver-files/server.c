#include "segel.h"
#include "request.h"
#include "queue.h"

pthread_cond_t queue_cond;
pthread_mutex_t queue_mutex;
pthread_cond_t sum_cond;
pthread_mutex_t sum_mutex;
Queue* requests_queue;
int current_requests_num = 0;

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

    //if (atoi(argv[2]) <= 0){
    //    fprintf(stderr, "Thread count must be positive\n");
    //    exit(1);
    //}
}

void* HandleRequest(void* connfd){
    while(1){
        pthread_mutex_lock(&queue_mutex);
        while(is_empty(requests_queue)){
            pthread_cond_wait(&queue_cond, &queue_mutex);
        }
        int fd = dequeue(requests_queue);
        pthread_mutex_unlock(&queue_mutex);

        requestHandle(fd);
        Close(fd);

        pthread_mutex_lock(&sum_mutex);
        current_requests_num--;
        printf("current_requests_num after removal: %d\n", current_requests_num);
        //pthread_cond_signal(&sum_cond);
        pthread_mutex_unlock(&sum_mutex);
    }
}

void initThreadPool(pthread_t* thread_pool, int num_of_threads){
    pthread_t tid;
    for(int i = 0; i < num_of_threads; i++){
        pthread_create(&tid, NULL, HandleRequest, NULL);
        thread_pool[i] = tid;
    }
}
int main(int argc, char *argv[])
{
    int listenfd, connfd, port, clientlen, num_of_threads, queue_size;
    struct sockaddr_in clientaddr;
    char* policy;
    getargs(&port, argc, argv);

    num_of_threads = atoi(argv[2]);
    queue_size = atoi(argv[3]);
    policy = argv[4];


    requests_queue = init(queue_size);

    pthread_cond_init(&queue_cond, NULL);
    pthread_mutex_init(&queue_mutex, NULL);
    pthread_cond_init(&sum_cond, NULL);
    pthread_mutex_init(&sum_mutex, NULL);

    pthread_t* thread_pool = (pthread_t *)malloc(sizeof(pthread_t) * num_of_threads);
    initThreadPool(thread_pool, num_of_threads);

    listenfd = Open_listenfd(port);
    while (1) {
	    clientlen = sizeof(clientaddr);
	    connfd = Accept(listenfd, (SA *)&clientaddr, (socklen_t *) &clientlen);

//        if(current_requests_num == queue_size){
//            if(strcmp(policy,"block") == 0){
//                //block main thread until there is a place in the queue
//                pthread_mutex_lock(&sum_mutex);
//                while(current_requests_num == queue_size){
//                    pthread_cond_wait(&sum_cond, &sum_mutex);
//                }
//                pthread_mutex_unlock(&sum_mutex);
//            }
//        }
        pthread_mutex_lock(&queue_mutex);
        pthread_mutex_lock(&sum_mutex);
        enqueue(connfd, requests_queue);
        current_requests_num++;
        printf("current_requests_num after adding %d = %d\n",connfd, current_requests_num);
        pthread_cond_signal(&queue_cond);
        pthread_mutex_unlock(&sum_mutex);
        pthread_mutex_unlock(&queue_mutex);
    }

}


    


 
