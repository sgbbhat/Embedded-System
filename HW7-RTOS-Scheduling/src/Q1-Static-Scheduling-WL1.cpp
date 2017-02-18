/*
Question 1, Workload 1
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
    int current_time = 0;
    int state = 0;
    Timer t;
    Task Task[8];
    bool run = false;
    int entry_time = 0;
    int task_count[8] = {0,0,0,0,0,0,0,0};
    Task[0].Task_init(10,2,0,10);       // Passing parameters to the task in the order Period, Execution time, Offset, Deadline
    Task[1].Task_init(10,2,0,10);
    Task[2].Task_init(20,1,5,20);
    Task[3].Task_init(20,2,5,20);
    Task[4].Task_init(40,2,5,30);
    Task[5].Task_init(50,2,5,30);
    Task[6].Task_init(80,2,10,60);
    Task[7].Task_init(80,2,10,60);

    bool complete = false;
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

        if(Task[0].InQueue)
            {
                state = 1;
            }

        if(state == 1)
            {
            if(!run)
                {
                    task_count[0] +=1;
                    entry_time = current_time;
                    cout<<"\n";
                    cout << left << setw(nameWidth) << setfill(separator) <<current_time-1 ;
                    cout << right << setw(nameWidth) << setfill(separator) << "J1"<<task_count[0];
                    run = true;
                    Task[1].exec_done = false;
                }
            if(current_time >= (entry_time + Task[0].WCET))
                {
                    Task[0].exec_done = true;
                    run = false;
                    if(Task[1].InQueue == true)
                        {
                            state = 2;
                            Task[1].exec_done = false;
                        }
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
                    if(Task[2].InQueue == true)
                        {
                            state = 3;
                        }
                    else if(Task[6].InQueue == true)
                        {
                            state = 6;
                        }
		   else if(current_time == 55)
                        {
                            state = 6;
                        }	
                    else
                        {
                            state = 9;
                        }
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
                    if(Task[3].InQueue == true)
                        {
                            state = 4;
                        }
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
                    if(Task[4].InQueue == true)
                        {
                            state = 5;
                        }
                    else
                    {
                        state = 9;
                    }
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
                    Task[0].exec_done = false;
                    Task[1].exec_done = false;
                    if(current_time > 50)
                    {
                        Task[5].exec_done = false;
                        state = 6;
                    }
                    else
                    {
                        state = 1;
                    }

                }
        }
        else if(state == 6)
        {
            if(!run)
                {
                    task_count[5] +=1;
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
                    if(Task[6].InQueue == true)
                        {
                            state = 7;
                        }
			else
			{
			    state = 9;
			}
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
                    cout << right << setw(nameWidth) << setfill(separator) << "J7"<<task_count[6]<<"\n";
                    run = true;
                }
            if(current_time >= (entry_time + Task[6].WCET))
                {
                    Task[6].exec_done = true;
                    run = false;
                    if(Task[7].InQueue == true)
                        {
                            state = 8;
                        }
                }
        }

       else if(state == 8)
        {
            if(!run)
                {
                    task_count[7] +=1;
                    entry_time = current_time;
                    cout << left << setw(nameWidth) << setfill(separator) <<current_time-1 ;
                    cout << right << setw(nameWidth) << setfill(separator) << "J8"<<task_count[7];
                    run = true;
                }
            if(current_time >= (entry_time + Task[7].WCET))
                {
                    Task[7].exec_done = true;
                    run = false;
                    Task[0].exec_done = false;
                    Task[1].exec_done = false;
                    Task[2].exec_done = false;
                    Task[3].exec_done = false;
                    state = 1;
                }
        }
        else if(state == 9)
        {
            if(!run)
                {
                    entry_time = current_time;
                    cout<<"\n";cout << left << setw(nameWidth) << setfill(separator) <<current_time-1 ;
                    cout << right << setw(nameWidth) << setfill(separator) << "    Idle";
                    run = true;
                }

            if(Task[2].InQueue == 1)
            {
                state = 3;
                run = false;
            }
            else if(Task[1].InQueue == 1)
            {
                run = false;
            }

            else
            {
                state = 9;
            }

            if((current_time % 10) == 1)
            {
                state = 1;

                if((current_time % 40) == 1)
                {
                    Task[2].exec_done = false;
                    Task[3].exec_done = false;
                    Task[4].exec_done = false;
                    Task[5].exec_done = false;
                }
                if((current_time % 20) == 1)
                {
                    Task[2].exec_done = false;
                    Task[3].exec_done = false;
                }
            }
        }
        if(current_time == 80)
        {
            complete = true;
            cout<<"\n\nOne Hyper-period complete\n";
        }
    }

    if(complete)
    {
        cout<<"Task 1 : Expected no.of exec = 8, no.of times exec = "<<task_count[0]<<" No deadlines missed\n";
        cout<<"Task 2 : Expected no.of exec = 8, no.of times exec = "<<task_count[1]<<" No deadlines missed\n";
        cout<<"Task 3 : Expected no.of exec = 4, no.of times exec = "<<task_count[2]<<" No deadlines missed\n";
        cout<<"Task 4 : Expected no.of exec = 4, no.of times exec = "<<task_count[3]<<" No deadlines missed\n";
        cout<<"Task 5 : Expected no.of exec = 2, no.of times exec = "<<task_count[4]<<" No deadlines missed\n";
        cout<<"Task 6 : Expected no.of exec = 2, no.of times exec = "<<task_count[5]<<" No deadlines missed\n";
        cout<<"Task 7 : Expected no.of exec = 1, no.of times exec = "<<task_count[6]<<" No deadlines missed\n";
        cout<<"Task 8 : Expected no.of exec = 1, no.of times exec = "<<task_count[7]<<" No deadlines missed\n";
    }
    return 0;

}
