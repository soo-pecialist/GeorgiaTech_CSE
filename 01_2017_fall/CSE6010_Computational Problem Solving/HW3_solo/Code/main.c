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
#include "./simengine/engine.h"
#include "application.h"

///////////////////////////////////////////////////////////////////////////////////////
//////////// MAIN PROGRAM
///////////////////////////////////////////////////////////////////////////////////////

int main (int argc, char* argv[])
{
	srand ( time(NULL) );

	if(argc < 2){
		usage(argv[0]);
		exit(1);
	}
        char *ptr;
	call_arrival_rate = (double) strtod(argv[1], &ptr);
        if (call_arrival_rate < 0){
            printf("ERROR: Invlalid value for call paramter\n");
            exit(1);
        }
            
        printf("Call Arrival Rate = %lf\n", call_arrival_rate);
	/*
	 *
	static struct call_queue* my_call_queue = NULL;
	queue_new();*/

    if(InitSimulation()!=SIM_SUCCESS){
        
        printf("Cannot initialize the simulation\n");
        return 0;
    }
    
	struct EventData *d;
        struct EventData *e;
        struct EventData *p;
	double ts;
        double ts_end = 60.0 * 180.0;

	// initialize event list with first arrival
	if ((d=malloc (sizeof(struct EventData))) == NULL) {fprintf(stderr, "malloc error\n"); exit(1);}
	d->EventType = INCOMING_ARRIVAL;
	ts = 0.0;
	Schedule (ts, d);
        
        printf("Time(min)\tService_Agents\tSales_Agents\tWaitListed_Calls\tOutgoing_Calls\tOutgoing/min\tStrue\tS_est\tSuccessful_Calls\tAbandoned_Calls\n");
        
        /*
         schedule outgoing calls here every 60 seconds*/
        for(ts = 60; ts <= ts_end; ts = ts + 60){
            if ((e=malloc (sizeof(struct EventData))) == NULL) {fprintf(stderr, "malloc error\n"); exit(1);}
            e->EventType = OUTGOING_MAKE;
            if ((p=malloc (sizeof(struct EventData))) == NULL) {fprintf(stderr, "malloc error\n"); exit(1);}
            p->EventType = PRINT_STATS;
            Schedule (ts, e);
            Schedule (ts, p);            
        }

	RunSimulation(ts_end);

	
        double wait_percent = ((double)waited_calls_count/total_incoming_calls)*100;
        double no_wait_percent = ((double)no_waited_calls_count/total_incoming_calls)*100;
        double hungup_calls_percent = ((double)hungup_calls_count/total_incoming_calls)*100;
        double avg_wait_time = (total_wait_time/(waited_calls_count-calls_in_queue)) > 0?(total_wait_time/(waited_calls_count-calls_in_queue)):0;        
        double avg_idle_sales_agents = total_idle_sales_agents/(ts_end/60);
        double avg_idle_service_agents = total_idle_service_agents/(ts_end/60);
        double avg_successful_calls_count = successful_calls_count/(ts_end/60);
        double avg_outgoing_calls_count = total_outgoing_calls/(ts_end/60);
        
        
        printf("\n\nResults of the Simulation for incoming calls part:\n\n");
	printf("Sales Agent Count at the end of simulation = %d\n", sales_agent_count);
	printf("Service Agent Count at the end of simulation = %d\n", service_agent_count);
	printf("Call Arrival Rate = %lf\n", call_arrival_rate);
	printf("Total incoming calls = %d\n", total_incoming_calls);
        printf("Percentage of Calls waited = %lf\n", wait_percent);
	printf("Percentage of Calls not waited = %lf\n", no_wait_percent);
	printf("Percentage of Hung Up calls = %lf\n", hungup_calls_percent);
        printf("Avg Wait time = %lf\n", avg_wait_time);
        printf("Incoming call Queue size at the end of simulation = %ld\n", calls_in_queue);
        printf("\nResults of the Simulation for outgoing calls part:\n\n");
        printf("Total outgoing calls = %d\n", total_outgoing_calls);
        printf("Average Number of outgoing calls per hour = %lf\n", avg_outgoing_calls_count);
        printf("Average Number of Successful outgoing calls per hour = %lf\n", avg_successful_calls_count);
        printf("Average Number of Idle Sales Agent = %lf\n", avg_idle_sales_agents);
	printf("Average Number of Idle Service Agent = %lf\n", avg_idle_service_agents);
        
      
}