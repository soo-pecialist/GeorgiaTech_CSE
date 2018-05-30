/*
 * kmeans_soohyeon.h
 *
 * created on: Sep 16, 2017
 *	   Author: Soo Hyeon Kim
 */

/* 
<Briefing>
// double *dist                    = (double *)malloc(sizeof(double) * n * k);
// int   *cluster_assignment_cur  = (int *)malloc(sizeof(int) * n);
// int   *cluster_assignment_prev = (int *)malloc(sizeof(int) * n);
// ** Notice all arrays are 1d-array

Kmeans(int dim, double *X, int n, int k, double *cluster_centroid, int *cluster_assignment_final)
	//X(data): ((0~dim-1), ... n times)
	//cluster_centroid(centroid coordinates): ((0~dim-1), ... (k times))
	//cluster_assignment_final(final clustered result) : e.g., (0, 2, 3, 1, ... (n times))
	>fail(char *str)
		//ref: if something is wrong, then terminates the program
	>calc_all_distances(dim, n, k, X, cluster_centroid, dist);
		//ref: calculate distance between each point and each centroid
		//dist: ((0~k-1), ... (n times))
		>>calc_distance(int dim, double *p1, double *p2)
			//ref: distance between two points
	>choose_all_clusters_from_distances(dim, n, k, dist, cluster_assignment_cur);
		//ref: find the closest cluster from each point and store it in cluster_assignment_cur[]
		//cluster_assignment_cur: (0, 1, 2, ... (n points))
	>copy_assignment_array(n, cluster_assignment_cur, cluster_assignment_prev);
		//ref: simply copying array
	>calc_cluster_centroids(dim, n, k, X, cluster_assignment_cur, cluster_centroid);
		//ref: updating centroids' coordinates happens here
	>calc_total_distance(dim, n, k, X, cluster_centroid, cluster_assignment_cur);
		//ref: return total distance (sum of distances between point and its centroid) 
		>>calc_distance(int dim, double *p1, double *p2)
	>copy_assignment_array(int n, int *source, int *target);
		//ref: copy source into target (restoration)
	>assignment_change_count(n, cluster_assignment_cur, cluster_assignment_prev);
		//ref: return how many changes have happened in this step
	>diagnosis(dim, n, k, X, cluster_assignment_cur, cluster_centroid);
		>>get_cluster_member_count()
			//ref: update cluster_member_count[] - of which ith member holds the number of members in ith cluster
*/


#ifndef KMEANS_SOOHYEON_
#define KMEANS_SOOHYEON_

void fail(char *str);

double calc_distance(int dim, double *p1, double *p2);

void calc_all_distances(int dim, int n, int k, double *X, double *centroid, double *distance_output);

double calc_total_distance(int dim, int n, int k, double *X, double *centroids, int *cluster_assignment_index);

void choose_all_clusters_from_distances(int dim, int n, int k, double *distance_array, int *cluster_assignment_index);

void calc_cluster_centroids(int dim, int n, int k, double *X, int *cluster_assignment_index, double *new_cluster_centroid);

void get_cluster_member_count(int n, int k, int *cluster_assignment_index, int *cluster_member_count);

void diagnosis(int dim, int n, int k, double *X, int *cluster_assignment_index, double *cluster_centroid);

void copy_assignment_array(int n, int *src, int *tgt);

int assignment_change_count(int n, int a[], int b[]);

void kmeans(int dim, double *X, int n, int k, double *cluster_centroid, int *cluster_assignment_final);


#endif /* KMEANS_SOOHYEON_ */
