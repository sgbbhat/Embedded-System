/*
Question 2, Workload 4
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

    Task[0].Task_init(25,8,0,25);       // Passing parameters to the task in the order Period, Execution time, Offset, Deadline
    Task[1].Task_init(50,13,0,50);
    Task[2].Task_init(100,40,0,100);
    int W[3] = {0,0,0};
    int k;
    int t1 = Task[1].WCET;
    int t2 = Task[2].WCET;

    int b2np = 2;
    int b3np = 2;

    W[0] = Task[0].WCET;

    for(k=0; k<2; k++)
    {
        W[1] = (Task[1].WCET +  2*b2np + ceil((double)t1/Task[0].period)*Task[0].WCET);
        t1 = W[1];
    }

    for(k=0; k<7; k++)
    {
        W[2] = (Task[2].WCET + 2*b3np + ceil((double)t2/Task[0].period)*Task[0].WCET + ceil((double)t2/Task[1].period)*Task[1].WCET);
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
