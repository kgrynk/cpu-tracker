#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <string.h>

#include "common.h"


extern CommonData commonData1;
extern CommonData commonData2;

typedef struct 
{
    char cpuName[8];
    int user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice;
} CpuData;


void *analyzerThread(void){

    CpuData previous[MAX_CPUS];
    CpuData current[MAX_CPUS];
    bool first = true;

    while (true) 
    {
        printf("Analyzing...\n");
        
        char buf[DATASIZE];
        int bufLines = 0;

        sem_wait(&commonData1.full);                                        // lock

        if (memcpy(buf, commonData1.buf, DATASIZE) == NULL){        // copy from commonData1, critical!
            perror("memcpy error\n");
            exit(EXIT_FAILURE);
        }
        bufLines = commonData1.bufLines;

        sem_post(&commonData1.empty);                                       // relase
            

        int bufLen = 0;

        for (int line = 0; line < bufLines; line++) {                           // read the whole line to local vars
            sscanf(buf + bufLen,
                "%s %d %d %d %d %d %d %d %d %d %d\n",
                current[line].cpuName,
                &current[line].user,
                &current[line].nice,
                &current[line].system,
                &current[line].idle,
                &current[line].iowait,
                &current[line].irq,
                &current[line].softirq,
                &current[line].steal,
                &current[line].guest,
                &current[line].guest_nice);   

            while (buf[bufLen++] != '\n');                                      // go to next line

        }

        if (!first) {                                                           // if previous struct exists, calculate usage
            memset(buf, 0, DATASIZE);
            bufLen = 0;

            for (int line = 0; line < bufLines; line++) {
                
                // PrevIdle = previdle + previowait
                int prevIdle = previous[line].idle + previous[line].iowait;
                
                // Idle = idle + iowait
                int currIdle = current[line].idle + current[line].iowait;

                // PrevNonIdle = prevuser + prevnice + prevsystem + previrq + prevsoftirq + prevsteal
                int prevTime = previous[line].user + previous[line].nice + previous[line].system + previous[line].irq + previous[line].softirq + previous[line].steal;
                
                // NonIdle = user + nice + system + irq + softirq + steal
                int currTime = current[line].user + current[line].nice + current[line].system + current[line].irq + current[line].softirq + current[line].steal;

                // PrevTotal = PrevIdle + PrevNonIdle
                prevTime += prevIdle;

                // Total = Idle + NonIdle
                currTime += currIdle;

                // differentiate: actual value minus the previous one
                // totald = Total - PrevTotal
                currTime -= prevTime;

                // idled = Idle - PrevIdle
                currIdle -= prevIdle;

                // CPU_Percentage = (totald - idled)/totald
                float cpuUsage = (float)(currTime - currIdle)*100/(float)currTime; 

                bufLen += snprintf(buf + bufLen, 32, "%s %f%%\n", current[line].cpuName, cpuUsage);
            }

            sem_wait(&commonData2.empty);                                               // lock

            if (memcpy(commonData2.buf, buf, DATASIZE) == NULL){                // copy to commonData2, critical!
                perror("memcpy error\n");
                exit(EXIT_FAILURE);
            }
            commonData2.bufLines = bufLines;

            sem_post(&commonData2.full);                                                // relase
        }

        memcpy(previous, current, sizeof(current));
        first = false;
    }
    return EXIT_SUCCESS;
}
