#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <string.h>

#include "common.h"

extern CommonData commonData1;
extern CommonData commonData2;


void *analyzerThread(void *arg){
    while (true) 
    {

        sem_wait(&commonData1.full);

        printf("Analyzing...\n");
        printf("%s\n", commonData1.buf);
        
        sem_post(&commonData1.empty);
    }
    return EXIT_SUCCESS;
}
