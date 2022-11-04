#include "main.h"


int main(){
    pthread_t thread_id;
    pthread_create(&thread_id, NULL, readerThread, NULL);
    pthread_create(&thread_id, NULL, analyzerThread, NULL);
    pthread_create(&thread_id, NULL, printerThread, NULL);
    pthread_join(thread_id, NULL);
    printf("It works!\n");
    return 0;
}