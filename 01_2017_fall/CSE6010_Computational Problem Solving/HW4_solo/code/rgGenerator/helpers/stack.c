/*
 ============================================================================
 Name        : stack.c
 Author      : Soo Hyeon Kim
 Version     : 10/21/17
 Copyright   : © 2017 Soo Hyeon Kim. All rights reserved.
 Student name: Soo Hyeon Kim
 Username	 : skim3026 (#:903230437)
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>

#include "stack.h"

//#include "ganalyser.h"




Stack* CreateStack(){

	Stack* s=(Stack*)malloc(sizeof(Stack));

	s->sSize=0;

	s->topNode=NULL;

	return s;
}

void StackPush(Stack*S, UserDataPtr elm)
{
	 if(S==NULL)  return;

	 //allocate memory
	 stackNode* newNode=(stackNode*)malloc(sizeof(stackNode));
	 newNode->pcData=elm;

	// printf("pushed %lu\n",((gNode*)(elm))->nodeID);

	 stackNode* tempNode=S->topNode;

	 newNode->next=tempNode;

	 S->topNode=newNode;

	 //increase the stack size
	 S->sSize++;
}

UserDataPtr StackPop(Stack*S)
{
	  if(S==NULL || S->topNode==NULL){

		  return NULL;
	  }

	  stackNode* oldTop;

	  oldTop=S->topNode;

	  S->topNode=S->topNode->next;

	  S->sSize--;

	  UserDataPtr ud=oldTop->pcData;

	 //printf("=======poped %lu\n",((gNode*)(ud))->nodeID);

	  //free the allocated memory
	  free(oldTop);

	  return ud;
}

unsigned int Size(Stack*S){

   if(S==NULL) return 0;
	return S->sSize;
}

int IsStackEmpty(Stack*S){
	return (S->sSize==0);
}
