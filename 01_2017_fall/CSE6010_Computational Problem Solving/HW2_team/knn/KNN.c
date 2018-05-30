//  CSE-6010 Assignment2
//
//  Created by  Xiangyu Kong on 9/17/17.
//  Copyright © 2017 Xiangyu Kong. All rights reserved.
//
//  Student name: Xiangyu Kong
//  Student username: xkong36


// Implementation Details:
// Main function:
// 1)Read the input data file :1. Train dataset input(the results of the K means) 2.The data to be classified(gbm_KNN.norm)
// 2)Calculate the distance between every target point and train points
// 3)Sort the distance result and then choose the nearest K points
// 4)Get the cluster of the majority K points, choosing the smallest number of the cluster if threre is a tie.
// 5)Assign the data need to be classified to the cluster

#include<stdio.h>
#include<stdlib.h>
#include<math.h>


#define INF_DIST (INFINITY)
#define WARNING "There is a problem when running your code, check the pointers \n"

double calc_one_distance(int attr1 , int num_target_number, int num_train_number, double*target_points , double*train_points);
double* calc_all_distances(int attr1, int n1, int n2, double *train_points, double *target_points, double* distance_output);
void insertion_sort(double arr[], int start, int end);
void get_kmin_distance(int n1, int n2, int k, double* dis_sum_sort, double* kmin_of_distance);
int* group_kmin(int n1, int n2, int k, double* kmin_of_distance, double* dis_sum);
int find_one_cluster(int num_of_point,int cluster_num, int k, int* k_neighbors_cluster);
void diagnosis(int attr1, int n, int k, double *X, int *cluster_assignment_index, double *cluster_centroid);
void get_cluster_member_count(int n, int k, int *cluster_assignment_index, int *cluster_member_count);
void check_null(double *pointer);


void usage(const char *programName) {
    printf("usage: %s   trainfile targetfile outputFile  the_value_of_k\n", programName);
    printf("Here,\n");
    printf(" kmeans_output_file  : \n");
    printf(" target_data_file : \n");
    printf("    outputFile       : \n");
    printf(" the_value_of_k     : \n");
    printf("\n Example: %s\n' testout.txt  gbm-KNN.norm  knnoutput.txt  6'\n", programName);
}

int main(int argc, const char * argv[]){

	//Read the input data
	const char* Train_file = argv[1];
	const char* Target_file = argv[2];
	const char* outputFile = argv[3];
    int k = (int) strtol(argv[4], NULL, 10);

	if (argc != 5){
	    usage(argv[0]);
	    return EXIT_FAILURE;
   	}

   	//create FILE pointer to read file
	FILE* target_fp;
	FILE* train_fp;
	FILE* ofp;

	target_fp = fopen(Target_file, "r");
	train_fp  = fopen(Train_file, "r");
	ofp = fopen(outputFile, "w");

	//check if the inputfile is null.
	if(target_fp == NULL || train_fp == NULL){
		printf("inputFile io error");
		return EXIT_FAILURE;
	}

	//check if the outputfile is null
	if(ofp == NULL){
	    printf("outputfile io error\n\n");
	    return EXIT_FAILURE;
	}

	// Read Data from input files
	int n1, n2, cluster_num;  //n1 : number of target points,  n2: number of train points
	int attr1, attr2; // a: number of attributes
	fscanf(target_fp, "%d %d", &n1, &attr1);
	fscanf(train_fp, "%d %d %d", &n2, &attr2, &cluster_num);

	//printf the read file result to check if the read data is correct
	printf("Number of target data points: %d\n", n1);
	printf("Number of target data attributes: %d\n", attr1);
	printf("Number of train data points: %d\n", n2);
	printf("Number of train data attributes: %d\n", attr2);
	printf("Number of cluster: %d\n", cluster_num);

	//define the array to store the train data and target data
	double* target_points = (double *)malloc(sizeof(double) * n1 * attr1);
	double* train_points = (double *)malloc(sizeof(double) * n2 * attr2);

	//define the array to store the label value of the train points
	int* clusters_of_point = (int *)malloc(sizeof(int) * n2);

	// read the data to arrays
	for(int i = 0; i < n1; i++){
		for(int j = 0; j < attr1; j++){
			fscanf(target_fp, "%lf", &target_points[i*attr1 + j]);
		}
	}


	for(int i = 0; i < n2; i++){
	    fscanf(train_fp, "%d", &clusters_of_point[i]);
		for(int j = 0; j < attr2; j++){
		    fscanf(train_fp, "%lf", &train_points[i*attr2 + j]);
		}
	}

	//define the array to store all the points distance
	double* dis_sum = (double *)malloc(sizeof(double) * n1 * n2);
	dis_sum = calc_all_distances(attr1, n1, n2, target_points, train_points, dis_sum);
	
	//copy the dis_sum array to sort
	double* dis_sum_sort = (double *)malloc(sizeof(double) * n1 * n2);
	for(int i = 0; i < n1 * n2; i++){
		dis_sum_sort[i] = dis_sum[i];
	}

	//sort the distance result
	for(int i = 0; i < n1; i++){
		insertion_sort(dis_sum_sort, i * n2, (i+1) * n2 );
	}

	//define the array to store the k smallest neighbot distance in each target points
	double* kmin_of_distance = (double *)malloc(sizeof(double) * n1 * k);
	int* kneighbor_index = (int *)malloc(sizeof(int) * n1 * k);


	get_kmin_distance(n1, n2, k, dis_sum_sort, kmin_of_distance);
	kneighbor_index = group_kmin(n1, n2, k, kmin_of_distance, dis_sum);
 //printf k neighbor index used for test
	printf("\n------Just For Testing------\nthe k neighbor index result is:\n");
	for(int i = 0; i < n1 * k; i++){
			printf("%d\t", kneighbor_index[i]);
		}
		printf("\n");

	//get the cluster results of the k nearest neighbors
	int* k_neighbors_cluster = (int *)malloc(sizeof(int) * n1 * k);



	for(int i = 0; i < n1 * k; i++){
		k_neighbors_cluster[i] = clusters_of_point[kneighbor_index[i]];
	}

	//define an array to record the classification results
	int* cluster_result = (int *)malloc(sizeof(int) * n1);

	printf("-----the cluster result: -----\n");
	for(int i = 0; i < n1; i++){
		cluster_result[i] = find_one_cluster(i, cluster_num, k, k_neighbors_cluster);
		printf(" %d \n", cluster_result[i]);
	}

	//write to output file
	fprintf(ofp, "%d\t%d\t%d\t%d\n", n1, attr1, cluster_num, k);
	for(int i=0; i<n1; i++)
	{

		fprintf(ofp, "%d\t", cluster_result[i]);
		fprintf(ofp, "\n");
	}

	//free all the arrays
	free(target_points);
	free(train_points);
	free(dis_sum);
	free(kmin_of_distance);
	free(kneighbor_index);
	free(k_neighbors_cluster);
	free(cluster_result);


}


