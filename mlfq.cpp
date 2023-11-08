#include <bits/stdc++.h>
using namespace std;
class process
{
public:
    int id;
    int priority;
    int arrival_time;
    int burst_time;
    int completion_time;
    int turnaround_time;
    int wait_time;
    int remaining_time;
    int queue_entered_time;

    process(int id,int priority,int arrival_time,int burst_time)
    {
        this->id = id;
        this->priority = priority;
        this->arrival_time = arrival_time;
        this->burst_time = burst_time;
        completion_time = 0;
        turnaround_time = 0;
        wait_time = 0;
        remaining_time = burst_time;
    }

    void compute_turnaround_time()
    {
        turnaround_time = completion_time - arrival_time;
    }
    void compute_wait_time()
    {
        wait_time = turnaround_time - burst_time;
    }
    void compute_timing_parameters(int current_time)
    {
        completion_time = current_time;
        compute_turnaround_time();
        compute_wait_time();
    }

};
int GLOBAL_TIME = 0;
int THRESHOLD_TIME = 0;
int QUANT = 0;
float MEAN_TURNAROUND_TIME = 0;
double THROUGHPUT = 0;
int FIRST_ARRIVAL = 0;
int NUM_PROCESSES = 0;
int INF = 1000000;
bool EXIT_FLAG = false;
ofstream fout;
vector<process> PROCESS_POOL;
vector<process> Q1,Q2,Q3,Q4;
map<int,vector<process>*> PRIORITY_MAP;

void schedule_from_Q4();
void schedule_from_Q3();
void schedule_from_Q2();
void schedule_from_Q1();

void print_queues()
{
    for (int i=4;i>0;i--)
    {
        for (int j=0;j<(*PRIORITY_MAP[i]).size();j++)
        {
            cout<<(*PRIORITY_MAP[i])[j].id<<" ";
        }
        cout<<endl;
        cout<<"size of Q"<<i<<" is "<<(*PRIORITY_MAP[i]).size()<<endl;
    }
}

void make_priority_map()
{
    PRIORITY_MAP[1] = &Q1;
    PRIORITY_MAP[2] = &Q2;
    PRIORITY_MAP[3] = &Q3;
    PRIORITY_MAP[4] = &Q4;
}

bool compare_arrival_time(process &p1,process &p2)
{
    if (p1.arrival_time == p2.arrival_time)
    {
        if (p1.priority == p2.priority)
        {
            return (p1.id < p2.id);
        }
        else
        {
            return (p1.priority > p2.priority);
        }
    }
    return (p1.arrival_time < p2.arrival_time);
}

bool compare_burst_time(process &p1,process &p2)
{
    if (p1.burst_time == p2.burst_time)
    {
        if (p1.arrival_time == p2.arrival_time)
        {
            return (p1.id < p2.id);
        }
        else
        {
            return (p1.arrival_time < p2.arrival_time);
        }
    }
    else
    {
        return (p1.burst_time < p2.burst_time);
    }
}

void increase_priority()
{
    for (int i=1;i<4;i++)
    {
        for(int j=0;j<(*PRIORITY_MAP[i]).size();j++)
        {
            if (GLOBAL_TIME-(*PRIORITY_MAP[i])[j].queue_entered_time >= THRESHOLD_TIME)
            {
                process p = (*PRIORITY_MAP[i])[j];
                (*PRIORITY_MAP[i]).erase((*PRIORITY_MAP[i]).begin()+j);
                j--;
                p.queue_entered_time = (*PRIORITY_MAP[i])[j].queue_entered_time + THRESHOLD_TIME;
                (*PRIORITY_MAP[i+1]).push_back(p);
            }
        }
    }
}

void insert_new_processes()
{
    // sort(PROCESS_POOL.begin(),PROCESS_POOL.end(),compare_arrival_time);
    while(PROCESS_POOL.size()>0)
    {
        if(PROCESS_POOL[0].arrival_time <= GLOBAL_TIME)
        {
            // PROCESS_POOL[0].queue_entered_time = GLOBAL_TIME;
            PROCESS_POOL[0].queue_entered_time = PROCESS_POOL[0].arrival_time;
            (*PRIORITY_MAP[PROCESS_POOL[0].priority]).push_back(PROCESS_POOL[0]);
            PROCESS_POOL.erase(PROCESS_POOL.begin());
        }
        else
        break;
    }
    increase_priority();
}


