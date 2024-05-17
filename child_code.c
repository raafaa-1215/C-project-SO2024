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

void addEdge(ArrayList g[],int from,int to);
bool isSuperMarketNode(ArrayList* graph, int* nextNode, int* target);
int findCitizenSolution(ArrayList* graph, int* totalVertices, int* visitedToken, ArrayList* visited, ArrayList* solution);

void readGenerateBoard(char* fileName, ArrayList* graph, int* totalVertices, int* avenues, int* streets, int* superMarkets, int* citizens, int* maxSol){
    int avenue, street;
    int pos;

    FILE *f;
    f = fopen(fileName,"r");
    if (f == NULL) {
        printf("Error reading file!\n");
        exit(1);
    }

    fscanf(f,"%d %d",avenues,streets);
    fscanf(f,"%d %d",superMarkets,citizens);

    *totalVertices = (*avenues) * (*streets) +2;

    for (int i =0; i < (*totalVertices); i++){
        graph[i] = createArrayList();
    }

    for (int i =1; i < (*totalVertices)-1; i++){
        // North arcs
        if((i-(*avenues)) > 0){
            addEdge(graph,i,i-(*avenues));
        }
        // East arcs
        if (((i-1) % (*avenues)) != 0){
            addEdge(graph,i,i-1);
        }
        // West arcs
        if ((i % (*avenues)) != 0){
            addEdge(graph,i,i+1);
        }
        // South arcs
        if((i+(*avenues) < (*totalVertices)-1)){
            addEdge(graph,i,i+(*avenues));
        }
    }
    
    // Atributes supermarkets to graph[10] position
    for (int i =0; i < (*superMarkets); i++){
        fscanf(f,"%d %d",&avenue,&street);
        pos = (street-1) * (*avenues) + avenue;
        addEdge(graph,pos,(*totalVertices)-1);
    }
    // Atributes citizens to graph[1] position
    for (int i =0; i < (*citizens); i++){
        fscanf(f,"%d %d",&avenue,&street);
        pos = (street-1) * (*avenues) + avenue;
        addEdge(graph,0,pos);
    }    

    // Calculates maximum of solutions
    *maxSol = ((*superMarkets) < (*citizens))? (*superMarkets):(*citizens);

    fclose(f);
}

int findCitizenSolutionAleatory(ArrayList* graph, int* maxSol, int* totalVertices, ArrayList solution[]){
    int visitedToken = 0;
    ArrayList visited = createArrayList();
    for (int i=0; i < (*totalVertices); i++){
        append(&visited, 0);
    }

    for (int i = 0; i < (*maxSol); i++){
        solution[i] = createArrayList();
    }

    int val;
    do{
        val = findCitizenSolution(graph, totalVertices, &visitedToken, &visited, solution);
    } while ((val != 0) && ((*maxSol) != (visitedToken-1)));
    
    return visitedToken-1;
}

int findCitizenSolution(ArrayList* graph, int* totalVertices, int* visitedToken, ArrayList* visited, ArrayList solution[]){
    ArrayList visitedList = createArrayList();
    int target = (*totalVertices) - 1;
    ArrayList nextList = createArrayList();
    copyArrayList(graph[0], &nextList);
    (*visitedToken)++;
    append(&visited[0], (*visitedToken));

    if(nextList.size == 0){
        return 0;
    }

    int nextNode;
    do{
        // selects a random position in the nextList
        int pos = rand() % nextList.size;
        nextNode = get(nextList, pos);
        if(isSuperMarketNode(graph, &nextNode, &target) == true){
            append(&visitedList, nextNode);
            break;
        }
        if(get(*visited, nextNode) != (*visitedToken)){
            append(&visitedList, nextNode);
            set(visited, nextNode, (*visitedToken));
            copyArrayList(graph[nextNode], &nextList);
        } else if(get(*visited, nextNode) == (*visitedToken)){
            deleteByValue(&nextList, nextNode);
        }
    } while(nextList.size != 0);

    copyArrayList(visitedList, &solution[(*visitedToken)-1]);

    // remove arcs if a solution for a citizen is found
    if (isSuperMarketNode(graph, &nextNode, &target)){
        for(int v=0; v < visitedList.size; v++){
            for (int i=0; i < (*totalVertices); i++){
                if (contains(graph[i], get(visitedList, v))){
                    deleteByValue(&graph[i], get(visitedList, v));
                }
            }
        }
        deleteByValue(&graph[nextNode], target);
        nextNode = target;
    }

    if(nextNode == target){
        return 1;
    } else {
        return 0;
    }
}

bool isSuperMarketNode(ArrayList* graph, int* nextNode, int* target){
    return (contains(graph[(*nextNode)], (*target)) == 1)? true:false;
}

void addEdge(ArrayList g[],int from,int to){
    append(&g[from],to);
}