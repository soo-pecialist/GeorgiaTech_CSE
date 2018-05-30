//
//  fileio.c
//  CSE-6010 Assignment5
//
//  Created by Soo Hyeon Kim on 11/20/17.
//  Copyright © 2017 Soo Hyeon Kim. All rights reserved.
//
//  Student name: Soo Hyeon Kim
//  Student username: skim3026 (#:903230437)

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#include "fileio.h"
#include "string.h"
#include <time.h>

#define MAX_WORD 255

//Read entire ant body size, ant position data, ant velocity data 
AntInfo* ReadModelDataFromMemory(Ant* antArr,  int antCount)
{
  //read ant data from a file
  AntInfo* antInfo=NULL; // all ant info container

  //allocate memory for the ant info
  antInfo=(AntInfo*)malloc(sizeof(AntInfo));
  if(antInfo==NULL){
   printf("cannot allocate memory for all ants\n");
   return NULL;
   }

  //allocate memory for the array of ants
  antInfo->ants=(AntInd*)malloc(antCount*sizeof(AntInd));
  if(antInfo->ants==NULL){
      printf("cannot allocate memory for all ants\n");
      return NULL;
   }

  //set the number of ants
  antInfo->size=antCount;

  int i=0;

  for(int i=0;i<antCount;i++){
    //copy the read data
    antInfo->ants[i].xPos=antArr[i].x; // x position
    antInfo->ants[i].yPos=antArr[i].y; // y position
    antInfo->ants[i].vX=antArr[i].v_x; // x velocity
    antInfo->ants[i].vY=antArr[i].v_y; // y velocity
  }

  return antInfo;
  //holds: 1) size 2) x-y position 3) x-y velocity
}

// output.txt
void WriteResult(char* fileName, ClusterInfo* result, float time)
{
  FILE *fp;

  //open the file
  fp = fopen(fileName, "a"); //open for appending (file need not exist)

  //make sure the file has been correctly opened
  if(fp==NULL){
	  printf("cannot open the file: %s for reading",fileName);
	  return;
  }

  //print out into output file
  fprintf(fp,"======================time=%f=================================\n",time);
  fprintf(fp,"- Number of clusters: %d\n", result->clusterCount);
  fprintf(fp,"- [Cluster %d] is the one with the maximum number of Ants=%d\n",result->maxClusterIndex,result->maxClusterValue);
  fprintf(fp,"- Net velocity in the x and y respectively are: %f, %f\n\n", result->averageXVelocity,result->averageYVelocity);

  fprintf(fp,"Cluster\tSize\tCentroid\n");
  for(int i=1;i<=result->clusterCount;i++){
		Point center=result->clusterCentroids[i];
		fprintf(fp,"%d\t%d \t(%f,%f)\n", i, result->antsPerCluster[i], center.x, center.y);
	}

  //close the file
  fclose(fp);
}

int FindDistribution(int start, int end, AntInfo* result)
{
	int counter=0;

  //loop through all ants
	for(int i=0;i<result->size;i++)
	{  
    //get the y posiiton of ant
		float y=result->ants[i].yPos;
    //if between range, then add count
		if(y>start && y<=end) counter++;
	}

	return counter;
}

int IsFileExist(char* fileName)
{
	FILE *fp;

	fp = fopen(fileName, "r");
	//the file does not exist
	if(fp==NULL) return 0;

	//the file exist
	fclose(fp);
	return 1;
}

//output.csv
void WriteDistributions(char* fileName, AntInfo* aInfo,ClusterInfo* cInfo,
		                      float time, float height)
{
	 FILE *fp;

	 //before anything check if this file exist or not so that we can write the header
	 int fileState=IsFileExist(fileName); // exist(1), no(0)

	  //open the file
	  fp = fopen(fileName, "a+"); //open for reading and writing (append if file exists)

	  //make sure the file has been correctly opened
	  if(fp==NULL){
		  printf("cannot open the file: %s for reading",fileName);
		  return;
	  }

    //all ants
	  AntInd* ants=aInfo->ants; 

	  //if the file does not exist before, we write the header
    //header: timestamp, 0-1#, 1-2#, .... (height-1 - height)#, net Vx, net Vy
	  if(fileState==0){
      fprintf(fp, "timestamp\\y");
		  for(int i=0;i<(int)height;i++)
			 fprintf(fp, ", %d", i+1); // first cell is void (column for timestamp)

		  fprintf(fp, ", Net X Velocity");
		  fprintf(fp, ", Net Y Velocity");
	  }

	  //Otherwise, insert a new line at the file
	  fprintf(fp, "\n");

	  fprintf(fp,"%.4f",time) ;
	  //write the distribution to the file (insert in the next line)
    for (int i=0; i<(int)(height);i++){
       //count ant number in height (i, i+1]
    	 int count=FindDistribution(i,i+1, aInfo);
    	 fprintf(fp, ", %d", count);
    }

      //write the net velocity
      fprintf(fp, ", %f", cInfo->averageXVelocity);
      fprintf(fp, ", %f", cInfo->averageYVelocity);

      fclose(fp);
}

