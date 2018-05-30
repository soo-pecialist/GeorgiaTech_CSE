/*
 ============================================================================
 Name        : ganalyser.h
 Author      : Soo Hyeon Kim
 Version     : 10/21/17
 Copyright   : © 2017 Soo Hyeon Kim. All rights reserved.
 Student name: Soo Hyeon Kim
 Username	 : skim3026 (#:903230437)
 ============================================================================
 */

#include "../graphgenerator.h"

//define the node colors during the visiting
#define WHITE 0
#define GRAY 1
#define BLACK 2

struct GraphNode{

	  unsigned int  nodeID;

	  int nodeColor;

	  //number of connected components
	  unsigned int conCount;

	  unsigned int stackEntryTimeStamp;
};

typedef struct GraphNode gNode;


void DFS(GraphType** G, unsigned long N);

void DFS_VISIT(GraphType** G, unsigned long N, gNode* u);

void PrintResult(unsigned int N);