int tt_next_arrival(int priority)
{
    int ttna = 100000;
    for (int i=0;i<PROCESS_POOL.size();i++)
    {
        if (PROCESS_POOL[i].priority > priority)
        {
            ttna = PROCESS_POOL[i].arrival_time - GLOBAL_TIME ;
            break;
        }
    }
    return ttna;
}
void check_if_end()
{
    if(PROCESS_POOL.size() == 0)
    {
        EXIT_FLAG = true;
    }
    else
    {
        //idle time
        int idle_time = PROCESS_POOL[0].arrival_time - GLOBAL_TIME;
        GLOBAL_TIME = PROCESS_POOL[0].arrival_time;
        cout<<"<idle>"<<GLOBAL_TIME;
        insert_new_processes();
    }
}

void print_stats()
{
    MEAN_TURNAROUND_TIME /= NUM_PROCESSES ;
    THROUGHPUT = double(NUM_PROCESSES) / double(GLOBAL_TIME-FIRST_ARRIVAL);
    fout << "Mean Turnaround time:  "<<MEAN_TURNAROUND_TIME<<" (ms); Throughput: "<<THROUGHPUT<<" processes/sec"<<endl;
    
}


void schedule_from_Q4()
{
    // cout<<"scheduling from Q4"<<endl;
    // cout<<"size of Q4 is "<<Q4.size()<<endl;
    int scheduled_time;
    int i = 0;
    while(1)
    {
        if (Q4.size() == 0)
        {
            break;
        }
        scheduled_time = min(QUANT,Q4[i].remaining_time);
        Q4[i].remaining_time = Q4[i].remaining_time - scheduled_time ;
        GLOBAL_TIME = GLOBAL_TIME + scheduled_time;
        cout<<"<p"<<Q4[i].id<<"-Q4>"<<GLOBAL_TIME;
        if (Q4[i].remaining_time == 0)
        {
            // process has completed
            Q4[i].compute_timing_parameters(GLOBAL_TIME);
            MEAN_TURNAROUND_TIME += Q4[i].turnaround_time;
            fout<<"ID: "<<Q4[i].id<<"; Orig. Level: "<<Q4[i].priority<<"; Final Level: 4; Comp. Time(ms): "<<Q4[i].completion_time<<" ;TAT (ms): "<<Q4[i].turnaround_time<<"\n";
            Q4.erase(Q4.begin()+i);
            i--;
        }
        insert_new_processes();
        i++;
        if (i==Q4.size())
        {
            i=0;
        }
    }
    if (Q4.size() == 0)
    {
        schedule_from_Q3();
    }
}

void schedule_from_Q3()
{
    // cout<<"scheduling from Q3"<<endl;
    // cout<<"size of Q3 is "<<Q3.size()<<endl;
    int scheduled_time;
    while(1)
    {
        sort(Q3.begin(),Q3.end(),compare_burst_time);
        if (Q3.size() == 0)
        {
            break;
        }
        scheduled_time = min(Q3[0].remaining_time,tt_next_arrival(3));
        Q3[0].remaining_time = Q3[0].remaining_time - scheduled_time;
        GLOBAL_TIME = GLOBAL_TIME + scheduled_time;
        cout<<"<p"<<Q3[0].id<<"-Q3>"<<GLOBAL_TIME;
        if (Q3[0].remaining_time == 0)
        {
            // process has completed
            Q3[0].compute_timing_parameters(GLOBAL_TIME);
            MEAN_TURNAROUND_TIME += Q3[0].turnaround_time;
            fout<<"ID: "<<Q3[0].id<<"; Orig. Level: "<<Q3[0].priority<<"; Final Level: 3; Comp. Time(ms): "<<Q3[0].completion_time<<" ;TAT (ms): "<<Q3[0].turnaround_time<<"\n";
            Q3.erase(Q3.begin());
        }
        insert_new_processes();
        schedule_from_Q4();
    }
    if (Q3.size() == 0)
    {
        schedule_from_Q2();
    }
}

