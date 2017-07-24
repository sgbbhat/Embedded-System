#include <iostream>
#include "list.h"
using namespace std;

list::list()
{
	headPtr = NULL;
	tempPtr = NULL;
	currPtr = NULL;
}

void list::AddNode(int value)
{
	node * n = new node;
	n->next = NULL;
	n->key = value;

	if(headPtr != NULL)
	{
		currPtr = headPtr;
		while(currPtr->next != NULL)
		{
			currPtr = currPtr->next;
		}
		currPtr->next = n;
	}
	else
	{
		headPtr = n;			// In new node created is the first node
	}
}

void list::DelNode(int value)
{
	node * delptr = new node;

	if(headPtr == NULL)
	{
		cout<<"List is empty"<<endl;
	}
	else
	{
		currPtr = headPtr ;
		tempPtr = headPtr ;

		while(currPtr != NULL && currPtr->key != value)
		{
			tempPtr = currPtr ;
			currPtr = currPtr->next ;
		}

		if(currPtr == NULL)
		{
			cout<<"Key not found in the list"<<endl;
		}
		else
		{
			delptr = currPtr ;
			currPtr = currPtr->next;
			tempPtr->next = currPtr;
		}

		delete delptr;
	}
}

void list::PrintList()
{
	currPtr = headPtr;
	while(currPtr != NULL)
	{
		cout<<currPtr->key<<endl;
		currPtr = currPtr->next;
	}
}