/**
 * @file    jacobi.cpp
 * @author  Patrick Flick <patrick.flick@gmail.com>
 * @brief   Implements matrix vector multiplication and Jacobi's method.
 *
 * Copyright (c) 2014 Georgia Institute of Technology. All Rights Reserved.
 */
#include "jacobi.h"

/*
 * TODO: Implement your solutions here
 */

// my implementation:
#include <iostream>
#include <math.h>
#include <cstdlib>

//fill a matrix with zero
void fill_with_zeros(double *m, const int size);
void find_D_and_R(const double* x, double* D, double* R,const int n);
double norm(const double *A, const double *x, const double *b, int n, double* temp);
void update_x(double *x,  double *A, double *R, double *D, double *b, int n,double* temp);

// Calculates y = A*x for a square n-by-n matrix A, and n-dimensional vectors x
// and y
void matrix_vector_mult(const int n, const double* A, const double* x, double* y)
{
int i,j;
	
  for(i=0;i<n;i++){
	  y[i]=0;
	 for(j=0;j<n;j++)
		y[i] +=A[i*n+j]*x[j];
  }
}

// Calculates y = A*x for a n-by-m matrix A, a m-dimensional vector x
// and a n-dimensional vector y
void matrix_vector_mult(const int n, const int m, const double* A, const double* x, double* y)
{
 int i, j;
	
    for (i =0; i < n; i++){
		y[i] = 0;
		for (j = 0; j < m; j++){
			y[i] += A[i*n+j]*x[j];
		}
	}
}

// implements the sequential jacobi method
void jacobi(const int n, double* A, double* b, double* x, int max_iter, double l2_termination)
{
  double *D, * R;
  double* temp;
  int i;
  int iter=0;
  double norm2=0;
	
  //allocate memory for D and R using a row major indexing
  D=(double*)malloc(sizeof(double)*n*n);
  if(D==NULL) return;
  R=(double*)malloc(sizeof(double)*n*n);
  if(R==NULL) return;
	
  temp=(double*)malloc(sizeof(double)*n);
  if(temp==NULL) return;

  //////////////////////////////////initialization /////////////////////////////
  //set the values of D and R elements to zeros
  fill_with_zeros(D,n);
  fill_with_zeros(R,n);

  //initialize x to zeros
  for(i=0;i<n;i++) x[i]=0;
	
  //find D and R
  find_D_and_R(A,D,R,n);

  ////////////////////////////////////// main loop ////////////////////////////////
  do {
    //update x
	update_x(x, A, R, D, b, n,temp);
	  
	norm2=norm(A,x,b,n, temp);
  }while(sqrt(norm2)>=l2_termination && iter++<max_iter);

  ////////////////////////////////////// finalizing ////////////////////////////////
  //free memory
  free(D);
  free(R);
  free(temp);
}

void fill_with_zeros(double *m, const int n)
{
int i,j;
  for(i=0;i<n;i++)
	for(j=0;j<n;j++)
	  m[i*n+j]=0;
}

void find_D_and_R(const double* A, double* D, double* R,  const int n)
{
int i,j;
	
  for(i=0;i<n;i++){
    D[i]=A[i*n+i];
    for(j=0;j<n;j++)
	  if(i!=j) R[i*n+j]=A[i*n+j];
  }
}

double norm(const double *A, const double *x, const double *b, int n,double* temp)
{
double norm_val=0;
double diff;
	
 int i;
	matrix_vector_mult(n,A,x,temp);
	for(i=0;i<n;i++){
		diff=temp[i] - b[i];
		norm_val += diff*diff;
	}
	
  return norm_val;
}

void update_x(double *x, double *A, double *R, double *D, double *b, int n,double* temp)
{
int j=0;
	
  matrix_vector_mult(n, R, x, temp); 
	
  for (j = 0; j < n; j++){
    	x[j] = (b[j]-temp[j])/D[j];
  }
	
  matrix_vector_mult(n, A, x, temp);
 
}



