//
//  linearqueue.c
//  CSE-6010 Assignment3
//
//  Created by Soo Hyeon Kim on 10/8/17.
//  Copyright © 2017 Soo Hyeon Kim. All rights reserved.
//
//  Student name: Soo Hyeon Kim
//  Student username: skim3026 (#:903230437)

#include "linearqueue.h"

struct Event FEL ={-1.0, NULL, NULL};

// Remove smallest timestamped event from FEL, return pointer to this event
// return NULL if FEL is empty
struct Event *lqRemove (void)
{
    struct Event *e;

    if (FEL.Next==NULL) return (NULL);
    e = FEL.Next;		// remove first event in list
    FEL.Next = e->Next;
    return (e);
}

// Schedule new event in FEL
// queue is implemented as a timestamp ordered linear list
void lqSchedule (double ts, void *data)
{
	struct Event *e, *p, *q;

	// create event data structure and fill it in
	if ((e = (struct Event*)malloc (sizeof (struct Event))) == NULL) exit(1);
	e->timestamp = ts;
	e->AppData = data;

	// insert into priority queue
	// p is lead pointer, q is trailer
	for (q=&FEL, p=FEL.Next; p!=NULL; p=p->Next, q=q->Next) {
		if (p->timestamp >= e->timestamp) break;
		}
	// insert after q (before p)
	e->Next = q->Next;
	q->Next = e;
}

