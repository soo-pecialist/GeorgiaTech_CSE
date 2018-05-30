//
//  calendarqueue.c
//  CSE-6010 Assignment3
//
//  Created by Soo Hyeon Kim on 10/9/17.
//  Copyright © 2017 Soo Hyeon Kim. All rights reserved.
//
//  Student name: Soo Hyeon Kim
//  Student username: skim3026 (#:903230437)

#include "calendarqueue.h"
#include "cqdefine.h"

//standard include files
#include <stdio.h>
#include <stdlib.h>

//TODO:uncomment this if you would like to show runtime messages
//or use it into your client program. You can also give it to your compiler
// #define DEBUG_INFO

/*a structure representing the queue.
 * This structure has been added to the implementation file
 * so that the queue data structure is not accessed from
 * outside the interface functions. We have added all the required
 * information about the queue to this structure.
 */
struct QueueInfo
{
	//an array that is used to store the buckets
	QueueEntry** qBuckets;

    //the number of buckets currently in the queue
    unsigned long qBucketCount;

    //the width of each bucket
    double bWidth;

    //queue size
    unsigned long qSize;

    //an upper threshold for the number of buckets
    unsigned long qBucketCountUpperThreshold;

    //a lower threshold for the number of buckets in the queue
    unsigned long qBucketCountLowerThreshold;

    //the queue last dequeued bucket
    unsigned long qLastbucket;

    //the priority at the top of the top bucket
    double qBuckettopPriority;

    double qLastPriority;

    //a flag to govern if the queue resize is enabled or not
    int qResizeEnabled; // 0: diable, other: okay

};

typedef struct QueueInfo cqQueue;

//an instance of the queue
cqQueue* pcQueue=NULL;


/*
 * a helper function that initialize the queue after each resize
 */
int LocalInit(unsigned long nbuck, double bwidth, double startprio);

/*
 * a helper function that insert an entry into the queue
 */

int InsertEntry(QueueEntry* newEntry, QueueEntry** listHead);

/*
 * a helper function that remove and return an entry at bucket i
 */
QueueEntry* RemoveEntry(unsigned long index);

/*
 * a function to calculate a new bucket width for the queue
 * when the resize function is called
 */

double CalculateNewBucketWidth();

/*
 * resize the queue to thenewSizen
 */
int ResizeQueue(int newSize);

/*
 *  a helper function that finds an event with the lowest priority
 *  Return: the index of the bucket with min. priority
 */
unsigned long FindMinPriorityBucket();

//used internally to remove an entry
int RemoveTopEntry(QueueEntry** topEntry);

int InitQueue(int enableResize)
{
	    //allocate memory
	    pcQueue=(cqQueue*)malloc(sizeof(cqQueue));

	    //if we cannot allocate memory, we return
	    if(pcQueue==NULL){

	    	return CQ_ERROR_NO_SUFFICIENT_MEMORY;
	    }


	    //enable resize
	    pcQueue->qResizeEnabled=enableResize;

	    return LocalInit(2, 1.0, 0.0); // according to Brown's paper, we initialize bwidhth to 1.0
}

//this function initialize a static queue that does NOT change its size
int InitStaticQueue(unsigned long qSize)
{
	    //allocate memory
		pcQueue=(cqQueue*)malloc(sizeof(cqQueue));

		//if we cannot allocate memory, we return
		if(pcQueue==NULL){

			return CQ_ERROR_NO_SUFFICIENT_MEMORY;
		}


		//disable resize (resize not allowed)
		pcQueue->qResizeEnabled=0;

		return LocalInit(qSize, 1.0, 0.0);
}

void Finalize()
{
//UserDataPtr dummy;

	  //first we free the currently allocated nodes
	 // while(Dequeue(&dummy)!=CQ_EMPTY);

	  // free the buckets
	  free (pcQueue->qBuckets);

	  //free the queue
      free (pcQueue);
}

