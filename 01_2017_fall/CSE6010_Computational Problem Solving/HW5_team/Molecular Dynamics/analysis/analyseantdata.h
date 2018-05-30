//
//  analyseantdata.h
//  CSE-6010 Assignment5
//
//  Created by Soo Hyeon Kim on 11/20/17.
//  Copyright © 2017 Soo Hyeon Kim. All rights reserved.
//
//  Student name: Soo Hyeon Kim
//  Student username: skim3026 (#:903230437)



#include "fileio.h"

//analyse the data directly from memory
void AnalyseAntDataFromMemory(char* clusteringFileName,
		                      char* distributionFilName,
		                      Ant* antArr,  int antCount,
		                      float timestamp, float height, float threshold);

// //main function to call -> not used
// void AnalyseAntData(char* inputFile, char* outputFile);