//
//  fileio.h
//  CSE-6010 Assignment5
//
//  Created by Soo Hyeon Kim on 11/20/17.
//  Copyright © 2017 Soo Hyeon Kim. All rights reserved.
//
//  Student name: Soo Hyeon Kim
//  Student username: skim3026 (#:903230437)

#include "datatype.h"
#include "../engine.h"

#define MAX_READ_BUFFER  1024

//Read entire ant body size, ant position data, ant velocity data 
AntInfo* ReadModelDataFromMemory(Ant* antArr,  int antCount);

//write the result data into a file. This function takes an array of ant info
void WriteResult(char* fileName, ClusterInfo* result,float time);

void WriteDistributions(char* fileName, AntInfo* result,
		                ClusterInfo* cInfo,float time, float height);

//return the file name using the time stamp
void GetFileName(const char* base, const int size, char* fullFile, const char* ext);


// Not used
//read the model data from a file. The function returns an arry of AntInfo
// AntInfo* ReadModelData(char* fileName);
