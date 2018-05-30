//
//  linearqueue.h
//  CSE-6010 Assignment3
//
//  Created by Soo Hyeon Kim on 10/8/17.
//  Copyright © 2017 Soo Hyeon Kim. All rights reserved.
//
//  Student name: Soo Hyeon Kim
//  Student username: skim3026 (#:903230437)

#include <stdio.h>
#include <stdlib.h>

struct Event {
	double timestamp;		// event timestamp
    void *AppData;			// pointer to event parameters
	struct Event *Next;		// priority queue pointer
};

// Function to remove smallest timestamped event
struct Event *lqRemove (void);

void lqSchedule (double ts, void *data);

