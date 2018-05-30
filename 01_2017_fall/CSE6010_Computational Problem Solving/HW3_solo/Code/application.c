#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>
#include "./simengine/engine.h"
#include "application.h"

int sales_agent_count = 300; //300
int service_agent_count = 700; //700
double call_arrival_rate;

//For measuring outgoing metrics
double total_wait_time = 0.0;
int total_incoming_calls = 0;
int waited_calls_count = 0;
int no_waited_calls_count = 0;
int hungup_calls_count = 0;
long int calls_in_queue = 0;

//For measuring outgoing metrics
int total_outgoing_calls = 0;
int outgoing_calls_per_min = 0;
int successful_calls_count = 0;
int dropped_calls_count = 0;
double Sest = 1.0; //Start sim with this value
double Strue = 0.4;

//Other metrics
double total_idle_sales_agents = 0;
double total_idle_service_agents = 0;

int successful_calls_count_per_min = 0;
int prev_successful_calls_count = 0;
double Slimit = 0.4;

/////////////////////////////////////////////////////////////////////////////////////////////
//
// Event Handlers
// Parameter is a pointer to the data portion of the event
//
/////////////////////////////////////////////////////////////////////////////////////////////

// General Event Handler Procedure define in simulation engine interface
void EventHandler (void *data)
{
    struct EventData *d;
    
    // coerce type
    d = (struct EventData *) data;
    // call an event handler based on the type of event
    //if (d->EventType == ARRIVAL) Arrival (d);
    //else if (d->EventType == DEPARTURE) Departure (d);
    if (d->EventType == INCOMING_ARRIVAL) incomingCallArrival (d);
    else if (d->EventType == INCOMING_END) incomingCallEnd (d);
    else if (d->EventType == OUTGOING_MAKE) outgoingCallMake (d);
    else if (d->EventType == OUTGOING_CONNECT) outgoingCallConnect (d);
    else if (d->EventType == OUTGOING_END) outgoingCallEnd (d);    
    else if (d->EventType == PRINT_STATS) printStats (d);        
    else {fprintf (stderr, "Illegal event found %d\n", d->EventType); exit(1); }
}

void usage(const char *programName) {
    printf("usage: %s  A \n", programName);
    printf("where:\n");
    printf("    A   : arrival rate of calls per second.\n");
    printf("\nExample: %s 10\n", programName);
}

double incomingCallTimeInterval(double A){
    //double prob = (exp(-A)*pow(A, timeInstant));
    double random = (double)rand() / (double)((unsigned)RAND_MAX + 1); // Random number between [0, 1); excludes 1 and prevents integer overflow in expression
    double interval = - (double)(log(random)/(A));
    return interval;
}

double incomingCallDuration(){
    return (rand() % (INCOMING_CALL_MAX_DURATION + 1 - INCOMING_CALL_MIN_DURATION) + INCOMING_CALL_MIN_DURATION);	
}

double outgoingCallDuration(){
    return (rand() % (OUTGOING_CALL_MAX_DURATION + 1 - OUTGOING_CALL_MIN_DURATION) + OUTGOING_CALL_MIN_DURATION);	
}

bool isIncomingCallQueued(double probability){
    return rand() <  probability * ((double)RAND_MAX + 1.0);
}

bool isOutgoingCallAnswered(double Strue){
    return rand() <  Strue * ((double)RAND_MAX + 1.0);
}

