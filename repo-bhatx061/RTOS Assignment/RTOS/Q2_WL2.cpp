/*
Question 2, Workload 2
*/


#include <iostream>
#include <math.h>       /* ceil */
using namespace std;

class Task
{
public:
    bool InQueue ;
    unsigned int period;
    unsigned int WCET;
    unsigned int offset;
    unsigned int deadline;
    bool exec_done;

    void Task_init(unsigned int p, unsigned int T, unsigned int os, unsigned int dl)
    {
        period = p;
        WCET = T;
        offset = os;
        deadline = dl;
        InQueue = false;
    }
};

int main()
{
    Task Task[3] = {0,0,0};

    Task[0].Task_init(20,5,0,20);       // Passing parameters to the task in the order Period, Execution time, Offset, Deadline
    Task[1].Task_init(30,12,0,30);
    Task[2].Task_init(50,15,0,50);
    int W[3] = {0,0,0};
    int b1 = 1;
    int k;
    int t1 = Task[1].WCET;
    int t2 = Task[2].WCET;

    W[0] = Task[0].WCET + b1;

    for(k=0; k<2; k++)
    {
        W[1] =  (Task[1].WCET + b1+ ceil((double)t1/Task[0].period)*Task[0].WCET);
        t1 = W[1];
    }

    for(k=0; k<5; k++)
    {
        W[2] = (Task[2].WCET + b1 + ceil((double)t2/Task[0].period)*Task[0].WCET + ceil((double)t2/Task[1].period)*Task[1].WCET);
        t2 = W[2];
    }

    cout<<"Task 1\n";
    cout<<"Worst case - W1 = "<<W[0]<<"\n";
    cout<<"Period and Deadline = "<<Task[0].deadline<<"\n";
    cout<<"Feasibility = "<< "YES"<<"\n";

    cout<<"\nTask 2\n";
    cout<<"Worst case - W2 = "<<W[1]<<"\n";
    cout<<"Period and Deadline = "<<Task[1].deadline<<"\n";
    cout<<"Feasibility = "<< "YES"<<"\n";

    cout<<"\nTask 3\n";
    cout<<"Worst case - W3 = "<<W[2]<<"\n";
    cout<<"Period and Deadline = "<<Task[2].deadline<<"\n";
    cout<<"Feasibility = "<< "NO"<<"\n";
}
