#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <sys/time.h>
#include <limits.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <signal.h>
#include "child_code.h"

#define MAX_NODES 700

int getTimeCalc();

int main(int argc, char *argv[])
{   
    int protection = PROT_READ | PROT_WRITE;
    int visibility = MAP_ANONYMOUS | MAP_SHARED;
    Result *shmem = (Result*) mmap(NULL, sizeof(Result), protection, visibility, 0, 0);

    sem_unlink("mymutexchild");
    sem_t *sem_child = sem_open("mymutexchild", O_CREAT, 0644, 1);
    int numProcesses;

    ArrayList graph[MAX_NODES];

    int avenues, streets;
    int superMarkets, citizens;

    int totalVertices;
    int maxSol;
    int pos;
   
    char fileName[60] = "./file_tests/";
    char argName[20];
    int msecToEnd;
    struct timeval tv1, tv2, tv_res;

    if(argc == 1){
        strcpy(argName, "instances_2.txt");
        numProcesses = 8;
        msecToEnd = 100;
    } else if(argc == 4){
        strcpy(argName, argv[1]);
        sscanf(argv[2], "%d", &numProcesses);
        sscanf(argv[3], "%d", &msecToEnd);
    } else{
        printf("Usage: main <test_name> <number processes> <time(ms)>\n");
    }

    strcat(fileName, argName);

    Result res;
    res.numSol = 0;
    struct timeval tvs;
    tvs.tv_sec = 0;
    tvs.tv_usec = 0;
    res.time = tvs;
    *shmem = res;
    
    ArrayList solution[MAX_NODES];
    
    int pids[MAX_NODES];

    int runTime = 0;
    gettimeofday(&tv1,NULL);

    for(int i=0; i < numProcesses; i++){
        pids[i] = fork();
        if (pids[i] == 0){
            struct timeval tvi, tvf, tv_res_res;
            gettimeofday(&tvi,NULL);
              
            srand(time(NULL) * getpid());
            int iter = 0;
            while (1) {
                readGenerateBoard(fileName, graph, &totalVertices, &avenues, &streets, &superMarkets, &citizens, &maxSol);
                int sol = findCitizenSolutionAleatory(graph, &maxSol, &totalVertices, solution);
                sem_wait(sem_child);
                // update shared memory
                if (shmem->numSol < sol) {
                    gettimeofday(&tvf,NULL);
                    timersub(&tvf,&tvi,&tv_res_res);
                    res.numSol = sol;
                    for (int j=0; j < sol; j++){
                        copyArrayList(solution[j], &res.solution[j]);
                    }
                    res.time = tv_res_res;
                    res.numIter = iter + 1;
                    
                    *shmem = res;
                }
                
                
                sem_post(sem_child);
                iter++;
            }
        }
    }
    usleep(msecToEnd * 1000);
    for (int i=0; i< numProcesses; i++)
        kill(pids[i],SIGKILL);
    
    printf("Ficheiro: %s\n", argName);
    printf("Numero de Soluções: %d\n", shmem->numSol);
    for (int i = 0; i < shmem->numSol; i++)
    {
        printArrayList(shmem->solution[i]);
    }
    printf("O programa filho demorou %.3f ms a encontrar a solução\n", (float) (shmem->time.tv_sec*1000 + shmem->time.tv_usec/1000));
    printf("Foram executadas %d iterações\n", shmem->numIter);
    
    // caculate parent run time
    gettimeofday(&tv2,NULL);
    timersub(&tv2,&tv1,&tv_res);
    printf("O programa pai demorou %ld ms a correr\n", tv_res.tv_sec*1000 + tv_res.tv_usec/1000);
    return 0;
}


