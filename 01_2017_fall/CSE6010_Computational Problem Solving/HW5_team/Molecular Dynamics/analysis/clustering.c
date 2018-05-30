//
//  clustering.c
//  CSE-6010 Assignment5
//
//  Created by Soo Hyeon Kim on 11/20/17.
//  Copyright © 2017 Soo Hyeon Kim. All rights reserved.
//
//  Student name: Soo Hyeon Kim
//  Student username: skim3026 (#:903230437)


#include <stdio.h>
#include <stdlib.h>

#include "clustering.h"
#include "memory.h"
#include "math.h"

//node labels while clustering
#define NOISE -1
#define UNDEFINED -2


//found the number of points that are located in a given range
void GetPointsInRange(Node* p, double range, VectorNodes* foundPoints,
		                                         VectorNodes* allPoints);

//get the distance between two points
float GetDistance(Point p1, Point p2);

//run the clustering algorithm
int DBSCANClustering(VectorNodes* allPoints,float eps, int minPts, Point* outClusterCentroid);

//process the result of clustering stage
ClusterInfo* ProcessClusteringResults(VectorNodes* cNodes,int cCount);

//perform the clustering
//called from AnalyseAntDataFromMemory()
ClusterInfo* PerformClustering(AntInfo* antInfo, float threshold)
{
  //create the data structure required for clustering // this holds all ants data
  VectorNodes* clusterNodes=(VectorNodes*)malloc(sizeof(VectorNodes));

  if(clusterNodes==NULL){
	  printf("cannot allocate memory for the ants\n");
	  return NULL;
  }

  //size of body
  clusterNodes->size=antInfo->size;

  //copy ant information into the clustering data structure
  for(int i=0;i<antInfo->size;i++)
  {
	  Node* node=(Node*)malloc(sizeof(Node));
	  if(node==NULL){
		  printf("cannot allocate memory for an ant\n");
		  return NULL;
	  }

     // copying ant's position for cluster
	  node->pt.x=antInfo->ants[i].xPos;
	  node->pt.y=antInfo->ants[i].yPos;
	  // default label is UNDEFINED (=not processed)
	  node->label=UNDEFINED;
	  // now cluster has ants' position info
	  clusterNodes->nodes[i]=node;
  }//end for

  //TODO:note that this will be freed when we free the cluster info
  Point* centroids=malloc(sizeof(Point)*antInfo->size);
  if(centroids==NULL){
	  printf("cannot allocate memory for an ant\n");
	  return NULL;
  }

  //perform the clustering
  //TODO: modify eps, minPts depending on your data
  int clusterCount=DBSCANClustering(clusterNodes,threshold,4,centroids); 
      // 1) clusterNodes->node[]->label's are updated
      // 2) centroids (x,y position) is secured

  ClusterInfo* info=ProcessClusteringResults(clusterNodes,clusterCount);
     // info: holds cluster data we need 
     //       - 1) cluster count; 2) each cluster size

  info->clusterCentroids=centroids;  // info += 3) each cluster centroid

  //free memory
  FreeClusterNodes(clusterNodes);

  return info;

}

void GetPointsInRange(Node* p, double eps, VectorNodes* foundPoints,
		                                       VectorNodes* allPoints)
{
    int k=0;
    
    Node** allNodes=allPoints->nodes;
    
    for (int i=0;i<allPoints->size;i++){
        
    	//calculate the distance between p(seed) and all points
    	float d=GetDistance(p->pt,allNodes[i]->pt);
        //check if this point is in the given distance range
        if(d<=eps){ // if so, add to neighbor 
            foundPoints->nodes[k++]=allNodes[i];
        }
    }
    
   // printf("%d\n",k);
    foundPoints->size=k; // update its neighbor number
}

float GetDistance(Point p1, Point p2)
{
  //calculate the eucludian distance
  float x2=pow(p1.x-p2.x,2);
  float y2=pow(p1.y-p2.y,2);
    
  double dist=sqrt(x2+y2);
    
  return dist;
}

int AreEqual(Point point1,Point point2)
{
	 if(point1.x==point2.x && point1.y==point2.y)
	 {
		 return 1; //equal
	 }

	 return 0; //not equal
}

//check if point is in v
int FindPoint(VectorNodes* v, Point point)
{
  for(int i=0;i<v->size;i++){

	 if(AreEqual(v->nodes[i]->pt,point)==1){
		 return FOUND; //1
	 }
  }

  return NOTFOUND; //0
}

void AddVectors(VectorNodes* dest, VectorNodes* source)
{
  int destSize=dest->size;
  int sourceSize=source->size;
    
   for(int i=0;i<sourceSize;i++){
	   if(FindPoint(dest,source->nodes[i]->pt)==NOTFOUND){ // avoid double count
		   dest->nodes[destSize++]=source->nodes[i];
	   }
   }

   //update dest->size (neighbor's neighbors are added)
   dest->size=destSize;
}

