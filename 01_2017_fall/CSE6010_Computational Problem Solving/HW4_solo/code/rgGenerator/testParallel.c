/*
 ============================================================================
 Name        : testParallel.c
 Author      : Soo Hyeon Kim
 Version     : 10/23/17
 Copyright   : © 2017 Soo Hyeon Kim. All rights reserved.
 Student name: Soo Hyeon Kim
 Username	 : skim3026 (#:903230437)
 ============================================================================
 */


#include <stdio.h>
#include <stdlib.h>

#include  <string.h>
#include <time.h>
#include <math.h>

#include <unistd.h>

#include "graphgenerator.h"
#include "helpers/ganalyser.h"
#include "omp.h"
#include "sys/time.h"

//execute a singe parallel run
void ExecuterRun(int N, double P, int ThrdCount);

float firstRunTime=1;
int main (int ArgC, char ** ArgV)
{
    
    printf("K\tN\tSpeedup(K)\n");
    for (int N=100; N<=10000;N *=10){
        printf("------------N=%d-------------------\n",N);
        for(int T=1;T< 5;T++)
        {
            //allocate memory
            int retCode=AllocateMemory(N);
            
            if(retCode!=GEN_SUCESS)  return 0;
            
            ExecuterRun(N,0.5,T);
            
            //free the memory
            DeleteGraph();
        }
    }
    
    return 0;
}

void ExecuterRun(int N, double P, int ThrdCount)
{
    struct timeval start, end;
    
	   //used to measure the runtime
    //double t = gettimeofday();
    gettimeofday(&start, NULL);
    
	   ParaGenerateGraph(N,P,ThrdCount);
    
	   //record the end time
	   //float diff = gettimeofday()-t;
    gettimeofday(&end, NULL);
    int diff=((end.tv_sec * 1000000 + end.tv_usec)
              - (start.tv_sec * 1000000 + start.tv_usec));
    
	   if(ThrdCount==1) firstRunTime=diff;
    
	   double k=firstRunTime/diff;
    
	   //printf("%d\t%f\t%f\n",ThrdCount,diff,k);
    printf("%d\t%d\t%f\n",ThrdCount,diff,k);
}
