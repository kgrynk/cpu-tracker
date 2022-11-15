#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#include "common.h"
#include "reader.h"
#include "analyzer.h"
#include "printer.h"

#define THREADS_COUNT 3

CommonData commonData1;
CommonData commonData2;

int main(){
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
        int exitStatus;
        void* exitStatusPtr = &exitStatus;
        if (pthread_join(pthread[i], &exitStatusPtr) !=  EXIT_SUCCESS || exitStatus != EXIT_SUCCESS){
            exit(EXIT_FAILURE);
        }
        // printf("Exit: %d\n", exitStatus);
    }

    printf("It works!\n");

    return EXIT_SUCCESS;
}
