//
//  performancetest_dynamic.c
//  CSE-6010 Assignment3
//
//  Created by Soo Hyeon Kim on 10/9/17.
//  Copyright © 2017 Soo Hyeon Kim. All rights reserved.
//
//  Student name: Soo Hyeon Kim
//  Student username: skim3026 (#:903230437)

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>


#include "../calendarqueue/calendarqueue.h"
#include "../linearqueue/linearqueue.h"

#define N  10   //base size
#define PN 5  //Performance experiment, i.e., N**PN

/*
 * this file is used to test and compare the performance of the
 * calendar queue. We use a dynamic calendar queue
 */

struct SimpleUserData{

	unsigned long ID;
	double priority;
};

struct EventData {
	int EventType;
};

struct PerformanceData
{
	 int qSize;        //queue size

	 float cqRuntime;  //runtime for the calendar queue

	 float lqRuntime;  //runtime for the linear queue
};

//perform an experiment for a certain queue size
//it returns the runtime
float PerformExperimentCalQueue(unsigned long qSize);

float PerformExperimentLinearQueue(unsigned long qSize);

//a random number generator from exponential distribution
double ExpRand(double lambda);

//generate a uniform random number between min and max
double U(double min,double max);

//save to a csv file
void SaveTOCSV(const char* fileName, struct PerformanceData* pData);

int main()
{
int i;

//collected information about each experiment
struct PerformanceData  expInfo[PN];

       srand (time(NULL));

        for(i=1;i<=PN;i++){

        	expInfo[i-1].qSize=pow(N,i);
        	expInfo[i-1].cqRuntime=PerformExperimentCalQueue(expInfo[i-1].qSize);
        	expInfo[i-1].lqRuntime=PerformExperimentLinearQueue(expInfo[i-1].qSize);
        }


        printf("Q Size\t Calendar Queue\t Linear Queue\n");
        for(i=0;i<PN;i++)
        {
        	printf("%d\t %f\t %f\n",expInfo[i].qSize,expInfo[i].cqRuntime, expInfo[i].lqRuntime);
        }

        //write the result to a csv file
         SaveTOCSV("test_dynamic.csv",expInfo);

       //float rt=PerformExperimentLinearQueue(10);
       //printf("runtime=%f\n",rt);


     return 0;
}

float PerformExperimentCalQueue(unsigned long qSize)
{
int i;
	        //initialize the queue
		   if(InitQueue(1)!=CQ_SUCESS){
			//if(InitStaticQueue(qSize)!=CQ_SUCESS){

				printf ("Cannot initialize the queue\n");

				return 0;
			}


			//insert N events in the queue
			for (i=0;i<qSize;i++)
			{
	            //create a user data
				struct SimpleUserData* userData;

				userData=(struct SimpleUserData*)malloc(sizeof(struct SimpleUserData));

				if(userData==NULL){

					printf ("Cannot allocate memory for the user data, we return\n");

					return 0;
				}

				userData->ID=i;

				//generate a random number representing the priority
				userData->priority=U(0,qSize);

				//printf("%f\n",userData->priority);

				 Enqueue(userData->priority, userData);
			}

			//Enqueue and Dequeue
			int PerformanceExperiment=1000;

			clock_t t1,t2;

			t1=clock();

			for(i=0;i<PerformanceExperiment;i++)
			{
				struct SimpleUserData* userData;

			    Dequeue((UserDataPtr*)&userData);

			    double ts=userData->priority;

				double extra=ExpRand(1.0);

				ts+=extra;

			    Enqueue(ts, userData);

				//printf ("%f\n",userData->priority);
			}

			t2=clock();

			float diff = ((float)t2-(float)t1)/CLOCKS_PER_SEC;

			Finalize();

			return (diff*1000)/1000;
}

float PerformExperimentLinearQueue(unsigned long qSize)
{
int i;

                printf("inserting items in the queue, please wait ...\n");

				//insert N events in the queue
				for (i=0;i<qSize;i++)
				{
					struct EventData *d;
					double ts;

					 // initialize event list with first arrival
				     if ((d=(struct EventData*)malloc (sizeof(struct EventData))) == NULL) {fprintf(stderr, "malloc error\n"); exit(1);}

						d->EventType = -1;

						ts = ExpRand(0.5);

						lqSchedule (ts, d);

						if(i%1000==0)
						   printf("progress=%.1f\n",100*i/(float)qSize);
				}

				printf("finished... now we perform the testing...\n");


				//Enqueue and Dequeue
				int PerformanceExperiment=1000;

				clock_t t1,t2;

				t1=clock();

				for(i=0;i<PerformanceExperiment;i++)
				{
					struct Event *d;

					struct EventData *userData;

					 if ((userData=(struct EventData*)malloc (sizeof(struct EventData))) == NULL) {fprintf(stderr, "malloc error\n"); exit(1);}

					d=lqRemove ();

					double ts=d->timestamp;

					double extra=ExpRand(1.0);

				    ts+=extra;

				    //printf("%f,%f\n",ts,extra);

				    lqSchedule (ts, userData);

				}


				t2=clock();

				float diff = ((float)t2-(float)t1)/CLOCKS_PER_SEC;

				//free the memory
				struct Event *e;
				while((e=lqRemove ())!=NULL){

					free(e);
				}


				return (diff*1000)/1000;
}

double ExpRand(double lambda)
{
    double u;
    u = rand() / (RAND_MAX + 1.0);
    return -log(1- u) / lambda;
}

double U(double min,double max)
{
	double u;
	    u = rand() / (RAND_MAX + 1.0);

	    return u*(max-min+1)+min;
}

void SaveTOCSV(const char* fileName, struct PerformanceData* pData)
{
int i;

	   FILE *fp;

	   fp = fopen(fileName, "w+");

	   fprintf(fp,"Q Size, Calendar Queue, Linear Queue\n");
	   for(i=0;i<PN;i++)
	   {
		fprintf(fp,"%d, %f, %f\n",pData[i].qSize,pData[i].cqRuntime,pData[i].lqRuntime);
	   }

	   fclose(fp);
}
