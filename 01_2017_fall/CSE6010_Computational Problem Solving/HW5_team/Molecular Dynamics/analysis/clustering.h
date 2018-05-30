//
//  clustering.h
//  CSE-6010 Assignment5
//
//  Created by Soo Hyeon Kim on 11/20/17.
//  Copyright © 2017 Soo Hyeon Kim. All rights reserved.
//
//  Student name: Soo Hyeon Kim
//  Student username: skim3026 (#:903230437)


#include "datatype.h"
/**
 * this is the main function which perform clustering. It takes
 * the ant info and return the clustering
 */
ClusterInfo* PerformClustering(AntInfo* antInfo, float threshold);

void GetBiggestCluster(ClusterInfo* info);

//print the reuslt of clustering
void PrintClusteringResults(ClusterInfo* info);

void FreeClusterInfo(ClusterInfo* info);
void FreeAntInfo(AntInfo* info);
void FreeClusterNodes(VectorNodes* clusterNodes);
