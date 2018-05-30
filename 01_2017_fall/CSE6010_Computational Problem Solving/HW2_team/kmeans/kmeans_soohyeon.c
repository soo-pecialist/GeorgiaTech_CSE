//
//  kmeans_soohyeon.c
//  CSE-6010 Assignment2
//
//  Created by Soo Hyeon Kim on 9/16/17.
//  Copyright © 2017 Soo Hyeon Kim. All rights reserved.
//
//  Student name: Soo Hyeon Kim
//  Student username: skim3026 (#:903230437)

//TODO: input command line - 1) name of inputfile 2) value of k 3) output file name
//TODO: 1)define length of attr 2) data pointer X 3) number of element n
	 // 4) number of clusters k  5) initial cluster centroids
// kmeans.c


#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#define sqr(x) ((x)*(x))
#define MAX_K 30              // maximum cluster number allowed
#define MAX_ITER 100          // maximum iteration number allowed
#define INF_DIST (INFINITY)

void fail(char *str)
  {
    if(!str)
      printf("fail() fails!");

    printf("%s", str);
    exit(-1);
  }

double calc_distance(int dim, double *p1, double *p2)
  {
    if(!p1 || !p2)
      fail("Error in calc_distance(). Pointers are not assigned!");    
    if(dim <= 0)
    		fail("Wrong input in calc_distance().");

    double distance_sq_sum = 0;

    for (int ii = 0; ii < dim; ii++)
      distance_sq_sum += sqr(p1[ii] - p2[ii]);

    return distance_sq_sum;

  }

void calc_all_distances(int dim, int n, int k, double *X, double *centroid, double *distance_output)
  {
    if(!X || !centroid || !distance_output)
      fail("Error in calc_all_distances(). Pointers are not assigned!");
    if(dim <= 0 || n <= 0 || k <= 0)
    		fail("Wrong input in calc_all_distances().");

    for (int ii = 0; ii < n; ii++) // for each point
      for (int jj = 0; jj < k; jj++) // for each cluster
        {
         // calculate distance between point and cluster centroid
          distance_output[ii*k + jj] = calc_distance(dim, &X[ii*dim], &centroid[jj*dim]);
        }
  }

double calc_total_distance(int dim, int n, int k, double *X, double *centroids, int *cluster_assignment_index)
 // NOTE: a point with cluster assignment -1 is ignored
  {
    if(!X || !centroids || !cluster_assignment_index)
      fail("Error in calc_total_distance(). Pointers are not assigned!");

    if (dim <= 0 || n <= 0 || k <= 0)
    		fail("Wrong input in cacl_total_distance().");

    double tot_D = 0;

   // for every point
    for (int ii = 0; ii < n; ii++)
      {
       // which cluster is it in?
        int active_cluster = cluster_assignment_index[ii];

       // sum distance
        if (active_cluster != -1)
          tot_D += calc_distance(dim, &X[ii*dim], &centroids[active_cluster*dim]);
      }

    return tot_D;
  }

void choose_all_clusters_from_distances(int dim, int n, int k, double *distance_array, int *cluster_assignment_index)
  {
    if(!distance_array || !cluster_assignment_index)
      fail("Error in choose_all_clusters_from_distances(). Pointers are not assigned!");
    if(dim<=0 || n <=0 || k<= 0)
    		fail("Wrong input in choose_all_clusters_from_distances().");

   // for each point
    for (int ii = 0; ii < n; ii++)
      {
        int best_index = -1;
        double closest_distance = INF_DIST;

       // for each cluster
        for (int jj = 0; jj < k; jj++)
          {
           // distance between point and cluster centroid
           // looking for minimum distance from point(ii) to cluster(jj)
            double cur_distance = distance_array[ii*k + jj];
            if (cur_distance < closest_distance)
              {
                best_index = jj;
                closest_distance = cur_distance;
              }
          }

       // record in array
        cluster_assignment_index[ii] = best_index;
      }
  }

