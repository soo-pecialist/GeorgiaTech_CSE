/*
 ============================================================================
 Name        : HW2.c
 Author      : Soo Hyeon Kim
 Version     : 09/15/17
 Copyright   : © 2017 Soo Hyeon Kim. All rights reserved.
 Student name: Soo Hyeon Kim
 Username	 : skim3026 (#:903230437)
 ============================================================================
 */

//TODO: maximum iteration: 100 & maximum clusters: 30
#include <stdio.h>
#include <stdlib.h>
#include "kmeans_soohyeon.h"

void usage(const char *programName) {
    printf("usage: %s  inputFile k outputFile\n", programName);
    printf("Here,\n");
    printf(" inputFile  : number of rows\n");
    printf("    k       : number of columns\n");
    printf(" outputFile : number of simulation iterations\n");
    printf("\nExample: %s\n'./data/testin.txt' 5 './data/testout.txt'\n", programName);
} // end usage()

//TODO: 1)define length of attr(dim) 2) data pointer X 3) number of element n
	 // 4) number of clusters k  5) initial cluster centroids
int main(int argc, const char * argv[]) {

//TODO: input command line - 1) name of inputfile 2) value of k 3) output file name
	//TODO: uncomment it when submit
	if (argc != 4)
	{
	    usage(argv[0]);
	    return EXIT_FAILURE;
   }
// 1) default setting
	const char* inputFile= argv[1];
	int k = (int) strtol(argv[2], NULL, 10);
	const char* outputFile = argv[3];

	// input parameter check
	if(inputFile <= 0 || k <= 0 || outputFile <= 0)
	{
		printf("INVALID INPUTS!\n");
		return EXIT_FAILURE;
	}

	//TODO: This is only for testing, comment out when not used
	// char* inputFile = "./data/gbm-KM.norm";
	// int k = 5;
	// char* outputFile = "./data/gbm-kM_OUT.norm";


	FILE* ifp; //input file
	FILE* ofp; //output file

	ifp = fopen(inputFile, "r");
	ofp = fopen(outputFile, "w");

	// file pointer check
	if(ifp == NULL) {
		printf("inputfile io error\n\n");
		usage(argv[0]);
		return EXIT_FAILURE;
	}

	if(ofp == NULL) {
		printf("outputfile io error\n\n");
		usage(argv[0]);
		return EXIT_FAILURE;
	}

//2) reading from input file
	int n; // # of point
	int dim; // # of attr

	if(fscanf(ifp, "%d %d", &n, &dim) != 2) {
		printf("ERROR");
	}
	fprintf(ofp, "%d %d %d\n", n, dim, k);

	printf("Number of data: %d\n", n);
	printf("Number of dimension: %d\n", dim);
	printf("Number of cluster: %d\n\n", k);

	double* X = (double *) malloc(sizeof(double) * n * dim); //data read: ((0~dim-1), ... n times)
	for (int ii = 0; ii < n; ii++)
	{
		for(int jj=0; jj < dim; jj++)
			fscanf(ifp, "%lf", &X[ii*dim + jj]);
	}


//3) assigning initial centroids - ((0~dim-1), ... (k times))
	double* cluster_centroid = (double *) malloc(sizeof(double) * k * dim); //centroid coordinates
	int* cluster_assignment_final = (int *)malloc(sizeof(int) * n);

	if (cluster_centroid == NULL || cluster_assignment_final == NULL)
	{
		printf("Memory shortage. pointers cannot be assigned!");
		return EXIT_FAILURE;
	}

	//first k points are initial centroids
	for (int ii = 0; ii < k; ii++)
	{
		for (int jj = 0; jj<dim; jj++)
		{
			cluster_centroid[ii*dim + jj] = X[ii*dim + jj];
			// printf("%lf ", X[ii*dim + jj]);
			// printf("%lf ", cluster_centroid[ii*dim + jj]);
		}
	}
//4) call in kmeans() in "kmeans_.c"
	// kmeans(int dim, double *X, int n, int k, double *cluster_centroid, int *cluster_assignment_final)
	// results will be recorded in cluster_centroid & cluster_assignment_final
	kmeans(dim, X, n, k, cluster_centroid, cluster_assignment_final);


//5) write onto output file
	for(int ii=0; ii<n; ii++)
	{

		fprintf(ofp, "%d\t", cluster_assignment_final[ii]);
		for(int jj=0; jj<dim; jj++)
		{
			fprintf(ofp, "%lf\t", X[ii*dim + jj]);
		}
		fprintf(ofp, "\n");
	}

//6) free memory

	if (X == NULL || cluster_centroid == NULL || cluster_assignment_final == NULL)
	{
		printf("Error took place during free-ing pointers!\n");
		return EXIT_FAILURE;
	}
	free(X);
	free(cluster_centroid);
	free(cluster_assignment_final);


	return EXIT_SUCCESS;

}// end main()




