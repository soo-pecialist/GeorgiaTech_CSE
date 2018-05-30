//
//  engine.h
//  CSE-6010 Assignment3
//
//  Created by Soo Hyeon Kim on 10/9/17.
//  Copyright © 2017 Soo Hyeon Kim. All rights reserved.
//
//  Student name: Soo Hyeon Kim
//  Student username: skim3026 (#:903230437)

#define SIM_SUCCESS 1
#define SIM_FAILURE 0

//initialize the simulation. This function should be called first before
//calling any of the other engine API.
//it returns SIM_SUCCESS, if the simulation has been initialized without
//any problems and SIM_FAILURE otherwize.

int InitSimulation();

// Call this procedure to run the simulation indicating time to end simulation
void RunSimulation (double EndTime);

// Schedule an event with timestamp ts, event parameters *data
void Schedule (double timeStamp, void *data);

// This function returns the current simulation time
double CurrentTime (void);

//
// Function defined in the simulation application called by the simulation engine
//
//  Event handler function: called to process an event
void EventHandler (void *data);

//////////////////////////////////////////////////////
//random number generation functions
/////////////////////////////////////////////////

//uniform random number generation function
double randexp(double lambda);

//exponential random number generation function
double urand(double min,double max);

