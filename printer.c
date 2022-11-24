#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <string.h>
#include <signal.h>

#include "common.h"

extern CommonData commonData2;
extern volatile sig_atomic_t exitRequested;


void *printerThread(void){
    while (!exitRequested) 
    {
        char buf[DATASIZE];

        // printf("Printer waiting... \n");
        // fflush(stdout);

        sem_wait(&commonData2.full);                                              // lock

        if (exitRequested) {
            return EXIT_SUCCESS;
        }

        if (memcpy(buf, commonData2.buf, DATASIZE) == NULL){              // copy from commonData1, critical!
            perror("memcpy error\n");
            exit(EXIT_FAILURE);
        }

        sem_post(&commonData2.empty);                                             // relase
                    
        // printf("...printer continues\n");
        // fflush(stdout);
        
        printf("%s\n", buf);
    }

    return EXIT_SUCCESS;
}