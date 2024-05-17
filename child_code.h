#pragma once
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <sys/time.h>
#include <limits.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include "array_list.h"

#define MAX_SOL 800

typedef struct result {
    int numSol;
    ArrayList solution[MAX_SOL];
    int numIter;
    struct timeval time;
} Result;

void readGenerateBoard(char* fileName, ArrayList* graph, int* totalVertices, int* avenues, int* streets, int* superMarkets, int* citizens, int* maxSol);
void addEdge(ArrayList g[],int from,int to);
int findCitizenSolution(ArrayList* graph, int* totalVertices, int* visitedToken, ArrayList* visited, ArrayList solution[]);
bool isSuperMarketNode(ArrayList* graph, int* nextNode, int* target);
int findCitizenSolutionAleatory(ArrayList* graph, int* maxSol, int* totalVertices, ArrayList solution[]);
