#include <iostream>
#include "list.h"
using namespace std;

int main(int argc, char *argv[])
{
	list l1;
	l1.AddNode(5);
	l1.AddNode(15);
	l1.AddNode(25);

	cout<<"Added - 5, 15, 25 "<<endl;
	l1.PrintList();

	l1.DelNode(15);

	cout<<"Deleted 15 : "<<endl;
	l1.PrintList();	

	l1.AddNode(25);
	l1.AddNode(35);
	l1.AddNode(45);
	l1.AddNode(55);
	l1.AddNode(65);
	l1.AddNode(75);

	cout<<"Added 25, 35, 45, 55, 65, 75 "<<endl;
	l1.PrintList();	
	
	l1.DelNode(45);
	l1.DelNode(75);

	cout<<"Deleted 45, 75 : "<<endl;
	l1.PrintList();	

	return 0;
}