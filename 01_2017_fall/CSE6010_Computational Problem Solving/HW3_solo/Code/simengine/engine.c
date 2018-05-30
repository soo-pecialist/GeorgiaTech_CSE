//
//  engine.c
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
//for the calendar queue
#include "../calendarqueue/calendarqueue.h"

#include "engine.h"

/*
 * Implementation file for the simulation engine.
 * The event list is stored and maintained by the calendar queue
 * including the user data (see the sample program for more information).
 */

struct SimEvent {
	 double timestamp;		// event timestamp also acts a priority
     void *AppData;			// pointer to event parameters
};

//a pointer data type to the SimEvent strucure
typedef struct SimEvent* SimEventPtr;

// Simulation clock variable
double CurrentSimTime = 0.0;


// Function to print timestamps of events in event list
void PrintList (void);

// Function to remove smallest timestamped event
SimEventPtr Remove (void);


// Remove smallest timestamped event from FEL, return pointer to this event
// return NULL if FEL is empty
SimEventPtr Remove (void)
{
	SimEventPtr e;

	int ret=Dequeue((UserDataPtr*)&e);

	if(ret==CQ_EMPTY){

		return NULL;
	}

    return (e);
}

// Print timestamps of all events in the event list (used for debugging)
void PrintList (void)
{
	 PrintQueue();
}

/////////////////////////////////////////////////////////////////////////////////////////////
// Simulation Engine functions visible to simulation application
/////////////////////////////////////////////////////////////////////////////////////////////

// Return current simulation time
double CurrentTime (void)
{
	return CurrentSimTime;
}

// Schedule new event in FEL
// queue is implemented as a timestamp ordered linear list
void Schedule (double ts, void *data)
{
	SimEventPtr e;

	// create event data structure and fill it in
	if ((e =(SimEventPtr) malloc (sizeof (struct SimEvent))) == NULL) exit(1);

	e->timestamp = ts;
	e->AppData = data;

	Enqueue(ts, e);
}

// Function to execute simulation up to a specified time (EndTime)
void RunSimulation (double EndTime)
{
	SimEventPtr e;

	//printf ("Initial event list:\n");

        //PrintList ();

	// Main scheduler loop
	while ((e=Remove()) != NULL
			&& CurrentSimTime<EndTime) {

		if(e==NULL)  return;

		double Now = e->timestamp;

		CurrentSimTime=Now;

		EventHandler(e->AppData);

                //printf("**Current Simulation time=%f**\n",CurrentSimTime);

		free (e);	// it is up to the event handler to free memory for parameters

            //PrintList ();
	}
}

int InitSimulation()
{

	   if(InitQueue(2)!=CQ_SUCESS){

			printf ("Cannot initialize the queue\n");

			return SIM_FAILURE;
		}

	   //initialize the random number generator
	   srand (time(NULL));

	   return SIM_SUCCESS;
}


//exponential random number generation function
double randexp(double lambda)
{
    double u;
    u = rand() / (RAND_MAX + 1.0);
    return -log(1- u) / lambda;
}

//uniform random number generation function
double urand(double min,double max)
{
	double u;
	    u = rand() / (RAND_MAX + 1.0);

	    return u*(max-min+1)+min;
}