int Enqueue(double priority,UserDataPtr userData)
{

	  QueueEntry* newEntry;

	  //allocate memory for the new entry
	  newEntry=(QueueEntry*)malloc(sizeof(QueueEntry));

	  //assign the priority to this entry
	  newEntry->priority=priority;
	  newEntry->userData=userData;

	  /*find the right bucket in which the entry will be placed
	  /this will depend on the given priority
	  /this is the virtual index*/
	  unsigned long  index=priority/pcQueue->bWidth;

	  //get the actual index
	  index=index % (pcQueue->qBucketCount);

	 // printf("priority %f has assigned index %ld\n",priority,index);

	  //insert this entry in the queue
	  int ret= InsertEntry(newEntry, &pcQueue->qBuckets[index]);

      //check if we need to resize the queue
	  if(pcQueue->qSize>pcQueue->qBucketCountUpperThreshold &&
			  pcQueue->qResizeEnabled==1) {

#ifdef DEBUG_INFO
		  printf("\nResizing the queue!\n");

		  printf("  --------->> before resizing <<--------\n");
		  PrintQueue();
#endif
		  ResizeQueue(2*pcQueue->qBucketCount);
#ifdef DEBUG_INFO
		  printf("  -------------->> after <<-------------\n");
		  PrintQueue();
#endif
	  }

	  return ret;
}

int Dequeue(UserDataPtr* userData)
{
   QueueEntry* topEntry;

	     int ret=RemoveTopEntry(&topEntry);

	     if(ret==CQ_SUCESS){

	    	 (*userData)=topEntry->userData;

	    	 //we can delete this entry
	    	 free(topEntry);
	     }

	     return ret;
}

int RemoveTopEntry(QueueEntry** topEntry)
{
	//if the queue is empty, we return NULL
	 if (pcQueue->qSize==0){
		 return CQ_EMPTY;
	 }

	 //we search the bucket for the entry of SMALLEST priority
	 register int i=pcQueue->qLastbucket; // To facilitate procedure, store the value in register

	 while(1){
		 //for debugging
		 //printf("qBuckettopPriority=%f\n",pcQueue->qBuckettopPriority);
		 //printf("bWidth=%f\n",pcQueue->bWidth);

		 //if we find the corresponding entry bucket
		 if(pcQueue->qBuckets[i]!=NULL &&
			pcQueue->qBuckets[i]->priority<pcQueue->qBuckettopPriority)
		 {
			 //update the last bucket index (i.e., to current one)
			 pcQueue->qLastbucket=i;

			 //remove the located entry
			 QueueEntry* foundEntry=RemoveEntry(i);

			 //update the last priority field
			 pcQueue-> qLastPriority=foundEntry->priority;

			 (*topEntry)=foundEntry;

			 return CQ_SUCESS;
		 }
		 else{ // if we don't find

			 //check next bucket by increasing the index
			 i++;

			 if(i==pcQueue->qBucketCount)  i=0;

			 //increase the bucket top priority
			 pcQueue->qBuckettopPriority += pcQueue->bWidth;

			 //if we reach end of the queue, then we can use direct search (see below)
			 if(i==pcQueue->qLastbucket) break;

		 }

	 }

	 //direct search
	 unsigned long minIndex=FindMinPriorityBucket();

	 //set the search parameters
	 pcQueue->qLastbucket=minIndex;

	 //last priority
	 pcQueue->qLastPriority=pcQueue->qBuckets[minIndex]->priority;

	 //pcQueue->qBuckettopPriority=pcQueue->bWidth*(minIndex+1);
	 pcQueue->qBuckettopPriority = (float)((int)(pcQueue->qLastPriority / pcQueue->bWidth) + 1) *
			                           pcQueue->bWidth + (0.5 * pcQueue->bWidth);

	 //repeat the search
	 return RemoveTopEntry(topEntry);
}

unsigned long FindMinPriorityBucket()
{
	unsigned long i;

	      unsigned  long minIndex=0;
	      double  minValue=0;

	      for (i=0;i<pcQueue->qBucketCount;i++){
	    	  if(pcQueue->qBuckets[i]!=NULL){

	    		  minIndex=i;
	    		  minValue=pcQueue->qBuckets[i]->priority;

	    		  break;
	    	  }
	      }

	      for (i=minIndex;i<pcQueue->qBucketCount;i++){
	    	  //we check only the first buck in the queue
	    	  if(pcQueue->qBuckets[i]!=NULL &&
	    		 pcQueue->qBuckets[i]->priority<minValue){
                  
                    minIndex=i;
                    minValue=pcQueue->qBuckets[i]->priority;
	    	  }

	      }

	      return minIndex;
}


