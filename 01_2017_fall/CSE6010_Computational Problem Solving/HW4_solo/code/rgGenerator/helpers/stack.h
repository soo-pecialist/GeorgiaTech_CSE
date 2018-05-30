/*
 ============================================================================
 Name        : stack.h
 Author      : Soo Hyeon Kim
 Version     : 10/21/17
 Copyright   : © 2017 Soo Hyeon Kim. All rights reserved.
 Student name: Soo Hyeon Kim
 Username	 : skim3026 (#:903230437)
 ============================================================================
 */

typedef void* UserDataPtr;

//a stack node
struct sNode{

	 //pointer to the user data
	 UserDataPtr pcData;

	 //pointer to the next node
	 struct sNode* next;
};

//stack node
typedef struct sNode stackNode;

struct Container{

	//stack size
	unsigned int  sSize;

	//the top of the stack
	stackNode* topNode;
};

typedef struct Container Stack;

//create a stack
Stack* CreateStack();

//push an element in the stack
void StackPush(Stack*S, UserDataPtr elm);

//pop an element from the stack
UserDataPtr StackPop(Stack*S);

int IsStackEmpty(Stack*S);

//return the stack size
unsigned int Size(Stack*S);









