#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <string.h>

#include "common.h"

extern CommonData commonData2;


void *printerThread(void *arg){
    while (true) 
    {
        printf("Printing...\n");
        char buf[DATASIZE];
        int bufLines = 0;

        sem_wait(&commonData2.full);                                              // lock

        if (memcpy(buf, commonData2.buf, DATASIZE) == NULL){          // copy from commonData1, critical!
            perror("memcpy error\n");
            exit(EXIT_FAILURE);
        }
        bufLines = commonData2.bufLines;

        sem_post(&commonData2.empty);                                             // relase
            
        printf("%s", buf);
    }
    return EXIT_SUCCESS;
}