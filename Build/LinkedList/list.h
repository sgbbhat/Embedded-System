#include <iostream>
using namespace std;

struct node
{
	int key;
	node * next;
};

class list
{
private:
	node * headPtr;
	node * tempPtr;
	node * currPtr;

public:
	list();
	void AddNode(int value);
	void DelNode(int value);
	void PrintList();
};

