#include<iostream>
#include"function1.h"
using namespace std;

int Factorial_val(int n)
{
	if(n==1)
		return 1;
	else
		return n * Factorial_val(n-1);
}