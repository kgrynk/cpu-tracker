#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>

#include "common.h"

extern CommonData commonData2;


void *printerThread(void *arg){
    printf("Printing...\n");

    return EXIT_SUCCESS;
}