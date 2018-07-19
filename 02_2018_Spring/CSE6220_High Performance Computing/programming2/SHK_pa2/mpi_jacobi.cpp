/**
 * @file    mpi_jacobi.cpp
 * @author  Patrick Flick <patrick.flick@gmail.com>
 * @brief   Implements MPI functions for distributing vectors and matrixes,
 *          parallel distributed matrix-vector multiplication and Jacobi's
 *          method.
 *
 * Copyright (c) 2014 Georgia Institute of Technology. All Rights Reserved.
 */

#include "mpi_jacobi.h"
#include "jacobi.h"
#include "utils.h"

#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <vector>
#include <iostream>
#include <cstring>

/*
 * TODO: Implement your solutions here
 */
void initialize_vector(double* vect, int size);

//multiply two vectors
void mult_vectors(double* v1, double* v2, double* result, int rows, int cols);

void communicate_R_and_D(double* A, double* R, double* D,
						   int i, int j, int rows, int cols, MPI_Comm comm);
void update_x(double* X, double* P, double* b, double* D,int n);
int check_terminiation(double* X,double* b, double term, int n, MPI_Comm comm);
void communicate_rows_cols(int i, int j, int row_size, int col_size, double* rVector, double* cVector, MPI_Comm comm);

void decompose(int p, int n, int *count, int *offset);
void decompose_matrix(int n, int q, double* input_matrix, 
					    double* destMatrix,int* count,int* offset);

//given the processor coordinates, return the assigned sub matrix size (row, col)
void get_sub_matrix_dim(int i, int j,int n, int q, int* row, int* col);
void get_processor_index(MPI_Comm comm, int* i, int* j);
	
void distribute_vector(const int n, double* input_vector, double** local_vector, MPI_Comm comm)
{
int rank=-1;
int local_n;
int i, j;
int q, size;
	
  // Get the size and rank
  MPI_Comm_size(comm,&size);
  MPI_Comm_rank(comm,&rank);

  //get the processor indices
  get_processor_index(comm, &i, &j);
 
  // get the q
  q = sqrt(size);
  int participate= (j==0)? 1:0;
	
 //distribute the values
  MPI_Comm row_master;
  MPI_Comm_split(comm, participate, i, &row_master);
  
  local_n=block_decompose(n,q,i);
  if(! participate){
	  MPI_Comm_free(&row_master);
	  return;
  }
  //allocate memory for the counts and offsets
  int* count=(int*)malloc(sizeof(int)*q);
  int* offset=(int*)malloc(sizeof(int)*q);

   //for debuging
  //printf("this is processor (%d,%d), size=%d, local_n=%d\n",i,j,q,local_n);
	
  if(rank==0){
    //determine the number of elements for each process (see project 1)
    decompose(q,n,count,offset); 
  }
	
  *local_vector = new double[local_n];
  // fprintf (stderr,"process: %d, size=%d \n",rank,local_n);

  MPI_Scatterv(input_vector, &count[0], &offset[0], MPI_DOUBLE, *local_vector, local_n, MPI_DOUBLE, 0, row_master);
  //free memory
  free(count);
  free(offset);
  MPI_Comm_free(&row_master);
}

/// gather the local vector distributed among (i,0) to the processor (0,0)
void gather_vector(const int n, double* local_vector, double* output_vector, MPI_Comm comm)
{
 int local_n;
 int i, j;
 int q, size, rank;
	
  MPI_Comm_size(comm,&size);
  MPI_Comm_rank(comm,&rank);

  get_processor_index(comm, &i, &j);
	
  //check if this processor will take part in the vector gethering
  int participate= (j==0)? 1:0;
	
 
  MPI_Comm row_master;
  MPI_Comm_split(comm, participate, i, &row_master);
	
  if(participate==0) {
	  MPI_Comm_free(&row_master);
	 return;
  }
	
  //get the number of elements for this processor	
  local_n=block_decompose_by_dim(n, comm, 0);
	
  //local grid size
  q = sqrt(size);
  //allocate memory for the counts and offsets
  int* count=(int*)malloc(sizeof(int)*q);
  int* offset=(int*)malloc(sizeof(int)*q);
	
  if(rank==0){
    //determine the number of elements for each process (see project 1)
    decompose(q,n,count,offset); 
  }
  MPI_Gatherv (local_vector, local_n,MPI_DOUBLE,output_vector, count, offset, MPI_DOUBLE,0,row_master);

  //free memory
  free(count);
  free(offset);
	
  MPI_Comm_free(&row_master);
}