int DBSCANClustering(VectorNodes* allPoints, float eps, int minPts,Point* outClusterCentroid)
{
  //number of clusters are initially set to zero
  int clusterCount=0;
    
    //main loop - loop through every ant
    for(int i=0;i<allPoints->size;i++){
        
        Node* P=allPoints->nodes[i]; // seed
        if(P->label!=UNDEFINED) continue; // already processed - skip
        
        VectorNodes N;  // neighbor holder
        
        //find the set of points in the neighborhood of P
        GetPointsInRange(P,eps,&N,allPoints);
        //Now N holds neighbor ants info which is within radius of eps
        
        //check if this node is a noise one
        if(N.size<minPts){
            P->label=NOISE;
            continue;
        }

        //otherwise this is a core point
        clusterCount=clusterCount+1;
        P->label=clusterCount;
        
        //save the cluster centroid = seed's position
        outClusterCentroid[clusterCount]=P->pt;

        //use the seed S=N\{P} // check seed's neighbors
        for(int j=0;j<N.size;j++){ // process every neighbor point
          Node* Q=N.nodes[j];
          if(Q->label==NOISE) Q->label=clusterCount; // change Noise to border point - it's edge of cluster
          if(Q->label!=UNDEFINED) continue; // previously processed
          if(AreEqual(Q->pt, P->pt)==1) continue; //S=N-{P}  // same ant
        
          Q->label=clusterCount;  // Label neighbor as current cluster#
          VectorNodes N2;         // holder of neighbor's neighbors
          GetPointsInRange(Q,eps,&N2,allPoints); // find neighbors & density check
        
          //expand this cluter --> N = N + N2
          if(N2.size>=minPts){
              AddVectors(&N,&N2); // Add new neighbors to seed set
          } 
          // printf("\n%d %d\n", clusterCount, N.size);  

        } // # j loop end
    }// #i loop end

    return clusterCount;
}

ClusterInfo* ProcessClusteringResults(VectorNodes* cNodes, int cCount)
{
	ClusterInfo* info=(ClusterInfo*)malloc(sizeof(ClusterInfo)); //return data

	info->antsPerCluster=(int*)malloc(sizeof(int)*(cCount+1)); // it's because we count clusters from 1 not 0
	info->clusterCount=cCount; // total cluster number       // i.e., first cluster index is 1

	//collect the number of elments in each cluster
	for(int i=1;i<=cCount;i++)
		info->antsPerCluster[i]=0; // initialize with 0's

	for(int i=0;i<cNodes->size;i++){
      int c=cNodes->nodes[i]->label; // current ant's cluster label (cluster # or noise(-1))
      if(c>0)  //exclude noise and undefined nodes // i.e., NOISE(-1) exclusive
         info->antsPerCluster[c]=info->antsPerCluster[c]+1; // cluter ant number update
	}

	return info;
}

void GetBiggestCluster(ClusterInfo* info)
{
  //get the cluster with the max number of ants
  info->maxClusterValue=info->antsPerCluster[1]; // assume first cluster is biggest
  info->maxClusterIndex=1; // accordingly
  for(int i=2;i<info->clusterCount;i++){
    if(info->maxClusterValue<info->antsPerCluster[i]){ // new max --> update max info
      info->maxClusterIndex=i;
      info->maxClusterValue=info->antsPerCluster[i];
     }
  }
}

void PrintClusteringResults(ClusterInfo* info)
{
  //print out onto terminal
  printf("- Number of clusters: %d\n", info->clusterCount);
  printf("- [Cluster %d] is the one with the maximum number of Ants=%d\n",info->maxClusterIndex,info->maxClusterValue);
  printf("- Net velocity in the x and y respectively are: %f, %f\n\n", info->averageXVelocity,info->averageYVelocity);
  
  printf("Cluster\tSize\tCentroid\n");
  for(int i=1;i<=info->clusterCount;i++){
	    Point center=info->clusterCentroids[i];
		  printf("%d\t%d \t(%f,%f)\n", i, info->antsPerCluster[i], center.x, center.y);
	}
}

void FreeClusterInfo(ClusterInfo* info)
{
  free (info->antsPerCluster);
  free (info->clusterCentroids);
  free(info);
}


void FreeAntInfo(AntInfo* info)
{
  free(info->ants);
  free(info);
}

void FreeClusterNodes(VectorNodes* clusterNodes)
{
  for(int i=0;i<clusterNodes->size;i++){
    free(clusterNodes->nodes[i]);
  }

  free(clusterNodes);
}