void incomingCallArrival (struct EventData *e){
	struct EventData *d;
	double ts;

	total_incoming_calls++;

	if (e->EventType != INCOMING_ARRIVAL) {fprintf (stderr, "Unexpected event type\n"); exit(1);}

        //for incoming calls that get answered right away
	if ((d=malloc (sizeof(struct EventData))) == NULL) {fprintf(stderr, "malloc error\n"); exit(1);}
	d->EventType = INCOMING_END;

	DEBUG (("Incoming Call Arrival event at time %f\n", CurrentTime()));
	
	//Let the agents take the incoming call only if there is no calls queued up
	if(calls_in_queue == 0){ 
		if(service_agent_count > 0){
			DEBUG (("Routed to service agent = %d\n", service_agent_count));
			service_agent_count--;
			no_waited_calls_count++;
			d->AgentType = SERVICE_AGENT;
                        d->IsQueuedCall = false;
                        ts = CurrentTime() + incomingCallDuration(); //duration of this call will be between 300 and 700 seconds
			Schedule (ts, d);	//schedule this call's end after 300 to 700 seconds as this has been successfully picked up
		}
		else if(sales_agent_count > 0){
			DEBUG (("Routed to sales agent = %d\n", sales_agent_count));
			sales_agent_count--;
			no_waited_calls_count++;
			d->AgentType = SALES_AGENT;
                        d->IsQueuedCall = false;
                        ts = CurrentTime() + incomingCallDuration(); //duration of this call will be between 300 and 700 seconds
			Schedule (ts, d);
		}
		else{
			DEBUG (("No one to pickup. Incoming call queued\n"));
			if(isIncomingCallQueued(INCOMING_CALL_QUEUING_PROB)){
                            calls_in_queue++;
                            d->IsQueuedCall = true;
                            ts = CurrentTime(); //duration of this call will be between 300 and 700 seconds
                            Schedule (ts, d);
                            waited_calls_count++;
			}
			else{	
                            hungup_calls_count++;
			}

		}
	}
	//If calls have been queued up, then let freed up agents answer them. Queue the recently incoming calls. Precedence for those calls that came in earlier
	else{
		DEBUG (("Incoming call queued as there were calls waiting in queue from before\n"));
		//Either the present call will get queued or it will be hung up
		if(isIncomingCallQueued(INCOMING_CALL_QUEUING_PROB)){
                    calls_in_queue++;
                    d->IsQueuedCall = true;
                    ts = CurrentTime(); //duration of this call will be between 300 and 700 seconds
                    Schedule (ts, d);
                    waited_calls_count++;
		}
		else{	
                    hungup_calls_count++;
		}
		
		//What if no agents available? Well the current call has been queued. Nothing else to do in this block
	}

	// schedule next call arrival event after 'x' seconds, 'x' value decided by Poisson's distribution
	if((d=malloc(sizeof(struct EventData)))==NULL) {fprintf(stderr, "malloc error\n"); exit(1);}
	d->EventType = INCOMING_ARRIVAL;
	ts = CurrentTime() + incomingCallTimeInterval(call_arrival_rate); //Distributed according to Poisson's distribution
	Schedule (ts, d);

	free (e);	// don't forget to free storage for event!
}

// event handler for call end events
void incomingCallEnd (struct EventData *e)
{
	struct EventData *d;
	double ts;
    
    	//printf ("Processing Departure event at time %f, AtPump=%d\n", CurrentTime(), AtPump);
	DEBUG(("Incoming Call end Event\n"));
	
	if (e->EventType != INCOMING_END) {fprintf (stderr, "Unexpected event type\n"); exit(1);}

	if(e->IsQueuedCall == false){
            if(e->AgentType == SALES_AGENT){	// update status variables here
                    sales_agent_count++;
            }
            else if(e->AgentType == SERVICE_AGENT){
                    service_agent_count++;
            }
        }

	if ((d=malloc (sizeof(struct EventData))) == NULL) {fprintf(stderr, "malloc error\n"); exit(1);}
        d->EventType = INCOMING_END;
        ts = CurrentTime() + incomingCallDuration(); //to schedule the queued calls next to be answered
        
        //Once a agent completes the call, the next queued call is is answered by the agent
	if(calls_in_queue > 0 && e->IsQueuedCall){
		DEBUG (("Calls waiting in queue will be answered now\n"));
		
		if(service_agent_count > 0){
			DEBUG (("Already queued call routed to service agent = %d\n", service_agent_count));
			service_agent_count--;
			d->AgentType = SERVICE_AGENT;
			calls_in_queue--;
			total_wait_time += CurrentTime() - e->WaitTimestamp;	
		}
		else if(sales_agent_count > 0){
			DEBUG (("Routed to sales agent = %d\n", sales_agent_count));
			sales_agent_count--;
			d->AgentType = SALES_AGENT;
			calls_in_queue--;
			total_wait_time += CurrentTime() - e->WaitTimestamp;	
		}
		Schedule (ts, d);
	}
	// If another vehicle waiting to use pump, allocate pump to vehicle, schedule its departure event
	free (e);	// release memory for data part of event
}