void distribute_matrix(const int n, double* input_matrix, double** local_matrix, MPI_Comm comm)
{
  int i, j;
  int q, size, rank;
  int row_size, col_size;
  int  mat_local_size;
	
  MPI_Comm_size(comm,&size);
  MPI_Comm_rank(comm,&rank);

  get_processor_index(comm, &i, &j);

  //local grid size
  q = sqrt(size);

  // define the matrix communicator
  MPI_Comm local_comm;
  MPI_Comm_split(comm, 0, i*q+j, &local_comm);

  //allocate memory for the counts and offsets
  int* count=(int*)malloc(sizeof(int)*size);
  int* offset=(int*)malloc(sizeof(int)*size);
  double* tempMatrix = NULL;
	
 //get the row and column size of for this processor
  get_sub_matrix_dim(i, j,n, q, &row_size, &col_size);
	
   //for debug
  //printf("I'm process with index (%d,%d), with sub matrix=(%d,%d)\n",i,j,row_size,col_size);
  if(rank==0){
	  tempMatrix = (double*)malloc(sizeof(double)*n*n);
	  decompose_matrix(n,q,input_matrix,tempMatrix,count,offset);
  }

  //allocate the local matrix 
  mat_local_size=col_size*row_size;
 *local_matrix = new double[mat_local_size];

 //fprintf(stderr, "P%d: distributing matrix, size=%d\n",rank, mat_local_size); 	
 MPI_Scatterv(tempMatrix, count, offset, MPI_DOUBLE, *local_matrix, mat_local_size, MPI_DOUBLE, 0, local_comm);
 MPI_Comm_free(&local_comm);
	
 //free memory
 free(count);
 free(offset);
 if(rank==0) free(tempMatrix);
}

void transpose_bcast_vector(const int n, double* col_vector, double* row_vector, MPI_Comm comm)
{
  int i, j;
  int q, size, rank;

  int row_size, col_size;
	
  MPI_Comm_size(comm,&size);
  MPI_Comm_rank(comm,&rank);

  get_processor_index(comm, &i, &j);
		
  //local grid size
  q = sqrt(size);  

  //get the row and column size of for this processor
  get_sub_matrix_dim(i, j,n, q, &row_size, &col_size);

  communicate_rows_cols(i,j,row_size,col_size, row_vector,col_vector, comm);
	
  MPI_Comm col_master_com;
  MPI_Comm_split(comm, j, i, &col_master_com);

  MPI_Bcast(row_vector, col_size, MPI_DOUBLE, j, col_master_com);
  MPI_Comm_free(&col_master_com);
}

void distributed_matrix_vector_mult(const int n, double* local_A, double* local_x, double* local_y, MPI_Comm comm)
{
  int i, j;
  int q;
  int size,rank;
  int row_size, col_size;
	
  MPI_Comm_size(comm,&size);
  MPI_Comm_rank(comm,&rank);

  get_processor_index(comm, &i, &j);
  q = sqrt(size);
	
  get_sub_matrix_dim(i, j,n, q, &row_size, &col_size);
	
  double *row_values=(double*)malloc(sizeof(double)*col_size);
  transpose_bcast_vector(n, local_x, row_values, comm);

  double  *temp=(double*)malloc(sizeof(double)*row_size);  
  mult_vectors(local_A,row_values,temp,row_size,col_size);

  //Row communication
  MPI_Comm row_comm;
  MPI_Comm_split(comm, i, j, &row_comm);

  //Using reduce to merging the result
  MPI_Reduce(&temp[0], local_y, row_size, MPI_DOUBLE, MPI_SUM, 0, row_comm);

  // free allocated memory
  MPI_Comm_free(&row_comm);
  free(temp);
  free(row_values);
    
}