void GetFileName(const char* base, const int simTime, char* fullFile, const char* ext)
{
  // https://en.wikibooks.org/wiki/C_Programming/time.h/time_t
  // http://www.cse.unt.edu/~donr/courses/4410/NOTES/timeExamples.html
  time_t t = time(NULL);
  struct tm tm = *localtime(&t);

  //tm.tm_mon : 0~11
  sprintf(fullFile, "%s_ST%d_%d-%d-%d-%d-%d%s",base, simTime, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec,ext);
  //e.g., 1) Cluster_ST1000_11-24-14-36-31.txt  2) Distirbution_ST1000_11-24-14-36-31.csv
}




// //Not used
// //parse the first line in the file and returns the number of ants and the threshold
// void ParseFirstLine(char* line, int* N, float* threashold);

// //parse a positon line from the file
// void ParseLine(char* line, float* xpos, float* ypos, float* vx, float* vy);

// //given a string, it returns the first number after the positon pos
// float GetNumber(char* string, int* startPos);


// float GetNumber(char* string, int* startPos)
// {
//   int wordPos=0;
//   char c;
//   char word[MAX_WORD]; // holder before tossing the value to 'value'
//   float value=0; // return value
  
//   //at the begining our word is empty
//   strcpy(word,"");
    
//   //first read N
//   while((c=string[(*startPos)++])!='\0'){ //if not EOL
        
//     if(c==' ') break; // we get string before space(' ')
    
//      word[wordPos++]=c;
      
//   }
//   //convert this word into a number
//   word[wordPos]='\0';
//   value=atof(word);
    
//   return value;
// }

// void ParseFirstLine(char* line, int* N, float* threashold)
// {
//     int startPos=0;
    
//     (*N)=(int)GetNumber(line,&startPos);
//     (*threashold)=GetNumber(line,&startPos);
    
//     //printf("%d\n%f\n",n,thrd);
// }

// void ParseLine(char* line, float* xpos, float* ypos, float* vx, float* vy)
// {
//   int startPos=0;
//  (*xpos)=GetNumber(line,&startPos);
//  (*ypos)=GetNumber(line,&startPos);
//  (*vx)=GetNumber(line,&startPos);
//  (*vy)=GetNumber(line,&startPos);
    
//  //printf("%f, %f, %f, %f\n", (*xpos),(*ypos),(*vx),(*vy));
  
// }

// AntInfo* ReadModelData(char* fileName)
// {
//   FILE *fp;
//   //an array to store all ant info
//   AntInfo* antInfo=NULL;
    
//   int N=0;
//   float  threshold=0.0;
//   float x,y, vx,vy;
    
//   char fileReadBuffer[MAX_READ_BUFFER];
  
//   //open the file
//   fp = fopen(fileName, "r");

//   //cmake sure the file has been correctly opened
//   if(fp==NULL){
//       printf("cannot open the file: %s for reading",fileName);
//       return NULL;
//   }
   
//   //read the first line in the file
//   fgets(fileReadBuffer, MAX_READ_BUFFER, (FILE*)fp);
//   ParseFirstLine(fileReadBuffer,&N,&threshold);
//   printf("- Number of Ants: %d\n", N);
    
//   //allocate memory for the ant info
//   antInfo=(AntInfo*)malloc(sizeof(AntInfo));
//   if(antInfo==NULL){
//       printf("cannot allocate memory for all ants\n");
//       return NULL;
//    }

//   //allocate memory for the array of ants
//   antInfo->ants=(struct AntPos*)malloc(N*sizeof(struct AntPos));
//   if( antInfo->ants==NULL){
//         printf("cannot allocate memory for all ants\n");
//         return NULL;
//    }

//   //set the number of ants
//   antInfo->size=N;

//   int i=0;
    
//   //read the file rest
//   while (fgets(fileReadBuffer, MAX_READ_BUFFER, (FILE*)fp)!=NULL){
//       //printf("%s",fileReadBuffer);
//       ParseLine(fileReadBuffer, &x,&y, &vx,&vy);
      
//       //copy the read data
//       antInfo->ants[i].xPos=x;
//       antInfo->ants[i].yPos=y;
//       antInfo->ants[i].vX=vx;
//       antInfo->ants[i].vY=vy;

//       i++;
//   }
    
//   //close the file
//   fclose(fp);
    
//   return  antInfo;
// }