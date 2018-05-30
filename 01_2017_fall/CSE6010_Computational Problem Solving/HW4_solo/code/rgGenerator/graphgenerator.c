/*
 ============================================================================
 Name        : graphgenerator.c
 Author      : Soo Hyeon Kim
 Version     : 10/22/17
 Copyright   : © 2017 Soo Hyeon Kim. All rights reserved.
 Student name: Soo Hyeon Kim
 Username	 : skim3026 (#:903230437)
 ============================================================================
 */



#include <stdio.h>
#include <stdlib.h>

#include  <string.h>
#include <time.h>

#include "graphgenerator.h"

#include "omp.h"


//a matrix to store the graph connection
GraphType** AdjMatrix=NULL;

GraphType** GetGraoh(){return AdjMatrix;}


void DeleteGraph(){
    //free the allocated memory
    free(AdjMatrix);}

int GenerateGraph(unsigned long N, double P)
{
    unsigned long i;
    
    //initialize the random number generator
    srand (time(NULL));
    
    //this is only required on the sequential version
    for(i=0;i<N;i++){
        
        AdjMatrix[i]=(GraphType*)malloc((N-i)* sizeof(GraphType));
    }
    
    
    //generate a graph for a range of indices
    return GenerateGraphRange(0,N,P);
}

int ParaGenerateGraph(unsigned long N, double P,int ThrdCount)
{
    
    
    //set the number of threads
    omp_set_num_threads(ThrdCount);
    
    //printf("note: we use %d threads to generate the graph!\n",ThrdCount);
    
#pragma omp parallel
    {
        unsigned int i=0;
        
        //get some useful information
        int currentThreadID=omp_get_thread_num();
        int totalThreads=omp_get_num_threads();
        
        //seed for each thread
        unsigned int seed = time(NULL);
        
        // printf("thread %d start to work\n",currentThreadID);
        
        /*each thread will take a row to generate interchangeably.
         * For example if we have two threads, then thread 0 will generate
         * rows 0, 2, 4, 6 ... etc, while thread 1 will generate rows
         * 1, 3, 5, ...etc.
         */
        //#pragma omp parallel for schedule(static,totalThreads)
        for(i=currentThreadID;i<N;i+=totalThreads){
            
            //printf("thrd=%d, i=%lu\n",currentThreadID,i);
            GenerateGraphRow(i, P, N,&seed);
        }
        
    }
    
    return GEN_SUCESS;
}

int GenerateGraphRow(unsigned int rowID, double P, unsigned long N, unsigned int* seed)
{
    unsigned long j;
    
    //GraphType* gRow=AdjMatrix[rowID];
    GraphType* gRow=(GraphType*)malloc((N-rowID)*sizeof(GraphType));
    
    
    /**two conditions must be hold in order to create an edge between  nodes i and j
     *  (i) there is no edge between a node and itself
     *  (ii) the probability of generating an edge is less than P
     */
    for(j=rowID;j<N;j++){
        
        if(rowID!=j && Random_P(seed)<=P){
            
            gRow[j-rowID]=1;
        }
        else{
            
            gRow[j-rowID]=0;
        }
    }
    
    //assign this row to the matirx
    AdjMatrix[rowID]=gRow;
    
    return GEN_SUCESS;
}


int GenerateGraphRange(unsigned long startIndex, unsigned long endIndex, double P)
{
    unsigned long i, j;
    
    
    for(i=startIndex;i<endIndex;i++){
        
        //since we consider undirected graph, we should repeat the edge generation
        //between symmetric vertices (e.g., AdjMatrix[i][j]=AdjMatrix[j][i])
        for(j=i;j<endIndex;j++){
            
            /**two conditions must be hold in order to create an edge between  nodes i and j
             *  (i) i not equal j: there is no edge between a node and itself
             *  (ii) the probability of generating an edge is less than P
             */
            if(i!=j && Random()<=P){
                
                AdjMatrix[i][j-i]=1;
            }
            else{
                
                AdjMatrix[i][j-i]=0;
            }
        }
    }
    
    return GEN_SUCESS;
}

double Random()
{
    
    double u;
    u = rand() / (RAND_MAX + 1.0);
    
    return u;
}

double Random_P(unsigned int* seed)
{
    
    double u;
    u = rand_r(seed) / (RAND_MAX + 1.0);
    
    return u;
}

int AllocateMemory(unsigned long N)
{
    //allocate memory for the graph
    AdjMatrix=(GraphType**)malloc(N* sizeof(GraphType*));
    
    if(AdjMatrix==NULL){
        
        printf("Error: cannot allocate memory!\n");
        
        return GEN_FAILURE;
    }
    
    //TODO: for the parallel code we will not do this
    
    //allocate the individual rows
    //please note we allocate only part of the row because the other part is symmetric
    /*for(int i=0;i<N;i++){
     AdjMatrix[i]=(GraphType*)malloc((N-i)* sizeof(GraphType));
     }*/
    
    return GEN_SUCESS;
}

