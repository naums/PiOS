#ifndef PILOTOS_SCHEDULING_POLICY
#define PILOTOS_SCHEDULING_POLICY

/// describes the state of a process
enum process_state
{
    RUNNING,        ///< the process is running (using the CPU)
    ACTIVE,         ///< the process could run, but does not use the CPU at the moment
    BLOCKED         ///< the process is blocked, i.e. waiting for something
};

/// process control block, contains information about the process
struct pcb
{
    int id;                  ///< the id of the process
    unsigned int arrival;    ///< arrival-time of the process (for realtime-use)
    unsigned int deadline;   ///< the deadline of the process (for realtime-use)
    unsigned int etime;      ///< the execution time of the process (for realtime-use)
    int priority;            ///< priority of the task 
    unsigned int lastUse;    ///< the time of last use (maybe used as counter)
    int state;               ///< state of the process (determines schedulability)
    void* sp;                ///< saved stack pointer after saving the context
};
extern struct pcb* running;
extern unsigned int numProcesses;

struct pcb* schedule_lastUsed ( struct pcb* p1, struct pcb* p2 );
void schedule_lastUsed_winning ( struct pcb* p1 );  

#endif