//calculate the distance between one target point and one train point
double calc_one_distance(int attr1 , int num_target_number, int num_train_number, double*target_points , double*train_points){
  	//check if the pointer is null
  	check_null(target_points);
  	check_null(train_points);

    double distance_sq_sum = 0;

    for (int i = 0; i < attr1; i++){
      distance_sq_sum += (target_points[num_target_number * attr1 + i] - train_points[num_train_number * attr1 + i]) * (target_points[num_target_number * attr1 + i] - train_points[num_train_number * attr1 + i]);
    }
    return distance_sq_sum;

  }

double* calc_all_distances(int attr1, int n1, int n2, double *target_points, double *train_points, double* distance_output){
	check_null(train_points);
	check_null(target_points);
	check_null(distance_output);

    for (int i = 0; i < n1; i++){ // for each point
      for (int j = 0; j < n2; j++){

        // calculate distance between each train point and each target point
        distance_output[i * n2 + j] = calc_one_distance(attr1, i, j, target_points, train_points);
        }
  	}
  	return distance_output;
 }

//sort the arrray from the index of start to the index of end
void insertion_sort(double arr[], int start, int end){
   int i, j;
   double key;
   for (i = start; i < end; i++)
   {
       key = arr[i];
       j = i-1;
       while (j >= start && arr[j] > key)
       {
           arr[j+1] = arr[j];
           j = j-1;
       }
       arr[j+1] = key;
   }
}


//define a function to find the k smallest distance
void get_kmin_distance(int n1, int n2, int k, double* dis_sum_sort, double* kmin_of_distance){
	check_null(dis_sum_sort);
	check_null(kmin_of_distance);

	for(int i = 0; i < n1; i++){
		for(int j = 0; j < k; j++){
			kmin_of_distance[i * k + j] = dis_sum_sort[i * n2 + j];
		}
	}
}


//define a function to find the index of all the k near neighbors
int* group_kmin(int n1, int n2, int k, double* kmin_of_distance, double* dis_sum){

	check_null(kmin_of_distance);
	check_null(dis_sum);
	int* kneighbor_index = (int* ) malloc(sizeof(int) * n1 * k);
	for(int m = 0; m < n1; m++){ // the first point k neibors
		for(int i = 0; i < k; i++){ // for loop from the point's nearest neighbor to its kth nearest neighbot
			for(int index = m * n2; index < (m + 1) * n2; index++){ //loop start from the first attributes of the first train points to find its order
				if(kmin_of_distance[m * k + i] == dis_sum[index]){
					kneighbor_index[m * k + i] = index % n2;
				}
			}
		}
	}
	return kneighbor_index;
}

int find_one_cluster(int num_of_point, int cluster_num, int k, int* k_neighbors_cluster){

	if(k_neighbors_cluster == NULL){
		printf(WARNING);
		return EXIT_FAILURE;
	}

	int count[cluster_num];
	for(int i = 0; i < cluster_num; i++){
		count[i] = 0;
	}

	//calculate the times of the every cluster
	for(int m = 0; m < k; m++){
		int result = k_neighbors_cluster[num_of_point * k + m]; //find the cluster of the mth neighbor
		count[result] = count[result] + 1;
		
	}
	//find the majority of the cluster in the k neighbots
	int max = -1;
	int max_index = 0;
	for(int i = 0; i < cluster_num; i++){
		if(count[i] > max){
			max = count[i];
			max_index = i;
		}
	}
	return max_index;
}

//define a function to check the null double pointers
void check_null(double *pointer){
	if (pointer == NULL){
		printf(WARNING);
		exit(EXIT_FAILURE);
	}
}


