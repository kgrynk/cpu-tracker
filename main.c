#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>

#include "common.h"
#include "reader.h"
#include "analyzer.h"
#include "printer.h"

#define THREADS_COUNT 3

CommonData commonData1;
CommonData commonData2;

volatile sig_atomic_t exitRequested = 0;

void signal_handler(int signum) {
    exitRequested = 1;
    sem_post(&commonData1.empty);                                           // relase all locks
    sem_post(&commonData1.full);
    sem_post(&commonData2.empty);
    sem_post(&commonData2.full);
}

int main(){

    struct sigaction action;
    memset(&action, 0, sizeof(struct sigaction));
    action.sa_handler = signal_handler;                                         // install signal handler
    sigaction(SIGINT, &action, NULL);                           // handle INT
    sigaction(SIGTERM, &action, NULL);                          // handle TERM
    
    
    pthread_t pthread[THREADS_COUNT];
    
    sem_init(&commonData1.empty, 0, 1);
    sem_init(&commonData2.empty, 0,1);
    sem_init(&commonData1.full, 0, 0);
    sem_init(&commonData2.full, 0, 0);

    void* threadRoutine[THREADS_COUNT] = { readerThread, 
                                           analyzerThread, 
                                           printerThread };
    

    for (int i = 0; i < THREADS_COUNT; i++) {
        if (pthread_create(&pthread[i], NULL, threadRoutine[i], NULL) != EXIT_SUCCESS){
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < THREADS_COUNT; i++) {
        pthread_join(pthread[i], NULL);
    }
    
    if(!exitRequested){
        return EXIT_FAILURE;
    }

    // perror("\nIt works!");

    return EXIT_SUCCESS;
}