int LocalInit(unsigned long nbuck, double bwidth, double startprio)
{
	     unsigned long n;
	     long int i;

	     pcQueue->qBuckets=(QueueEntry**)calloc(nbuck,sizeof(QueueEntry*));

		 //initialize the buckets
		 for (i=0;i<nbuck;i++)
		 {
			 pcQueue ->qBuckets[i]=NULL;
		 }

	     //set the number of buckets
	     pcQueue->qBucketCount = nbuck;

	     //set the width
	     pcQueue->bWidth=bwidth;

	     //reset the queue size
	     pcQueue->qSize=0;

	     //reset last priority to the given one
	     pcQueue->qLastPriority=startprio;

	     n=startprio/pcQueue->bWidth; // find virtual bucket

	     pcQueue->qLastbucket=n % pcQueue->qBucketCount; // find actual bucket

	     pcQueue->qBuckettopPriority=(n+1)*pcQueue->bWidth + pcQueue->bWidth*0.5; // buckettop's 0.5w bigger

	     //set the upper and lower queue size threshold
         //upper_threshold is twice the current number of bucket
	     pcQueue->qBucketCountLowerThreshold=(pcQueue->qBucketCount/2)-2;
         //lower_threshold is half the current number of bucket
	     pcQueue->qBucketCountUpperThreshold=2*pcQueue->qBucketCount;

	     return CQ_SUCESS;
}
/*
 * a helper function that insert an entry into the queue
 */

int InsertEntry(QueueEntry* newEntry, QueueEntry** listHead)
{
	      //insert the this entry in the queue
		  //case 1: the queue is empty
		  //case 2: the new entry has priority higher than the first node (in our case priority is ascending)
		  if((*listHead)==NULL ||
			  newEntry->priority<(*listHead)->priority) // (*newEntry).priority < (*(*listHead)).priority
		  {
		  	   //add the new node to the list head
		  	    newEntry->next=(*listHead);

		  	    (*listHead)=newEntry;
		  }
		  //case 3: the new node will be inserted after the head
		  else{

				 QueueEntry* currentEntry = (*listHead);

				 //we will try to locate its correct place
				 while (currentEntry->next!=NULL &&
						 currentEntry->next->priority < newEntry->priority)
				 {
					 currentEntry = currentEntry->next;
				 }

				 //found, we add it here
				 newEntry->next = currentEntry->next;
				 currentEntry->next = newEntry;
			 }

	     pcQueue->qSize++;

	     return CQ_SUCESS;
}

/*
 * a helper function that remove and return an entry at bucket i
 */
QueueEntry* RemoveEntry(unsigned long index)
{

	   QueueEntry* firstEntry= pcQueue->qBuckets[index];

	   //so the first entry will be the next one, which might be null
	   pcQueue->qBuckets[index]=firstEntry->next;

	   //decrease the size
	   pcQueue->qSize=pcQueue->qSize-1;

	   //check for resizing the queue
	   if(pcQueue->qSize < pcQueue->qBucketCountLowerThreshold &&
			   pcQueue->qResizeEnabled==1) {

#ifdef DEBUG_INFO
	   		  printf("Resizing the queue!\n");

	   		  printf("  --------->> before resizing <<--------\n");
              PrintQueue();
#endif
	   		  ResizeQueue(pcQueue->qBucketCount/2);
#ifdef DEBUG_INFO
	   		  printf("  -------------->> after <<-------------\n");
	   		  PrintQueue();
#endif
	   	  }

	   return  firstEntry;
}

