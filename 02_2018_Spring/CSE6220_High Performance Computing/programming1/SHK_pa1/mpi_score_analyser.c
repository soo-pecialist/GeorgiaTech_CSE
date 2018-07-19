/**
 * @file    mpi_score_analyser.c
 * @author  Saminda Wijeratne <samindaw@gatech.edu>
 * @brief   Implements parallel functions required to calculate grades
 *
 * Copyright (c) 2018 Georgia Institute of Technology. All Rights Reserved.
 */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "mpi_score_analyser.h"

//out helper functions
int get_score_per_process(int p, int size, int n)
{
       if(n%size==0)
          return n/(float)size;

       if(p< (n%size))
         return (floor(n/(float)size)+1);
             
       return floor(n/(float)size);
}

void decompose(int p, int size, int n, int *count, int *offset)
{
int i;
int local_score=0;

       if(n%size==0){
          local_score=get_score_per_process(i,size,n);

           for(i=0;i<size;i++){
                count[i]=local_score;
                offset[i]=i*local_score;
            }
        }
       else{
          int current_Offset=0;
          for(i=0;i<size;i++){
              local_score=get_score_per_process(i,size,n);
              count[i]=local_score;
              offset[i]=current_Offset;
              current_Offset +=local_score;
          }

      }

}

void mpi_distribute_scores(double* all_scores, int n, double** scores, int* local_n, MPI_Comm comm){
       // Hint: Lookup the usage of MPI scatter functions
        int process_count=0;
        int process_rank=0;
	int scores_per_process=0;

        //the offset and coounts for each array
        int* count=NULL;
        int* offset=NULL;
	
	MPI_Comm_size(comm,&process_count);
        MPI_Comm_rank(comm,&process_rank);

        //allocate memory for the counts and offsets
        count=(int*)malloc(sizeof(int)*process_count);
        offset=(int*)malloc(sizeof(int)*process_count);

        //disribute the number of elements 
	MPI_Bcast(&n, 1, MPI_INT, 0, comm);

        //when the number of scores are divisable by the number of processes
        scores_per_process=get_score_per_process(process_rank,process_count,n);

	//update the local size
	(*local_n)=scores_per_process;
	
	//allocate memory for this process scores
	*scores = (double*) malloc(sizeof(double)*scores_per_process);
	
	
        if(process_rank==0){

            //determine the number of elements for each process
            decompose(process_rank,process_count,n,count,offset); 
        }

	MPI_Scatterv (all_scores, count, offset, MPI_DOUBLE, *scores,
                                   scores_per_process, MPI_DOUBLE, 0, comm);

      //free memory
      free(count);
      free(offset);
}

double mpi_get_max_score(double* scores, int n, MPI_Comm comm){
	// Hint: Lookup usage of functions like MPI_Scan, MPI_Reduce, 
	//       MPI_Allreduce, MPI_Bcast
	
	double  localMax=0;
	double  gloabalMax=0;
	int i=0;
	
	localMax=scores[0];
	for(i=1;i<n;i++)
	  if(localMax<scores[i])
		 localMax=scores[i];
	
	MPI_Allreduce(&localMax,&gloabalMax,1,MPI_DOUBLE,MPI_MAX,comm);
	
    return gloabalMax;
}

double mpi_get_min_score(double* scores, int n, MPI_Comm comm){
	double  localMin=0;
	double  gloabalMin=0;
	int i=0;
	
	localMin=scores[0];
	for(i=1;i<n;i++)
	  if(localMin>scores[i])
		 localMin=scores[i];
	
	MPI_Allreduce(&localMin,&gloabalMin,1,MPI_DOUBLE,MPI_MIN,comm);
	
    return gloabalMin;        
}

double mpi_get_average_score(double* scores, int n, MPI_Comm comm){
	
	double  local_sum=0;
	double  global_sum=0;
        int  global_n=0;
	
	int process_count=0;	
	MPI_Comm_size(comm,&process_count);
	
	for(int i=0;i<n;i++)
            local_sum +=scores[i];
	
	//reduce the sum of scores
        MPI_Allreduce(&local_sum,&global_sum,1,MPI_DOUBLE,MPI_SUM,comm);

        //reduce the sum of elements
        MPI_Allreduce(&n,&global_n,1,MPI_INT,MPI_SUM,comm);
	
    return global_sum/global_n;      
}

void mpi_calculate_grades(double* scores, int n, int* grades, MPI_Comm comm){
	// Hint: If you use mpi_get_max_score(..), mpi_get_min_score(..) and mpi_get_average_score(..) functions
	//		you'd not need to call any MPI functions inside this function
	
	int i=0;
	
	double maxScore=mpi_get_max_score(scores,n,comm);
	double minScore=mpi_get_min_score(scores,n,comm);
	double avgScore=mpi_get_average_score(scores,n,comm);
	
	double scored1=maxScore-avgScore;
	double scared2=avgScore-minScore;
	
	//calculate the grades
	for(i=0;i<n;i++){

		if( (scores[i]>= (2*scored1/3 +avgScore)) && (scores[i]<=maxScore)){
			grades[i]=0;
		}
		else if( (scores[i]>= (scored1/3 +avgScore)) && (scores[i] < (2*scored1/3+avgScore))){
			grades[i]=1;
		}
		else if( (scores[i]< (scored1/3 +avgScore)) && (scores[i]>= avgScore)){
			grades[i]=2;
		}
		else if( (scores[i]>= (minScore+scared2/5)) && (scores[i]< avgScore) ){
			grades[i]=3;
		}
		else
		{
			grades[i]=4;
		}
	}
	
}

void mpi_calculate_grade_count(double* scores, int n, int** grade_counts, int* no_of_grades, MPI_Comm comm){
    // Pseudo code:
    //		1. calculate grade for each score
    //		2. define/update the no_of_grades available (eg: the assignment requires 5. i.e. A,B,C,D,F)
    //		3. allocate memory for grade_counts and reset the counts to 0
    //		4. calculate local grade counts
    //		5. update grade_counts by calculating the global grade counts
	
int* grades;
int* globalCount;
int i=0;
int gi;
	
	grades = (int*) malloc(sizeof(int) * n);
	mpi_calculate_grades(scores,n,grades,comm);
	
	(*no_of_grades)=5;
	
	 //allocate memory
	*grade_counts = (int*) malloc(sizeof(int) *(*no_of_grades));
	
	for(i=0;i<(*no_of_grades);i++)
	   (*grade_counts)[i]=0;
	
	for(i=0;i<n;i++){
		gi=grades[i];
		(*grade_counts)[gi]++;
	}
	
	free(grades); 
	
	//TODO: dynamically allocate this
         globalCount=(int*) malloc(sizeof(int) * (*no_of_grades));
	
	//collect the data
	MPI_Reduce(*grade_counts,globalCount,(*no_of_grades), MPI_INT, MPI_SUM,0,comm);
	
	for(i=0;i<(*no_of_grades);i++)
	   (*grade_counts)[i]=globalCount[i];

        free(globalCount);
}
