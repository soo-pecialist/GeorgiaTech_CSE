//
//  analyseantdata.c
//  CSE-6010 Assignment5
//
//  Created by Soo Hyeon Kim on 11/20/17.
//  Copyright © 2017 Soo Hyeon Kim. All rights reserved.
//
//  Student name: Soo Hyeon Kim
//  Student username: skim3026 (#:903230437)


#include "fileio.h"
#include "clustering.h"
#include "memory.h"

#include <stdio.h>

// #include "../engine.h"  // included in fileio.h
#include "analyseantdata.h"

//calcualte the average velocity in the x and y direction and add it to the Cluster Info
void CalculateNetVelocity(ClusterInfo* clusterInfo, AntInfo* antInfo);

// this is called from main.c
void AnalyseAntDataFromMemory(char* clusteringFileName,
                              char* distributionFileName,
                              Ant* antArr,  int antCount, float timestamp, float height, float threshold)
{

  AntInfo* antInfo=ReadModelDataFromMemory(antArr,antCount);
  //holds: 1) size 2) x-y position 3) x-y velocity

  if(antInfo==NULL) return;

  //cluster the ant data
  ClusterInfo* clusterInfo=PerformClustering(antInfo, threshold);
  //clusterInfo now has: 1) cluster count; 2) each cluster size; 3) each cluster centroid

  CalculateNetVelocity(clusterInfo, antInfo); //clusterInfo += 4) net velocity

  GetBiggestCluster(clusterInfo); //clusterinfo += 5) max cluster index 6) max cluster size
  
  //TODO: if you want to see clustering process, uncomment
  // PrintClusteringResults(clusterInfo);

  //write result to a file
  WriteResult(clusteringFileName,clusterInfo,timestamp); // to outputFileName.txt
  WriteDistributions(distributionFileName,antInfo,clusterInfo,timestamp,height); // to "distribution.csv" (adding one line each time)

  //free the cluster info
  FreeClusterInfo(clusterInfo);
  FreeAntInfo(antInfo);

}

void CalculateNetVelocity(ClusterInfo* clusterInfo, AntInfo* antInfo)
{
   clusterInfo->averageXVelocity=0;
   clusterInfo->averageYVelocity=0;

   //sum all velocities
   for(int i=0;i<antInfo->size;i++){
	   clusterInfo->averageXVelocity+=antInfo->ants[i].vX;
	   clusterInfo->averageYVelocity+=antInfo->ants[i].vY;
   }

   //take the average
   clusterInfo->averageXVelocity/=antInfo->size;
   clusterInfo->averageYVelocity/=antInfo->size;
}



// //Not used
// void AnalyseAntData(char* inputFileName, char* outputFileName)
// {
//   //read ant data from a file
//   AntInfo* antInfo=ReadModelData(inputFileName);
    
//   if(antInfo==NULL) return;
   
//   //cluster the ant data
//   ClusterInfo* clusterInfo=PerformClustering(antInfo);
//   //clusterInfo now has: 1) cluster count; 2) each cluster size; 3) each cluster centroid

//   CalculateNetVelocity(clusterInfo, antInfo); //clusterInfo += 4) net velocity

//   PrintClusteringResults(clusterInfo);

//   //write result to a file
//   WriteResult(outputFileName,clusterInfo,0); // to outputFileName.txt
//   WriteDistributions("distibution.csv",antInfo,clusterInfo,0,0); // to "distribution.csv" (adding one line each time)

//   //free the cluster info
//   FreeClusterInfo(clusterInfo);

//   FreeAntInfo(antInfo);
// }