double CalculateNewBucketWidth()
{
#define MAX_SAMPLES  25

	//define used variables
	int nsamples, templastbucket, i, j;

	float templastprio;
	double tempbuckettop, average, newaverage;

	QueueEntry* temp[MAX_SAMPLES];

	//default bucket size
	if (pcQueue->qSize < 2) return(1.0);

	//calculate the number of samples
	if (pcQueue->qSize <5){
		nsamples = pcQueue->qSize;
	}
	else{
		nsamples = 5 + (int)((float)pcQueue->qSize / 10);
	}

	if (nsamples > MAX_SAMPLES) nsamples = MAX_SAMPLES;

	//save the current queue settings
	templastbucket = pcQueue->qLastbucket;
	templastprio = pcQueue->qLastPriority;
	tempbuckettop = pcQueue->qBuckettopPriority;

	//disable resizing during this test
	pcQueue->qResizeEnabled=0;

	average = 0.0;

	//try the samples
	for (i = 0; i < nsamples; i++){
		RemoveTopEntry(&temp[i]);
		if (i > 0) average += temp[i]->priority - temp[i-1]->priority;
	}

	average = average / (float)(nsamples - 1);

	newaverage = 0.0; j = 0;

	Enqueue(temp[0]->priority,temp[0]->userData);
	for (i = 1; i < nsamples; i++){
		if ((temp[i]->priority - temp[i-1]->priority) < (average * 2.0)){
			newaverage += (temp[i]->priority - temp[i-1]->priority);
			j++;
		}
		Enqueue(temp[i]->priority,temp[i]->userData);
	}

	//calculate the bucket width
	newaverage = (newaverage / (float)j) * 3.0;

	//restore queue info
	pcQueue->qLastbucket=templastbucket;
	pcQueue->qLastPriority=templastprio;
	pcQueue->qBuckettopPriority=tempbuckettop;

#ifdef DEBUG_INFO
	  printf("New bucket width=%f!\n", newaverage);
#endif

	  pcQueue->qResizeEnabled=1;

	return(newaverage);
}

int ResizeQueue(int newSize)
{
	    double qbWidth;
	    unsigned long i;
	    unsigned long qOldBucketCount;

	    //if resize is not enabled we return
	    if(pcQueue->qResizeEnabled==0)  return CQ_SUCESS;

	    //Calculate the bucket width
	    qbWidth=CalculateNewBucketWidth();

	    //save the old bucket count for the future processing
	    qOldBucketCount=pcQueue->qBucketCount;
	    QueueEntry** qOldBuckets;
		qOldBuckets=pcQueue->qBuckets;
    
	    LocalInit(newSize,qbWidth,pcQueue->qLastPriority);
    
	    //TODO: improve this part
	    //pcQueue->bWidth=qOldSize/(double) pcQueue->qBucketCount;
	    pcQueue->bWidth=qbWidth;

	    //copy the buckets from the old to the new calander
	    for(i=0;i<qOldBucketCount;i++){

	    	 QueueEntry* currentEntry=qOldBuckets[i];

	    	 while(currentEntry!=NULL){
	    		 Enqueue(currentEntry->priority,currentEntry->userData);
	    		 currentEntry=currentEntry->next;
	    	 }
	    }
    
	    //now we can safely free the space allocated for the old buckets
	    free(qOldBuckets);

	    return CQ_SUCESS;
}

void PrintQueue()
{
	unsigned long i;

	    printf("============ ++[Status Quo]++ ===========\n");
	    printf("Queue Size:\t%ld\n",pcQueue->qSize);
	    printf("Number of Buckets:\t%ld\n",pcQueue->qBucketCount);
	    printf("Bucket Width:\t%.3f\n",pcQueue->bWidth);

	    printf("The following is the contents of non-empty buckets:\n");
    
//        printf("newsize %ld\n", pcQueue->qBucketCount);
	      for(i=0;i<pcQueue->qBucketCount;i++){

	    	  //head of the list
	    	  QueueEntry* currentEntry=pcQueue->qBuckets[i];

	    	  if(currentEntry!=NULL)
	    		  printf("Bucket %ld contents: ",i);

	    	  while(currentEntry!=NULL){

	    		  printf("\t%0.2f",currentEntry->priority);

	    		  currentEntry=currentEntry->next;

	    	  }

	    	  if(pcQueue->qBuckets[i]!=NULL)  printf("\n");

	      }
}
