/**
 * @file    score_analyser.h
 * @author  Saminda Wijeratne <samindaw@gatech.edu>
 * @brief   Implement serial functions required to calculate grades
 *
 * Copyright (c) 2018 Georgia Institute of Technology. All Rights Reserved.
 */


#include "score_analyser.h"
#include <stdlib.h>
#include <stdio.h>

double get_max_score(double* scores, int n){
	double max=scores[0];
	
	for (int i=1;i<n;i++)
		if(max<scores[i])
			 max=scores[i];
	
    return max;
}

double get_min_score(double* scores, int n){
	double min=scores[0];
	
	for (int i=1;i<n;i++)
		if(min>scores[i])
			 min=scores[i];
	
    return min;
}

double get_average_score(double* scores, int n){
	double sum=0;
	for(int i=0;i<n;i++)
		sum +=scores[i];
	
    return (sum/n);
}

void calculate_grades(double* scores, int n, int* grades){
int i;
	
        double minScore=get_min_score(scores,n);
	double maxScore=get_max_score(scores,n);
	double avgScore=get_average_score(scores,n);
	
	
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

void calculate_grade_count(double* scores, int n, int** grade_counts, int* no_of_grades){
int i;
	
int* grades;
int gi;
	
	(*no_of_grades)=5;
		
	grades = (int*) malloc(sizeof(int) * n);
	calculate_grades(scores,n,grades);
	
	 //allocate memory
	*grade_counts = (int*) malloc(sizeof(int) *(*no_of_grades));
	
	for(i=0;i<(*no_of_grades);i++)
	   (*grade_counts)[i]=0;
	
	for(i=0;i<n;i++){
		gi=grades[i];
		(*grade_counts)[gi]++;
	}
	
	free(grades);
}




