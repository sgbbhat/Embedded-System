/*
Question 1, Workload 2
*/

#include <iostream>
#include <time.h>
#include <iomanip>

using namespace std;

class Timer
{
private:
    unsigned long start;
public:
    Timer()
    {
        start = ::time(NULL);
    }
    virtual ~Timer() { }

    unsigned long Lapse()
    {
        return ::time(NULL)-start;
    }
};

class Task
{
public:
    bool InQueue ;
    int period;
    int WCET;
    int offset;
    int deadline;
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

int main(void)
{
    int current_time;
    int state = 0;
    Timer t;
    Task Task[8];
    bool run = false;
    int entry_time = 0;
    int task_count[8] = {0,0,0,0,0,0,0,0};

    Task[0].Task_init(20,4,0,15);       // Passing parameters to the task in the order Period, Execution time, Offset, Deadline
    Task[1].Task_init(20,1,5,20);
    Task[2].Task_init(30,2,5,30);
    Task[3].Task_init(30,1,5,30);
    Task[4].Task_init(50,1,10,40);
    Task[5].Task_init(50,1,10,40);
    Task[6].Task_init(50,2,25,50);
    Task[7].Task_init(50,2,25,50);

    bool complete = false;
    bool InQue = false;
    bool dummy = false;

    const char separator    = ' ';
    const int nameWidth     = 6;

    cout << left << setw(nameWidth) << setfill(separator) << "Time";
    cout << right << setw(nameWidth) << setfill(separator) << setfill(separator) << setfill(separator) <<setfill(separator) <<"    JOB\n";

    while(complete == false)
    {
        current_time = t.Lapse() ;
        Task[0].InQueue = (((current_time % Task[0].period) > Task[0].offset) && (!Task[0].exec_done));
        Task[1].InQueue = (((current_time % Task[1].period) > Task[1].offset) && (!Task[1].exec_done));
        Task[2].InQueue = (((current_time % Task[2].period) > Task[2].offset) && (!Task[2].exec_done));
        Task[3].InQueue = (((current_time % Task[3].period) > Task[3].offset) && (!Task[3].exec_done));
        Task[4].InQueue = (((current_time % Task[4].period) > Task[4].offset) && (!Task[4].exec_done));
        Task[5].InQueue = (((current_time % Task[5].period) > Task[5].offset) && (!Task[5].exec_done));
        Task[6].InQueue = (((current_time % Task[6].period) > Task[6].offset) && (!Task[6].exec_done));
        Task[7].InQueue = (((current_time % Task[7].period) > Task[7].offset) && (!Task[7].exec_done));

        if((current_time%Task[0].period) == 0)
        {
            Task[0].exec_done = false;
        }
        if((current_time%Task[1].period) == 0)
        {
            Task[1].exec_done = false;
        }
        if((current_time%Task[2].period) == 0)
        {
            Task[2].exec_done = false;
        }
        if((current_time%Task[3].period) == 0)
        {
            Task[3].exec_done = false;
        }
        if((current_time%Task[4].period) == 0)
        {
            Task[4].exec_done = false;
        }
        if((current_time%Task[5].period) == 0)
        {
            Task[5].exec_done = false;
        }
        if((current_time%Task[6].period) == 0)
        {
            Task[6].exec_done = false;
        }
        if((current_time%Task[7].period) == 0)
        {
            Task[7].exec_done = false;
        }

        if(state == 0)
        {
            InQue = Task[0].InQueue || Task[1].InQueue || Task[2].InQueue || Task[3].InQueue || Task[4].InQueue || Task[5].InQueue || Task[6].InQueue || Task[7].InQueue;
            if(InQue)
                {
                if(Task[0].InQueue)
                    {
                        state = 1;
                    }
                else if(Task[1].InQueue)
                    {
                        state = 2;
                    }
                else if(Task[2].InQueue)
                    {
                        state = 3;
                    }
                else if(Task[3].InQueue)
                    {
                        state = 4;
                    }
                else if(Task[4].InQueue)
                    {
                        state = 5;
                    }
                else if(Task[5].InQueue)
                    {
                        state = 6;
                    }
                else if(Task[6].InQueue)
                    {
                        state = 7;
                    }
                else if(Task[7].InQueue)
                    {
                        state = 8;
                    }
                }

                if(!InQue)
                {
                    state = 9;
                }
        }

        else if(state == 1)
            {
            if(!run)
                {
                    task_count[0] +=1;
                    entry_time = current_time;
                    cout<<"\n";
                    cout << left << setw(nameWidth) << setfill(separator) <<current_time-1 ;
                    cout << right << setw(nameWidth) << setfill(separator) << "J1"<<task_count[0];
                    run = true;
                }
            if(current_time >= (entry_time + Task[0].WCET))
                {
                    Task[0].exec_done = true;
                    run = false;
                    state = 0;
                }
            }
        else if(state == 2)
            {
            if(!run)
                {
                    task_count[1] +=1;
                    entry_time = current_time;
                    cout<<"\n";
                    cout << left << setw(nameWidth) << setfill(separator) <<current_time-1 ;
                    cout << right << setw(nameWidth) << setfill(separator) << "J2"<<task_count[1];
                    run = true;
                }

            if(current_time >= (entry_time + Task[1].WCET))
                {
                    Task[1].exec_done = true;
                    run = false;
                    dummy = false;
                    state = 0;
                }
        }
        else if(state == 3)
        {
            if(!run)
                {
                    task_count[2] +=1;
                    entry_time = current_time;
                    cout<<"\n";
                    cout << left << setw(nameWidth) << setfill(separator) <<current_time-1 ;
                    cout << right << setw(nameWidth) << setfill(separator) << "J3"<<task_count[2];
                    run = true;
                }
            if(current_time >= (entry_time + Task[2].WCET))
                {
                    Task[2].exec_done = true;
                    run = false;
                    state = 0;
                }
        }
        else if(state == 4)
        {
            if(!run)
                {
                    task_count[3] +=1;
                    entry_time = current_time;
                    cout<<"\n";
                    cout << left << setw(nameWidth) << setfill(separator) <<current_time-1 ;
                    cout << right << setw(nameWidth) << setfill(separator) << "J4"<<task_count[3];
                    run = true;
                }
            if(current_time >= (entry_time + Task[3].WCET))
                {
                    Task[3].exec_done = true;
                    run = false;
                    state = 0;
                }
        }
        else if(state == 5)
        {
            if(!run)
                {
                    task_count[4] +=1;
                    entry_time = current_time;
                    cout<<"\n";
                    cout << left << setw(nameWidth) << setfill(separator) <<current_time-1 ;
                    cout << right << setw(nameWidth) << setfill(separator) << "J5"<<task_count[4];
                    run = true;
                }
            if(current_time >= (entry_time + Task[4].WCET))
                {
                    Task[4].exec_done = true;
                    run = false;
                    state = 0;
                }
        }
        else if(state == 6)
        {
            if(!run)
                {
                    task_count[5] += 1;
                    entry_time = current_time;
                    cout<<"\n";
                    cout << left << setw(nameWidth) << setfill(separator) <<current_time-1 ;
                    cout << right << setw(nameWidth) << setfill(separator) << "J6"<<task_count[5];
                    run = true;
                }
            if(current_time >= (entry_time + Task[5].WCET))
                {
                    Task[5].exec_done = true;
                    run = false;
                    state = 0;
                }
        }
        else if(state == 7)
        {
            if(!run)
                {
                    task_count[6] +=1;
                    entry_time = current_time;
                    cout<<"\n";
                    cout << left << setw(nameWidth) << setfill(separator) <<current_time-1 ;
                    cout << right << setw(nameWidth) << setfill(separator) << "J7"<<task_count[6];
                    run = true;
                }
            if(current_time >= (entry_time + Task[6].WCET))
                {
                    Task[6].exec_done = true;
                    run = false;
                    state = 0;
                }
        }

       else if(state == 8)
        {
            if(!run)
                {
                    task_count[7] +=1;
                    entry_time = current_time;
                    cout<<"\n";
                    cout << left << setw(nameWidth) << setfill(separator) <<current_time-1 ;
                    cout << right << setw(nameWidth) << setfill(separator) << "J8"<<task_count[7];
                    run = true;
                }
            if(current_time >= (entry_time + Task[7].WCET))
                {
                    Task[7].exec_done = true;
                    run = false;
                    state = 0;
                }
        }
        else if(state == 9)
        {
            InQue = Task[0].InQueue || Task[1].InQueue || Task[2].InQueue || Task[3].InQueue || Task[4].InQueue || Task[5].InQueue || Task[6].InQueue || Task[7].InQueue ;
            if(InQue)
            {
                if((current_time) < 2)
                {
                    cout<<"\n";cout << left << setw(nameWidth) << setfill(separator) <<current_time-1 ;
                    cout << right << setw(nameWidth) << setfill(separator) << "    Idle";
                }
                else
                {
                    cout<<"\n";cout << left << setw(nameWidth) << setfill(separator) <<current_time-2 ;
                    cout << right << setw(nameWidth) << setfill(separator) << "    Idle";
                }
                state = 0;
            }
            else
            {
                if((current_time == 13)||(current_time == 47) || (current_time == 71))
                {
                    if(!dummy)
                    {
                        cout<<"\n";cout << left << setw(nameWidth) << setfill(separator) <<current_time-1 ;
                        cout << right << setw(nameWidth) << setfill(separator) << "    Idle";
                        dummy = true;
                    }

                }
                state = 9;
            }

        }

        if(current_time == 300)
        {
            complete = true;
            cout<<"\nOne Hyper-period complete\n";
        }
    }

    if(complete)
    {
        cout<<"Task 1 : Expected no.of exec = 15, no.of times exec = "<<task_count[0]<<" No deadlines missed\n";
        cout<<"Task 2 : Expected no.of exec = 15, no.of times exec = "<<task_count[1]<<" No deadlines missed\n";
        cout<<"Task 3 : Expected no.of exec = 10, no.of times exec = "<<task_count[2]<<" No deadlines missed\n";
        cout<<"Task 4 : Expected no.of exec = 10, no.of times exec = "<<task_count[3]<<" No deadlines missed\n";
        cout<<"Task 5 : Expected no.of exec = 6, no.of times exec = "<<task_count[4]<<" No deadlines missed\n";
        cout<<"Task 6 : Expected no.of exec = 6, no.of times exec = "<<task_count[5]<<" No deadlines missed\n";
        cout<<"Task 7 : Expected no.of exec = 6, no.of times exec = "<<task_count[6]<<" No deadlines missed\n";
        cout<<"Task 8 : Expected no.of exec = 6, no.of times exec = "<<task_count[7]<<" No deadlines missed\n";
    }
    return 0;

}
