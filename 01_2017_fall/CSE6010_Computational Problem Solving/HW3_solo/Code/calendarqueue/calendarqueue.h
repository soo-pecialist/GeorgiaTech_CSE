//
//  calanderqueue.h
//  CSE-6010 Assignment3
//
//  Created by Soo Hyeon Kim on 10/9/17.
//  Copyright © 2017 Soo Hyeon Kim. All rights reserved.
//
//  Student name: Soo Hyeon Kim
//  Student username: skim3026 (#:903230437)

#include "cqdefine.h"
/*
 * A header file for defining a queue data structure called calendar queue based on the
 * idea of BROWN: Calendar Queues:  A Fast O(1) Priority Queue Implementation for
 * the Simulation Event Set Problem.
 */


//user data: can point to anything the user would like to store inside the queue
typedef void*  UserDataPtr;

//this structure  define an entry in the queue
struct QueueNode
{
	 //this field represent the priority of this node
     double priority;

     //a pointer to the additional user data
     UserDataPtr userData;

     //a pointer to the next node in the same bucket
     struct QueueNode* next;
};

//a wrapper to the QueueNode structure
typedef struct QueueNode QueueEntry;


/*
 * InitQueue( );
 * This function initializes an empty queue
 * input:
 *         enableResize: a flag indicating weather the container should be resized
 * return:
 *       a flag indicating if the job is successful or not
 */
int InitQueue(int enableResize);

/*
 * this initialize a static queue that does not change its size
 *input:
 *         qSize: the queue size
 * return:
 *       a flag indicating if the job is successful or not
 */
int InitStaticQueue(unsigned long qSize);

/*
 * int Enqueue(QueueEntry* entry, double priority);
 * this function adds an entry to the queue
 *Input:
 *     priority: associated priority of this entry.
 *     userData: a void pointer that can point to any user data
 *
 * return:
 *       an integer representing the state of the enqueue operation
 */
int Enqueue(double priority,UserDataPtr userData);

/*
 *int Dequeue(UserDataPtr* userData);
 *this function removes and returns the entry with the
 * the lowest priority from the calendar queue
 * this function returns CQ_EMPTY is the queue is empty or
 * CQ_SUCESS otherwise
 */
int Dequeue(UserDataPtr* userData);

/*
 *  the function should be called at the end to free the allocated memory
 *  by the queue.
 */
void Finalize();

/*
 * printQueue()
 * the function is useful for debugging purposes
 */
void PrintQueue();