void calc_cluster_centroids(int dim, int n, int k, double *X, int *cluster_assignment_index, double *new_cluster_centroid)
  {
    int cluster_member_count[MAX_K];

    if(!X || !cluster_assignment_index || !new_cluster_centroid)
      fail("Error in calc_cluster_centroids(). Pointers are not assigned!");
    if(dim <= 0 || n <= 0 || k <= 0)
    		fail("Wrong input in calc_cluster_centroids().");

   // initialize cluster centroid coordinate sums to zero
    for (int ii = 0; ii < k; ii++)
      {
        cluster_member_count[ii] = 0; // this will go deominator

        for (int jj = 0; jj < dim; jj++)
          new_cluster_centroid[ii*dim + jj] = 0;
     }

   // sum all points
   // for every point
    for (int ii = 0; ii < n; ii++)
      {
       // which cluster is it in NOW?
        int active_cluster = cluster_assignment_index[ii];

       // update count of members in that cluster
        cluster_member_count[active_cluster]++;

       // sum point coordinates for finding centroid
        for (int jj = 0; jj < dim; jj++)
          new_cluster_centroid[active_cluster*dim + jj] += X[ii*dim + jj];
      }

   // now divide each coordinate sum by number of members to find mean(centroid)
   // for each cluster
    for (int ii = 0; ii < k; ii++)
      {
        if (cluster_member_count[ii] == 0) // no member
          printf("WARNING: Empty cluster %d! \n", ii);

       // for each dimension
        for (int jj = 0; jj < dim; jj++)
          new_cluster_centroid[ii*dim + jj] /= cluster_member_count[ii];  /// XXXX will divide by 0 here for any empty clusters!

      }
  }

void get_cluster_member_count(int n, int k, int *cluster_assignment_index, int *cluster_member_count)
  {

    if(!cluster_assignment_index || !cluster_member_count)
      fail("Error in get_cluster_member_count(). Pointers are not assigned");
    if(n <= 0 || k <= 0)
    		fail("Wrong input in get_cluster_member_count().");

   // initialize cluster member counts
    for (int ii = 0; ii < k; ii++)
      cluster_member_count[ii] = 0;

   // count members of each cluster
    for (int ii = 0; ii < n; ii++)
      cluster_member_count[cluster_assignment_index[ii]]++;
  }

void diagnosis(int dim, int n, int k, double *X, int *cluster_assignment_index, double *cluster_centroid)
  {
    int cluster_member_count[MAX_K];

    if(!X || !cluster_assignment_index || !cluster_centroid)
      fail("Error in diagnosis(). Pointers are not assigned!");
    if(dim <= 0 || n <= 0 || k<=0)
    		fail("Wrong input in diagnosis().");

    get_cluster_member_count(n, k, cluster_assignment_index, cluster_member_count);

    printf("= = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =\n");
    printf("  <Final clusters> \n\n");
    for (int ii = 0; ii < k; ii++)
    {
    	printf(" (cluster %d) members: %4d,  centroid(att1, att2): (%.2f, %.2f) \n", \
      		ii, cluster_member_count[ii], cluster_centroid[ii*dim + 0], cluster_centroid[ii*dim + 1]);
  		printf(" {");

  		for(int jj = 0; jj<n; jj++)
  			{
  				if(cluster_assignment_index[jj] == ii)
  					printf("X%d ", jj);
  			}

  		printf("}\n\n");
  	}
  }

void copy_assignment_array(int n, int *src, int *tgt)
  { 
    if(!src || !tgt)
      fail("Error in copy_assignment_array(). Pointers are not assigned!");
    if(n <=0)
    		fail("Wrong input in copy_assignment_array().");

    for (int ii = 0; ii < n; ii++)
      tgt[ii] = src[ii];
  }

int assignment_change_count(int n, int a[], int b[])
  {
    if(!a || !b)
      fail("Error in assignment_change_count().");
    if(n <=0)
        	fail("Wrong input in assignment_change_count().");

    int change_count = 0;

    for (int ii = 0; ii < n; ii++)
      if (a[ii] != b[ii])
        change_count++;

    return change_count;
  }

