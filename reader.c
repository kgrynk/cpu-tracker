#include <pthread.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <linux/kernel.h>
#include <stdbool.h>
#include <unistd.h>
#include <semaphore.h>

#include "common.h"

extern CommonData commonData1;


void *readerThread(void *arg){
    

    for (int measure = 0; ; measure++) 
    {
        char buf[DATASIZE];
        size_t bufLen = 0;
        int bufLines = 0;
        
        printf("Reading...\n");
        
        FILE* statFile = fopen("/proc/stat", "r");
        if (statFile == NULL) 
        {
            perror("fopen error\n");
            exit(EXIT_FAILURE);
        }
    
        for (bufLines = 0; bufLen < DATASIZE; bufLines++) 
        {
            char line[MAX_LINE_SIZE] = {0};
            size_t lineSize = 0;
            char c = 0;
            
            do
            {
                c = fgetc(statFile);
                if(c == EOF)
                {
                    perror("fgets error\n");
                    exit(EXIT_FAILURE);
                }
                line[lineSize] = c;
                lineSize++;
            } 
            while(c != '\n' && lineSize < MAX_LINE_SIZE);                 // && c != EOF);


            if(strncmp(line, "cpu", 3) == 0)                    // check if line starts from "cpu" string
            {
                if (memcpy(buf + bufLen, line, lineSize) == NULL){
                    perror("memcpy error\n");
                    exit(EXIT_FAILURE);
                }
                bufLen += lineSize;
            } 
            else 
            {
                break;
            }
        }


        // lock
        sem_wait(&commonData1.empty);

        if (memcpy(commonData1.buf, buf, bufLen) == NULL){          // critical!
            perror("memcpy error\n");
            exit(EXIT_FAILURE);
        }
        commonData1.bufLines = bufLines;
    
        // relase
        sem_post(&commonData1.full);

        if (fclose(statFile) != EXIT_SUCCESS){
            perror("fclose error\n");
            exit(EXIT_FAILURE);
        }

        sleep(1);
    }

    sem_wait(&commonData1.empty);

    return EXIT_SUCCESS;
}