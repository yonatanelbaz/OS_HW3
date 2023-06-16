#include "segel.h"
#include "request.h"
#include "queue.h"

pthread_cond_t queue_cond;
pthread_mutex_t queue_mutex;
pthread_cond_t sum_cond;
pthread_mutex_t sum_mutex;
//Queue* requests_queue;
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

void* HandleRequest(void* requests_queue){
    requests_queue = (Queue*)requests_queue;
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
        pthread_cond_signal(&sum_cond);
        pthread_mutex_unlock(&sum_mutex);
    }
}

void initThreadPool(pthread_t* thread_pool, int num_of_threads, Queue* requests_queue){
    pthread_t tid;
    for(int i = 0; i < num_of_threads; i++){
        pthread_create(&tid, NULL, HandleRequest, requests_queue);
        thread_pool[i] = tid;
    }
}
int main(int argc, char *argv[])
{
    Queue* requests_queue;
    int listenfd, connfd, port, clientlen, num_of_threads, queue_size;
    struct sockaddr_in clientaddr;
    char* policy;
    getargs(&port, argc, argv);

    num_of_threads = atoi(argv[2]);
    queue_size = atoi(argv[3]);
    policy = argv[4];
    int max_queue_size = -1;
    if(strcmp(policy,"dynamic") == 0){
        //might cause eroor if max size not provided despite dynamic policy
        max_queue_size = atoi(argv[5]);
    }

    requests_queue = init(queue_size);

    pthread_cond_init(&queue_cond, NULL);
    pthread_mutex_init(&queue_mutex, NULL);
    pthread_cond_init(&sum_cond, NULL);
    pthread_mutex_init(&sum_mutex, NULL);

    pthread_t* thread_pool = (pthread_t *)malloc(sizeof(pthread_t) * num_of_threads);
    initThreadPool(thread_pool, num_of_threads, requests_queue);

    listenfd = Open_listenfd(port);
    while (1) {
	    clientlen = sizeof(clientaddr);
	    connfd = Accept(listenfd, (SA *)&clientaddr, (socklen_t *) &clientlen);


        if(current_requests_num >= queue_size){
            printf("policy: %s\n", policy);
            if(strcmp(policy,"block") == 0){
                pthread_mutex_lock(&sum_mutex);
                while(current_requests_num == queue_size){
                    pthread_cond_wait(&sum_cond, &sum_mutex);
                }
                pthread_mutex_unlock(&sum_mutex);
            }

            else if(strcmp(policy,"dt") == 0 || (strcmp(policy,"dynamic") == 0 && max_queue_size <= queue_size)){
                printf("max queue size reached\n");
                printf("max queue size: %d, queue size: %d\n", max_queue_size, queue_size);
                Close(connfd);
                continue;
            }

            else if(strcmp(policy,"dh") == 0){
                int removed_fd;
                pthread_mutex_lock(&queue_mutex);
                pthread_mutex_lock(&sum_mutex);
                removed_fd = dequeue(requests_queue);
                if(removed_fd != -1){
                    close(removed_fd);
                    current_requests_num--;
                }
                pthread_mutex_unlock(&sum_mutex);
                pthread_mutex_unlock(&queue_mutex);
            }

            else if(strcmp(policy,"bf") == 0){
                pthread_mutex_lock(&sum_mutex);
                while(current_requests_num != 0){
                    pthread_cond_wait(&sum_cond, &sum_mutex);
                }
                close(connfd);
                pthread_mutex_unlock(&sum_mutex);
                continue;
            }

            else if(strcmp(policy,"dynamic") == 0){
                pthread_mutex_lock(&queue_mutex);
                close(connfd);
                queue_size++;
                increase_queue_size(requests_queue);
                printf("queue size increased to %d\n", queue_size);
                pthread_mutex_unlock(&queue_mutex);
                continue;
            }
        }

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


    


 