void kmeans(
            int  dim,		                 // dimension of data (attributes number)
            double *X,                       // pointer to data
            int   n,                         // number of elements
            int   k,                         // number of clusters
            double *cluster_centroid,        // initial cluster centroids // ((0~dim-1), ... (n times))
            int   *cluster_assignment_final  // output
           )     
  {

    if(X == NULL || cluster_centroid == NULL || cluster_assignment_final == NULL)
      fail("Error! Pointers are not assigned when kmeans() is called!");
    if(dim <= 0 || n <= 0 || k<=0)
    		fail("Wrong input in kmeans().");


    // expected cluster_assignment_final (e.g., k = 3 & n = 10)
    // (0, 2, 0, 0 , 1, 1, 1, 0, 2, 1)
    double *dist                    = (double *)malloc(sizeof(double) * n * k); // ((0~k-1), ... (n times) )
    int   *cluster_assignment_cur  = (int *)malloc(sizeof(int) * n); // having current cluster-assigning array
    int   *cluster_assignment_prev = (int *)malloc(sizeof(int) * n); // e.g., (0, 2, 0, 0, 1, 1, 1, 0, 2, 1)


    if (!dist || !cluster_assignment_cur || !cluster_assignment_prev)
      fail("Error while allocating arrays");

   // initial setup
    calc_all_distances(dim, n, k, X, cluster_centroid, dist); //ref: calculate distance between each point and each centroid
    choose_all_clusters_from_distances(dim, n, k, dist, cluster_assignment_cur); //ref: find the closest cluster from each point and store it in cluster_assignment_cur[]
    copy_assignment_array(n, cluster_assignment_cur, cluster_assignment_prev);

   // BATCH UPDATE
    double prev_totD = INF_DIST;
    int batch_iteration = 0;

    printf("Progressing...\n");
    printf("Repetition      Change      Total Dist.^2   Total Dist.^2 Change\n");
    while (batch_iteration < MAX_ITER)
      {
//        printf("batch iteration %d \n", batch_iteration);
//        diagnosis(dim, n, k, X, cluster_assignment_cur, cluster_centroid);

        // update cluster centroids
         calc_cluster_centroids(dim, n, k, X, cluster_assignment_cur, cluster_centroid);

        // deal with empty clusters
        // XXXXXXXXXXXXXX

        // see if we've failed to improve
         double totD = calc_total_distance(dim, n, k, X, cluster_centroid, cluster_assignment_cur);
         if (totD > prev_totD)
          // failed to improve - currently solution worse than previous
           {
            // restore old assignments
	         copy_assignment_array(n, cluster_assignment_prev, cluster_assignment_cur);

	        // recalc centroids
	         calc_cluster_centroids(dim, n, k, X, cluster_assignment_cur, cluster_centroid);

	         printf("  negative progress made on this step\n");
	         printf("  total distance of points to its centroids has increased by (%.2f) \n", totD - prev_totD);
	         printf("  centroids restored to the previous ones");
	         printf("  iteration completed \n");

            // done with this phase
             break;
           }

        // save previous step
         copy_assignment_array(n, cluster_assignment_cur, cluster_assignment_prev);

        // update: move all points to nearest cluster
         calc_all_distances(dim, n, k, X, cluster_centroid, dist);
         choose_all_clusters_from_distances(dim, n, k, dist, cluster_assignment_cur);

         int change_count = assignment_change_count(n, cluster_assignment_cur, cluster_assignment_prev);

         printf("%6d     %9d  %16.2f %17.2f\n", batch_iteration, change_count, totD, totD - prev_totD);
         fflush(stdout);

        // done with this phase if nothing has changed
         if (change_count == 0)
           {
             printf("\n  No change made - Iteration completed \n\n");
             break; // terminate
           }

         prev_totD = totD;

         batch_iteration++;
      }

diagnosis(dim, n, k, X, cluster_assignment_cur, cluster_centroid);


   // write to output array
    copy_assignment_array(n, cluster_assignment_cur, cluster_assignment_final);

    if (!dist || !cluster_assignment_cur || !cluster_assignment_prev)
      fail("Error while free-ing arrays");

    free(dist);
    free(cluster_assignment_cur);
    free(cluster_assignment_prev);
  
  }   // end of kmeans()