/// Solves Ax = b using the iterative jacobi method
void distributed_jacobi(const int n, double* local_A, double* local_b, double* local_x,
                MPI_Comm comm, int max_iter, double l2_termination)
{
    
 int i, j;
 int q;
 int size,rank;
 int row_size, col_size;
 
 double* local_D;
 double* local_R;
 int is_finished = 0;

	
  MPI_Comm_size(comm,&size);
  MPI_Comm_rank(comm,&rank);

  get_processor_index(comm, &i, &j);
  q = sqrt(size);

  get_sub_matrix_dim(i, j,n, q, &row_size, &col_size);
	
  local_D=(double*)malloc(sizeof(double)*col_size);
  local_R=(double*)malloc(sizeof(double)*col_size*row_size);
	
  memcpy(local_R, local_A, col_size*row_size*sizeof(double));

  //communicate the local part of R and D
  communicate_R_and_D(local_A,local_R,local_D,i,j,row_size,col_size,comm);

   //initialize  
   if(j == 0)  initialize_vector(local_x,row_size);
      
   int participate= (j==0)? 1:0;
	
   MPI_Comm row_master;
   MPI_Comm_split(comm, participate, i, &row_master);

   MPI_Comm  col_master;
   MPI_Comm_split(comm, i, j, &col_master);
	
   double* local_P=(double*)malloc(sizeof(double)*row_size);
   double* local_W=(double*)malloc(sizeof(double)*row_size);
	
   
    //Iteration
    for(int iter = 0; iter < max_iter; iter++){
			 
       distributed_matrix_vector_mult(n, local_R, local_x, local_P, comm);
	   if (j == 0)  update_x(local_x,local_P, local_b, local_D,row_size);
       distributed_matrix_vector_mult(n, local_A, local_x, &local_W[0], comm);
	
	   if (j==0)
		 is_finished=check_terminiation(local_W,local_b,l2_termination,row_size,row_master);
		
      MPI_Bcast(&is_finished, 1, MPI_INT, 0, col_master);

     if (is_finished == 1) break;
    }
	
    MPI_Comm_free(&row_master);
    MPI_Comm_free(&col_master);
	
	free(local_D);
	free(local_R);
	free(local_P);
	free(local_W);
}

// wraps the distributed matrix vector multiplication
void mpi_matrix_vector_mult(const int n, double* A,
                            double* x, double* y, MPI_Comm comm)
{
    // distribute the array onto local processors!
    double* local_A = NULL;
    double* local_x = NULL;
    distribute_matrix(n, &A[0], &local_A, comm);
    distribute_vector(n, &x[0], &local_x, comm);

    // allocate local result space
    double* local_y = new double[block_decompose_by_dim(n, comm, 0)];
    distributed_matrix_vector_mult(n, local_A, local_x, local_y, comm);

    // gather results back to rank 0
    gather_vector(n, local_y, y, comm);
}

// wraps the distributed jacobi function
void mpi_jacobi(const int n, double* A, double* b, double* x, MPI_Comm comm,
                int max_iter, double l2_termination)
{
    // distribute the array onto local processors!
    double* local_A = NULL;
    double* local_b = NULL;
    distribute_matrix(n, &A[0], &local_A, comm);
    distribute_vector(n, &b[0], &local_b, comm);

   // int rank;
   // MPI_Comm_rank(comm,&rank);
    //fprintf (stderr,"process: %d \n",rank);
    	

    // allocate local result space
    double* local_x = new double[block_decompose_by_dim(n, comm, 0)];
    distributed_jacobi(n, local_A, local_b, local_x, comm, max_iter, l2_termination);

    // gather results back to rank 0
    gather_vector(n, local_x, x, comm);
}

void decompose(int p, int n, int *count, int *offset)
{
int rank;
	
  for(rank = 0; rank < p; rank++)
     count[rank]=block_decompose(n,p,rank);

   offset[0] = 0;
   for(rank = 1; rank < p; rank++)
      offset[rank] = offset[rank-1] + count[rank-1];  
}

