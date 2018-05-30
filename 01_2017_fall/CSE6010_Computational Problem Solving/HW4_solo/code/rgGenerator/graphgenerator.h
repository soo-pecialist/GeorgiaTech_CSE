/*
 ============================================================================
 Name        : graphgenerator.h
 Author      : Soo Hyeon Kim
 Version     : 10/22/17
 Copyright   : © 2017 Soo Hyeon Kim. All rights reserved.
 Student name: Soo Hyeon Kim
 Username	 : skim3026 (#:903230437)
 ============================================================================
 */



#ifndef GRAPHGENERATOR_H_
#define GRAPHGENERATOR_H_

typedef char GraphType;

#define GEN_SUCESS 1
#define GEN_FAILURE -1

/**generate a graph using N and P
 * N: number of nodes
 * P: edge generation probability
 */
int GenerateGraph(unsigned long N, double P);

/**generate a graph using a parallel approach
 * N: number of nodes
 * P: edge generation probability
 * ThrdCount: number of threads
 */
int ParaGenerateGraph(unsigned long N, double P, int ThrdCount);

/**generate part of the graph
 * startIndex: start range
 * endIndex:  end range
 * P: edge generation probability
 */
int GenerateGraphRange(unsigned long startIndex, unsigned long endIndex,double P);

/** generate only one row of the adjacency matrix
 * rowID: the row id
 * N: number of nodes
 * P: edge generation probability
 */
int GenerateGraphRow(unsigned int rowID, double P,
                     unsigned long N, unsigned int* seed);

//a random number generator
double Random();

//a thread save version of the random number generator
double Random_P(unsigned int* seed);

/**allocate memory for the graph data structure
 * N: the graph number of nodes
 */
int AllocateMemory(unsigned long N);

GraphType** GetGraoh();

void DeleteGraph();

#endif /* GRAPHGENERATOR_H_ */
