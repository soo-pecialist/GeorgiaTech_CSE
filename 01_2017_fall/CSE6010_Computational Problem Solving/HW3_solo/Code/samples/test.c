//
//  test.c
//  CSE-6010 Assignment3
//
//  Created by Soo Hyeon Kim on 10/9/17.
//  Copyright © 2017 Soo Hyeon Kim. All rights reserved.
//
//  Student name: Soo Hyeon Kim
//  Student username: skim3026 (#:903230437)


#include <stdio.h>
#include <stdlib.h>

#include "../calendarqueue/calendarqueue.h"

//TODO:uncomment this if you would like to show runtime messages
//or use it into your client program. You can also give it to your compiler
//#define DEBUG_INFO


//this is a test file for the simulation engine and the
//calendar queue.

struct SimpleUserData{

	int ID;
	double priority;
};
int main()
{
	if(InitQueue(1)!=CQ_SUCESS){ // dynamic calendar queue

		printf ("Cannot initialize the queue\n");

		return 0;
	}

	printf ("The queue has been initialized correctly\n");

	//in this example the user data is not important, so we let it to be NULL
	 //or we can give it a simple structure

	struct SimpleUserData data1;
	struct SimpleUserData data2;
	struct SimpleUserData data3;
	struct SimpleUserData data4;
	struct SimpleUserData data5;
    struct SimpleUserData data6;
    struct SimpleUserData data7;
    struct SimpleUserData data8;
    struct SimpleUserData data9;
    struct SimpleUserData data10;
    struct SimpleUserData data11;
    struct SimpleUserData data12;
    struct SimpleUserData data13;
    struct SimpleUserData data14;
    struct SimpleUserData data15;
    struct SimpleUserData data16;
    struct SimpleUserData data17;
    
    
	data1.ID=1;
	data1.priority=0.2;
	Enqueue(0.2, &data1);

	data2.ID=2;
	data2.priority=0.1;
	Enqueue(0.1, &data2);

	data3.ID=3;
	data3.priority=1;
	Enqueue(1, &data3);

	data4.ID=4;
	data4.priority=0.4;
	Enqueue(0.4, &data4);

	//here the queue should trigger resize
	data5.ID=5;
	data5.priority=4;
	Enqueue(4, &data5);
    
    data6.ID=6;
    data6.priority=4.2;
    Enqueue(4.2, &data6);

    data7.ID=7;
    data7.priority=3.8;
    Enqueue(3.8, &data7);
    
    data8.ID=8;
    data8.priority=5.1;
    Enqueue(data8.priority, &data8);
    
    //here the queue should trigger resize
    data9.ID=9;
    data9.priority=4.9;
    Enqueue(data9.priority, &data9);
    
    data10.ID=10;
    data10.priority=5.3;
    Enqueue(data10.priority, &data10);
    
    data11.ID=11;
    data11.priority=5.7;
    Enqueue(data11.priority, &data11);
    
    data12.ID=12;
    data12.priority=7.1;
    Enqueue(data12.priority, &data12);
    
    data13.ID=13;
    data13.priority=5.9;
    Enqueue(data13.priority, &data13);
    
    data14.ID=14;
    data14.priority=6.5;
    Enqueue(data14.priority, &data14);
    
    data15.ID=15;
    data15.priority=7.3;
    Enqueue(data15.priority, &data15);
    
    data16.ID=16;
    data16.priority=6.6;
    Enqueue(data16.priority, &data16);
    
    //here the queue should trigger resize
    data17.ID=17;
    data17.priority=6.4;
    Enqueue(data17.priority, &data17);
    

    //test dequeue
    struct SimpleUserData* dataOut;
    dataOut=(struct SimpleUserData*)malloc(sizeof(struct SimpleUserData));
    
    printf("\ntesting dequeue...\n");
    for (int i=0; i<12; i++){
        Dequeue((UserDataPtr*)&dataOut);
#ifdef DEBUG_INFO
        printf(">>The entry with ID %d and priority %f has been dequeued\n",dataOut->ID,dataOut->priority);
#endif
    }
    
    printf("\nprinting the queue after dequeuing:\n");
    PrintQueue();
    
    //free the used memory by the queue code
    Finalize();
    
    printf ("finished!\n");
    
	return 0;

}
