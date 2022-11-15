#pragma once

#include <semaphore.h>

#define MAX_LINE_SIZE 256
#define MAX_CPUS 129
#define DATASIZE MAX_LINE_SIZE * MAX_CPUS


typedef struct{
    char buf[DATASIZE];
    
    int bufLines;           // number of cpu threads + 1
    sem_t empty, full;
} CommonData;
