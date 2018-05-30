//
//  datatype.h
//  CSE-6010 Assignment5
//
//  Created by Soo Hyeon Kim on 11/20/17.
//  Copyright © 2017 Soo Hyeon Kim. All rights reserved.
//
//  Student name: Soo Hyeon Kim
//  Student username: skim3026 (#:903230437)



#ifndef DATATYPE_H_
#define DATATYPE_H_

#define MAX_ANT_SIZE  100000
#define FOUND    1
#define NOTFOUND 0

/**
 * a structure to represent an ant. It includes the position as well as
 * the velocity of the ant
 */
struct AntPos
{
    float xPos;   //ant x position
    float yPos;   //ant y position
    float vX;     //ant volicity in the x direction
    float vY;     //ant volicity in the y direction
};

typedef struct AntPos AntInd; // individual Ant

struct AllAnts
{
   //number of ants
   int size;

   //an array of ants
   AntInd* ants;
};

typedef struct AllAnts AntInfo;

struct PT
{
    float x;
    float y;
};

typedef struct PT Point;

struct Cluster
{   
    int maxClusterIndex; //store max cluster's index
    int maxClusterValue; //store max cluster size

    int clusterCount; // total cluster number

    int* antsPerCluster; // clusters' ant number 

    Point* clusterCentroids; // clusters' centroid

    float averageXVelocity;
    float averageYVelocity;
};

typedef struct Cluster ClusterInfo; 
//holds 6 info: 1) cluster count; 2) each cluster size; 3) each cluster centroid
//              4) net velocity(x,y) 5) max cluster index 6) max cluster size


//clustering internal data structure
struct ClusterNode // each entity
{
    //the point which we need to cluster
    Point  pt;
    
    //is this node has been visited
    int label;
    
    //point cluster ID
    int clusterID;
};

typedef struct ClusterNode Node;

struct VectorClusterNodes // to hold all nodes
{
    int size;
    
    Node *nodes[MAX_ANT_SIZE];
};

typedef struct VectorClusterNodes VectorNodes;

//TODO: add other data strucure here

#endif /* DATATYPE_H_ */
