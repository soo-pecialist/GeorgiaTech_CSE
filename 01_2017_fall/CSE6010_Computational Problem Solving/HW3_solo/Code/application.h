/*
 * application.h
 *
 *  Created on: Oct 2, 2017
 *      Author: rahul
 */
#include <stdbool.h>


#ifndef PROJECT3_APPLICATION_H_
#define PROJECT3_APPLICATION_H_


// Event types
//#define	ARRIVAL     1
//#define	DEPARTURE   2
#define	INCOMING_ARRIVAL    1
#define	INCOMING_END   2
#define OUTGOING_MAKE 3
#define OUTGOING_CONNECT 4
#define OUTGOING_END 5
#define PRINT_STATS 6

#define INCOMING_CALL_MIN_DURATION 300 //300
#define INCOMING_CALL_MAX_DURATION 700 //700

#define OUTGOING_CALL_MIN_DURATION 200 //200
#define OUTGOING_CALL_MAX_DURATION 400 //400

#define INCOMING_CALL_QUEUING_PROB 0.75

#define SALES_AGENT 1
#define SERVICE_AGENT 2

#define STRUE 0.6

#if 0
	#define DEBUG(a) printf a
#else
	#define DEBUG(a) (void)0
#endif

#if 1
	#define INFO(a) printf a
#else
	#define INFO(a) (void)0
#endif

/////////////////////////////////////////////////////////////////////////////////////////////
//
// State variables  and other global information
//
/////////////////////////////////////////////////////////////////////////////////////////////

extern int sales_agent_count; //300
extern int service_agent_count; //700
extern double call_arrival_rate;

//For measuring metrics for incoming calls
extern double total_wait_time;
extern long int calls_in_queue;
extern int total_incoming_calls;
extern int waited_calls_count;
extern int no_waited_calls_count;
extern int hungup_calls_count;

//For measuring metrics for outgoing calls
extern int total_outgoing_calls;
extern int outgoing_calls_per_min;
extern int successful_calls_count;
extern int dropped_calls_count;
extern double Sest;
extern double Strue;

//Other metrics
extern double total_idle_sales_agents;
extern double total_idle_service_agents;

/////////////////////////////////////////////////////////////////////////////////////////////
//
// Data structures for event data
//
/////////////////////////////////////////////////////////////////////////////////////////////

// Events only have one parameter, the event type
struct EventData {
	int EventType;
	int AgentType; // To be used for knowing the kind of agent associated with an event. Used in incoming call end event
        bool IsQueuedCall; // to be used by the incoming call end event for calculating wait time
        double WaitTimestamp; // Timestamp of the call that was queued
};

// prototypes for event handlers
void Arrival (struct EventData *e);		// car arrival event
void Departure (struct EventData *e);	// car departure event

void incomingCallArrival (struct EventData *e);		// incoming call arrival event
void incomingCallEnd (struct EventData *e);	// incoming call departure event
void outgoingCallMake(struct EventData *e);
void outgoingCallConnect(struct EventData *e);
void outgoingCallEnd(struct EventData *e);
void printStats(struct EventData *e);

void usage(const char *programName);

double incomingCallTimeInterval(double A);
double incomingCallDuration();
double outgoingCallDuration();
bool isIncomingCallQueued(double probability);
bool isOutgoingCallAnswered(double Strue);

#endif /* PROJECT3_APPLICATION_H_ */