void schedule_from_Q2()
{
    // cout<<"scheduling from Q2"<<endl;
    // cout<<"size of Q2 is "<<Q2.size()<<endl;
    int scheduled_time;
    while(1)
    {
        sort(Q2.begin(),Q2.end(),compare_burst_time);
        if (Q2.size() == 0)
        {
            break;
        }
        scheduled_time = min(Q2[0].remaining_time,tt_next_arrival(2));
        Q2[0].remaining_time = Q2[0].remaining_time - scheduled_time;
        GLOBAL_TIME = GLOBAL_TIME + scheduled_time;
        cout<<"<p"<<Q2[0].id<<"-Q2>"<<GLOBAL_TIME;
        if (Q2[0].remaining_time == 0)
        {
            // process has completed
            Q2[0].compute_timing_parameters(GLOBAL_TIME);
            MEAN_TURNAROUND_TIME += Q2[0].turnaround_time;
            fout<<"ID: "<<Q2[0].id<<"; Orig. Level: "<<Q2[0].priority<<"; Final Level: 2; Comp. Time(ms): "<<Q2[0].completion_time<<" ;TAT (ms): "<<Q2[0].turnaround_time<<"\n";
            Q2.erase(Q2.begin());
        }
        insert_new_processes();
        schedule_from_Q4();
    }
    if (Q2.size() == 0)
    {
        schedule_from_Q1();
    }
}

void schedule_from_Q1()
{
    // cout<<"scheduling from Q1"<<endl;
    // cout<<"size of Q1 is "<<Q1.size()<<endl;
    int scheduled_time; 
    while(1)
    {
        if (Q1.size() == 0)
        {
            break;
        }
        scheduled_time = min(Q1[0].remaining_time,tt_next_arrival(1));
        Q1[0].remaining_time = Q1[0].remaining_time - scheduled_time ;
        GLOBAL_TIME = GLOBAL_TIME + scheduled_time;
        cout<<"<p"<<Q1[0].id<<"-Q1>"<<GLOBAL_TIME;
        if (Q1[0].remaining_time == 0)
        {
            // process has completed
            Q1[0].compute_timing_parameters(GLOBAL_TIME);
            MEAN_TURNAROUND_TIME += Q1[0].turnaround_time;
            fout<<"ID: "<<Q1[0].id<<"; Orig. Level: "<<Q1[0].priority<<"; Final Level: 1; Comp. Time(ms): "<<Q1[0].completion_time<<" ;TAT (ms): "<<Q1[0].turnaround_time<<"\n";
            Q1.erase(Q1.begin());
        }
        insert_new_processes();
        schedule_from_Q4();
    }
    if (Q1.size() == 0)
    {
        check_if_end();
        if (EXIT_FLAG == true) return;
        schedule_from_Q4();
    }
}


int main(int argc, char* argv[])
{
    if (argc < 4)
    {
        cout << "Command line inputs given incorrectly" << endl;
        exit(0);
    }
    QUANT = atoi(argv[1]);
    THRESHOLD_TIME = atoi(argv[2]);
    char *inputfile = argv[3];
    char *outputfile = argv[4];

    ifstream fin;
    string line;
    fin.open(inputfile);
    while (getline(fin, line))
    {
        int id, priority, arrival_time, burst_time;
        char *inp = new char[line.length() + 1];
        strcpy(inp, line.c_str());
        sscanf(inp, "%d %d %d %d", &id, &priority, &arrival_time, &burst_time);
        process p(id,priority,arrival_time,burst_time);
        PROCESS_POOL.push_back(p);
    }
    fin.close();

    sort(PROCESS_POOL.begin(),PROCESS_POOL.end(),compare_arrival_time);
    NUM_PROCESSES = PROCESS_POOL.size();
    GLOBAL_TIME = FIRST_ARRIVAL = PROCESS_POOL[0].arrival_time;
    cout<<GLOBAL_TIME;
    make_priority_map();
    fout.open(outputfile);
    while(1)
    {
        insert_new_processes();
        schedule_from_Q4();
        if (EXIT_FLAG == true) break;
    }
    cout<<endl;
    print_stats();
    fout.close();
    return 0;
}