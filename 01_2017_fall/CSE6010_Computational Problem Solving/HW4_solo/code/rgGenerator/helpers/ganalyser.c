/*
 ============================================================================
 Name        : ganalyser.c
 Author      : Soo Hyeon Kim
 Version     : 10/21/17
 Copyright   : © 2017 Soo Hyeon Kim. All rights reserved.
 Student name: Soo Hyeon Kim
 Username	 : skim3026 (#:903230437)
 ============================================================================
 */


#include <stdio.h>
#include <stdlib.h>

#include "stack.h"
#include "ganalyser.h"


gNode**  V=NULL;

Stack* S=NULL;

void DFS(GraphType** G, unsigned long N)
{
    unsigned int i;
    
    //create an array of nodes
    V=(gNode**)malloc(N*sizeof(gNode*));
    
    //create a stack
    S=CreateStack();
    
    //initialize the nodes
    for(i=0;i<N;i++){
        
        V[i]=(gNode*)malloc(sizeof(gNode));
        
        V[i]->nodeID=i;
        
        V[i]->nodeColor=WHITE;
        
        V[i]->conCount=0;
    }
    
    //visit each node, if it has not been visited before
    for(i=0;i<N;i++){
        
        if(V[i]->nodeColor==WHITE){
            
            DFS_VISIT(G,N,V[i]);
        }
    }
    
    unsigned int largestSize=V[0]->conCount;
    
    //find the node with the largest component
    for(i=1;i<N;i++){
        
        if(largestSize<V[i]->conCount){
            
            largestSize=V[i]->conCount;
        }
    }
    
    printf("-------------------------------------------------\n");
    printf("The size of the largest component=%d\n",largestSize);
    printf("-------------------------------------------------\n");
    
    for(i=0;i<N;i++){
        free(V[i]);
    }
    
    free(V);
}

/*void DFS_VISIT(GraphType** G, unsigned long N, gNode* u){
 
 unsigned int j;
 
 u->nodeColor=GRAY;
 
 for(j=0;j<N;j++){
 
 //printf("visiting node");
 
 if(G[u->nodeID][j]!=0 && V[j]->nodeColor==WHITE){
 
 DFS_VISIT(G,N,V[j]);
 
 u->conCount+=V[j]->conCount;
 }
 }
 
 u->nodeColor=BLACK;
 
 }*/

void DFS_VISIT(GraphType** G, unsigned long N, gNode* u) {
    
    unsigned int j;
    
    unsigned int counter=0;
    
    StackPush(S,u);
    u->stackEntryTimeStamp=counter++;
    
    u->nodeColor=GRAY;
    
    gNode* topNode=u;
    
	   while(IsStackEmpty(S)==0){
           
           //insert its connected nodes to the stack
           for(j=topNode->nodeID;j<N;j++){
               
               if(G[topNode->nodeID][j-topNode->nodeID]!=0 && V[j]->nodeColor==WHITE){
                   
                   StackPush(S,V[j]);
                   
                   //increase the counter after each push
                   V[j]->stackEntryTimeStamp=counter++;
                   
                   V[j]->nodeColor=GRAY;
               }
           }
           
           //pop the stack top node
           topNode=( gNode*)StackPop(S);
           
           //calculate the number of connected components
           topNode->conCount=counter-(topNode->stackEntryTimeStamp);
       }
}

void PrintResult(unsigned int N)
{
    unsigned int i;
    
    for (i=0;i<N;i++)
    {
        printf("connected components with node %d=%d\n",i, V[i]->conCount);
    }
}