void decompose_matrix(int n, int q, double* input_matrix, double* destMatrix,int* count,int* offset)
{
  int local_rows, local_cols;
  int r, c,m;
  int rCount=0, cCount=0;
  int mId=0;
	
   offset[0]=0;
	for(r =0; r<q; r++){
      cCount = 0;
	  local_rows=block_decompose(n,q,r);
	  for(c=0;c<q;c++){
		 local_cols=block_decompose(n,q,c);
		              
        for (m = 0; m<local_rows; m++){
          memcpy(&destMatrix[mId], &input_matrix[(rCount+m)*n+cCount], local_cols*sizeof(double));
          mId += local_cols;
        }
		  
        cCount += local_cols;
        count[r*q+c] = local_rows*local_cols;
		
		if(r*q+c>0) 
		  offset[r*q+c] = offset[r*q+c-1]+count[r*q+c-1];
      }
		
      rCount += local_rows;
    }
}


//given the processor coordinates, return the assigned sub matrix size (row, col)
void get_sub_matrix_dim(int i, int j,int n, int q, int* row, int* col)
{
  *row=block_decompose(n,q,i);
  *col=block_decompose(n,q,j);
}

// returns the prcessor indeces (i, j)
void get_processor_index(MPI_Comm comm, int* i, int* j)
{
int rank=-1;
int coord[2] = {-1,-1};
  MPI_Comm_rank(comm,&rank);
  MPI_Cart_coords(comm, rank, 2, coord);
  *i=coord[0];
  *j=coord[1];
}

void initialize_vector(double* vect, int size)
{
  int k;
	for(k=0;k<size; k++) 
		vect[k]=0;
}

void mult_vectors(double* v1, double* v2, double* result, int rows, int cols)
{
 for(int c = 0; c<rows; c++){
	 result[c]=0;
    for(int r = 0; r<cols; r++)
       result[c] += v1[c*cols+r]*v2[r];
    }
}

void update_x(double* X, double* P, double* b,double* D, int n)
{
int k; 
	for(k = 0; k < n; k++){
		X[k] = (b[k]- P[k])/D[k];
	}	
}

int check_terminiation(double* W,double* b, double term, int n, MPI_Comm comm)
{
int k;
double l2_square=0;
double l2_norm;
	
	for(k = 0; k < n; k++){
		l2_square += (b[k] - W[k])*(b[k] - W[k]);
	}    

	MPI_Allreduce(&l2_square, &l2_norm, 1, MPI_DOUBLE, MPI_SUM, comm);
	l2_norm = sqrt(l2_norm);

	return (l2_norm <= term);
}

void communicate_R_and_D(double* A, double* R, double* D,
						  int i, int j, int rows, int cols,MPI_Comm comm)
{
int k;
	
  if(i == 0 && j ==0){
   for(k = 0; k < rows; k++){
	 D[k] = A[k*cols + k];
	 R[k*cols + k] = 0;
  }
  }else if (i == j){
	for(k = 0; k < rows; k++){
		D[k] = A[k*cols + k];
		R[k*cols + k] = 0;
	}
	int dcoordinate[2] = {i,0};
	int drank = 0;
	MPI_Cart_rank(comm, dcoordinate, &drank);
	MPI_Send(D, rows, MPI_DOUBLE, drank, 1, comm);
 }else if (j == 0){
	int scoordinate[2] = {i,i};
	int srank = 0;
	MPI_Cart_rank(comm, scoordinate, &srank);
	MPI_Status status;
	MPI_Recv(D, rows, MPI_DOUBLE, srank, 1, comm, &status);
 }	
}

void communicate_rows_cols(int i, int j, int row_size, int col_size, double* rVector, double* cVector, MPI_Comm comm)
{
 if(j==0 && i==0){
	memcpy(rVector, cVector, row_size*sizeof(double));
  }else if(j==0 && i!=0) {
	int recvIndeces[] = {i, i};
    int diag = 0;
    MPI_Cart_rank(comm, recvIndeces, &diag);
    MPI_Send(cVector, row_size, MPI_DOUBLE, diag,0,comm);
  } else if(i==j){ 
    int diag = 0;
    int senderIndeces[] = {i, 0};
    MPI_Cart_rank(comm, senderIndeces, &diag);
    MPI_Status status;
    MPI_Recv(rVector, col_size, MPI_DOUBLE, diag, 0, comm, &status);
  }
}

