/*
 * main.c
 *
 *  Created on: Oct 2, 2017
 *      Author: rahul
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <omp.h>
#include "engine.h"
#include "antsim.h"


//for analysis
#include "analysis/analyseantdata.h"


///////////////////////////////////////////////////////////////////////////////////////
//////////// MAIN PROGRAM
///////////////////////////////////////////////////////////////////////////////////////

int main (int argc, char* argv[]){
    
    srand ( time ( NULL));
    
    double ant_min_size = 1; //the ant is assumed to be a square with side ant_min_size cm
    double timeStep = 0.1; //Time step for the simulation
    double threshold = 1.5 * ant_min_size;
    
    if(argc < 6){
        usage(argv[0]);
        exit(1);
    }
        
    double H, W, R, simTime;

    sscanf(argv[1],"%d",&N);
    sscanf(argv[2],"%lf",&H);
    sscanf(argv[3],"%lf",&W);
    sscanf(argv[4],"%lf",&R);
    sscanf(argv[5],"%lf",&simTime);
      
    //***************** Input error checking **********************
    if(N < 0){
        printf("ERROR: Invalid value for number of ants!\n");
        exit(1);
    }
    
    if(H < 0.0 || W < 0.0){
        printf("ERROR: Incorrect container dimensions!\n");
        exit(1);
    }
    
    //TODO: Check with dimension of ant
    if(R > min(H, W) || R < ant_min_size){ //if the bounding box is larger than the smaller side of the container or is smaller than the smallest dimension of the an then it's an error
        printf("ERROR: Invalid value for Viscek bounding box/Viscek radius!\n");
        exit(1);
    }
    
    if(simTime < timeStep){ //Simulation time should be greater than the timestep at least
        printf("ERROR: Invalid value for simulation time! Timestep value is %lf. Simulation time should be greater than this value\n", timeStep);
        exit(1);
    }
    //*************************************************************
    
    N = ant_overflow_check(N, ant_min_size, W, H); //check if the entered amount of ants can be contained within the container     
    
    Ant* antArr =  malloc_ant_array(N); //malloc array of N ants
    
    int numBoxRows = (int)ceil(H/R);
    int numBoxCols = (int)ceil(W/R);
    int count=0;		//Caleb
        
    ViscekBox** boxMat = malloc_box_matrix(numBoxRows, numBoxCols); //divide the container into square regions of size R
    
    init_ant_array(N, ant_min_size, W, H, R, antArr); // initialize the ants with a position, velocity and a boxID
    
    init_box_matrix(boxMat, numBoxRows, numBoxCols);
  
    //Print box stats
    //print_box_stats(boxMat, numBoxRows, numBoxCols);
        

    ///===================================by Sean ==================
    char clusteringFileName[]="analysisresult/Cluster";
	char distributionFileName[]="analysisresult/Distirbution";
	char clusterOutputFileName[1024];
	char distOutputFileName[1024];
    // clusterOutputFileName = .txt file name
	GetFileName(clusteringFileName, (int)simTime, clusterOutputFileName, ".txt");
    // distOutputFileName = .csv file name
	GetFileName(distributionFileName, (int)simTime, distOutputFileName, ".csv");
	///=================================end ==================


    populate_box_matrix(boxMat, antArr, N);
    
    //Print ant stats
    //print_ant_stats(antArr, N);
    
    //Print box stats
    //print_box_stats(boxMat, numBoxRows, numBoxCols);
    
    srand(time(NULL));
    
    for(double time = 0.0; time <= simTime+1; time = time + timeStep){
        // call the interaction function on every ant
        for(int i = 0; i < N; i++){
            interact(&antArr[i], W, H, boxMat,R,numBoxRows,numBoxCols); //The interaction allows the ant to probe the 8 nearest box neighbors for neighboring ants to be influenced by them
        }
        // update the position of every ant after every interaction
        for(int i = 0; i < N; i++){



        		antArr[i].x += antArr[i].v_x * timeStep; //update x position. If ant is near the boundary of the container then align along the direction of the container
        		if(antArr[i].x < 0.0)
        			antArr[i].x = 0.0;
        		if(antArr[i].x > W )
        			antArr[i].x = W;
            
        		antArr[i].y += antArr[i].v_y * timeStep; //update y position
        		if(antArr[i].y < 0.0)
        			antArr[i].y = 0.0;
        		if(antArr[i].y > H )
        			antArr[i].y = H;

            antArr[i].v_x_old=antArr[i].v_x;  //Update velocity memory
            antArr[i].v_y_old=antArr[i].v_y;

            antArr[i].boxID.x = (int)(antArr[i].y/R); //update box to which the ant belongs to now
            antArr[i].boxID.y = (int)(antArr[i].x/R); 
        }
        
        //erase all data from the box matrix
        for(int r_itr = 0; r_itr < numBoxRows; r_itr++){
            for(int c_itr = 0; c_itr < numBoxCols; c_itr++){
                boxMat[r_itr][c_itr].numAnts = 0;
                free_ant_list(&boxMat[r_itr][c_itr].antList);
            }
        }
        //print_box_stats(boxMat, numBoxRows, numBoxCols);
        
        //update the box matrix for updated stats of ants
        init_box_matrix(boxMat, numBoxRows, numBoxCols);
        populate_box_matrix(boxMat, antArr, N);
        
        //printf("timestamp: %lf\n", time);
        //print_ant_stats(antArr, N);
        //print_box_stats(boxMat, numBoxRows, numBoxCols);
        //printf("*******************************************************\n\n");
        
        
        // print the positions of the ants into a file
        int tempTime = (int)(time*10);
        count++;
        //int timeStampFloor = (int)floor(time);
        //int timeStampCeil = (int)ceil(time);
        //printf("timeStampFloor = %d, timeStampCeil = %d, timeStamp = %lf, tempTime = %d\n", timeStampFloor, timeStampCeil, time, tempTime);
        //////////////////////////////////////////////////////////////
        // Change this back to mod 100!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        ///////////////////////////////////////////////////////////////
        //if(tempTime%1 == 0){ //Output snapshot every 10 seconds
        if(count%10==0){		//Caleb

            char timeStampString[50];
            char *filePrefix = "Ant_Array_Snapshot";
            char *fileFormat = ".txt";
            sprintf(timeStampString,"%d", count);
            char *result = malloc(strlen(filePrefix) + strlen(timeStampString) + strlen(fileFormat)+1); //+1 for the null-terminator
            strcpy(result, filePrefix);
            strcat(result, timeStampString);
            strcat(result, fileFormat);


            FILE *snapshot_file = fopen(result, "w");
            char outputFirstLine[100];
            //sprintf(outputFirstLine,"%d %lf\n", N, threshold);
            //fputs(outputFirstLine, snapshot_file);
            for(int i = 0; i < N; i++){
                char outputPosition[200];
                //sprintf(outputPosition,"%lf %lf %lf %lf\n", antArr[i].x, antArr[i].y, antArr[i].v_x, antArr[i].v_y);
                sprintf(outputPosition,"%lf %lf\n", antArr[i].x, antArr[i].y);
                fputs(outputPosition, snapshot_file);
            }
            fclose(snapshot_file);

            ///===================================by Sean ==================
            //perform the analysis
            AnalyseAntDataFromMemory(clusterOutputFileName,distOutputFileName,antArr,N,time,H, threshold);
            ///=================================end ==================
        }
    }
    
    free_box_matrix(numBoxRows, numBoxCols, boxMat);
    free(antArr);
    printf("INFO: Ant array freed!\n");
    return 0;
}
