/*
 ============================================================================
 Name        : rgGenerator.c
 Author      : Soo Hyeon Kim
 Version     : 10/21/17
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


/*
 * This program generates a random graph. The user enters N, P, the number of
 * graph vertices and the probability that an edge exists between two nodes, respectively.
 * After that the code generate and possibly dump the graph contents into a file
 * Usage: rgGenerator N P <-dump>, where:
 * N:  the number of vertices
 * P:  edge probability
 * -dump:  an optional command to dump the generated graph into a file
 */

//a structure to hold the program commands after parsing them
struct Commands{
    
    unsigned long N;            //the number of nodes
    float         P;           //edge generation probability
    int           ThrdCount;   //number of threads to use
    int           dumpToFile;  //should we dump the result to a file
};

typedef struct Commands rgCommands;


//show the graph on the screen
void DisplayGraph(unsigned long N);

//write the graph into file
void WriteToFile(unsigned long N, double P);


/** parse the given commands in the prompt and returns them in a structure
 * cCount: number of commands
 * cString commands in a string format
 */
rgCommands ParseCommands(int cCount, char ** cString);

//** the main function
int main (int ArgC, char ** ArgV)
{
    //parse the given commands
    rgCommands rgCmds=ParseCommands(ArgC,ArgV);
    
    //convert N, P into numbers
    unsigned long N=rgCmds.N;
    double P=rgCmds.P;
    
    printf("the graph will be generated with the following configuration:- N=%lu and P=%.3f\n",N,P);
    
    
    //allocate memory
    int retCode=AllocateMemory(N);
    
    if(retCode!=GEN_SUCESS)  return 0;
    
    //used to measure the runtime
    double t = omp_get_wtime();
    
    //initialize the random number generator
    srand (time(NULL));
    
    //if the number of threads more than one, we will use the parallel version
    if(rgCmds.ThrdCount>0){
        
        retCode=ParaGenerateGraph(N,P,rgCmds.ThrdCount);
    }
    else{
        
        //run the graph generation sequentially
        retCode=GenerateGraph(N,P);
    }
    
    //record the end time
    float diff = omp_get_wtime()-t;
    
    
    if(retCode!=GEN_SUCESS){
        
        printf("Error: unexpected error has occurred while generating the graph!\n");
        
        DeleteGraph();
    }
    
    //show the result on the screen
    DisplayGraph(N);
    
    //write the graph to a file
    if(rgCmds.dumpToFile==1){
        
        WriteToFile(N,P);
        
        printf("The graph has been written to the file ./dump.txt\n");
    }
    
    //display the runtime information
    printf("It took %f sec. to generate this graph (displaying output not counted).\n",diff);
    
    GraphType** Graph=GetGraoh();
    
    //Analyze the graph
    DFS(Graph,N);
    
    DeleteGraph();
    
    return 0;
}


void DisplayGraph(unsigned long N)
{
    unsigned long i, j;
    
    GraphType** Graph=GetGraoh();
    
    
    if(N>100){
        printf("For graphs with more than 100 nodes, we do not display them on the screen.\n");
        printf("Please use the dump option to save it to a file.\n");
        return;
    }
    
	   for (i=0;i<N;i++){
           
           for(j=0;j<N;j++){
               
               if(j<i)
                   printf("  ");
               
               else
                   printf("%d ",Graph[i][j-i]);
               
           }
           
           //print a new line
           printf("\n");
       }
}


void WriteToFile(unsigned long N, double P)
{
    FILE *fp=NULL;
    GraphType** Graph=GetGraoh();
    
	   //open the file for writing
	   fp = fopen("dump.txt", "w+");
    
	   if(fp==NULL){
           printf("cannot open the file for writing\n");
           return;
       }
    
    fprintf(fp,"------------------------------------\n");
    fprintf(fp,"This graph has been generated with the following configuration:- N=%lu and P=%.3f\n",N,P);
    fprintf(fp,"------------------------------------\n");
    
	   unsigned long i, j;
    
	   for (i=0;i<N;i++){
           
           for(j=0;j<N;j++){
               
               if(j<i)
                   fprintf(fp,"   ");
               else
                   fprintf(fp,"%d ",Graph[i][j-i]);
               
           }
           
           //print a new line
           fprintf(fp,"\n");
       }
}

rgCommands ParseCommands(int cCount, char ** cString)
{
	   //the program name and N, P
    if(cCount<5){
        
        printf("Usage: ./rgGenerator -n N -p P <-t T> <-d>\n");
        printf("For example: ./rgGenerator -n 5 -p 0.5 -t 1 -d\n");
        printf("will generate a graph of 5 nodes, using 0.5 edge generation probability, using 5 threads and dump the result to a file\n");
        
        exit(0);
    }
    
    rgCommands  cmds;
    int opt = 0;
    
    char* cmdValue;
    
    //default values
    cmds.N=10;
    cmds.P=0.5;
    cmds.ThrdCount=1;
    cmds.dumpToFile=0;
    
    //parse the commands
    while ((opt = getopt(cCount, cString, "n:p:t:d")) != -1) {
        
        switch(opt){
                
            case 'n':
                cmdValue = optarg;
                cmds.N=atoi(cmdValue);
                break;
            case 'p':
                cmdValue = optarg;
                cmds.P=atof(cmdValue);
                break;
            case 't':
                cmdValue = optarg;
                cmds.ThrdCount=atoi(cmdValue);
                break;
            case 'd':
                cmdValue = optarg;
                cmds.dumpToFile=1;
                break;
            default:
                printf("invalid option -%d",opt);
                exit(0);
        }
        
    }
    
    return cmds;
}