void outgoingCallMake(struct EventData *e){
    struct EventData *d;
    double ts;
    
    DEBUG(("Outgoing Call Make Event\n"));
	
    if (e->EventType != OUTGOING_MAKE) {fprintf (stderr, "Unexpected event type\n"); exit(1);}
    
    ts = CurrentTime() + 10; //connecting a call takes 10 more seconds
    
    outgoing_calls_per_min = sales_agent_count/Sest; // N
    
    //We need to make N outgoing calls for the next 60 seconds
    for(int i = 0; i < outgoing_calls_per_min; i++){
        if ((d=malloc (sizeof(struct EventData))) == NULL) {fprintf(stderr, "malloc error\n"); exit(1);}
        d->EventType = OUTGOING_CONNECT;
        Schedule (ts, d); //uniformly distribute the N calls within one minute
        total_outgoing_calls++;        
    }
    
    free(e);
    
}

void outgoingCallConnect(struct EventData *e){
    struct EventData *d;
    double ts;
    
    DEBUG(("Outgoing Call Connect Event\n"));
	
    if (e->EventType != OUTGOING_CONNECT) {fprintf (stderr, "Unexpected event type\n"); exit(1);}
    
    if ((d=malloc (sizeof(struct EventData))) == NULL) {fprintf(stderr, "malloc error\n"); exit(1);}
    d->EventType = OUTGOING_END;
    
    if(sales_agent_count > 0 && isOutgoingCallAnswered(Strue)){  //probabilty of call going through depends on Strue. Sest has to change according to changes in state variables
        ts = CurrentTime() + outgoingCallDuration();
        sales_agent_count--;
        Schedule (ts, d);
        successful_calls_count++;
        successful_calls_count_per_min++;        
    }
    else {
        dropped_calls_count++; // Abandoned calls
    }
    double tmp = Sest/2 + ((double)successful_calls_count_per_min/outgoing_calls_per_min/2); //Estimating S_est here
    
    if(call_arrival_rate > 1.4)Sest = (tmp < 0.8*Slimit)?Sest:tmp; //Error protection
    if(call_arrival_rate <= 1.4)Sest = tmp; 
    
    free(e);
}

void outgoingCallEnd(struct EventData *e){
    
    DEBUG(("Outgoing Call End Event\n"));	
    if (e->EventType != OUTGOING_END) {fprintf (stderr, "Unexpected event type\n"); exit(1);}
    
    sales_agent_count++;
    
    free(e);
    
}

void printStats(struct EventData *e){
    int ts = (int)CurrentTime()/60;
    if (e->EventType != PRINT_STATS) {fprintf (stderr, "Unexpected event type\n"); exit(1);}
    printf("%d\t%d\t%d\t%d\t%d\t%d\t%lf\t%lf\t%d\t%d\n", ts, service_agent_count, sales_agent_count, waited_calls_count, total_outgoing_calls, outgoing_calls_per_min, Strue, Sest, successful_calls_count, dropped_calls_count);
    
    //Changing Strue values for observing the chase of Strue by S_est
    if(ts == 60){
        Strue = 0.6;
    }
    if(ts == 120){
        Strue = 0.4;
    }
    
    total_idle_sales_agents += sales_agent_count;    //for calculating the averages
    total_idle_service_agents += service_agent_count;
    successful_calls_count_per_min = 0; //For S_est
    free(e);
}
