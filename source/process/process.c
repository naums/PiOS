#include "process.h"

void* findNewStack () 
{
    static void* stack = (void*) 0x8000;
    if (stack <= (void*) 0x1000)
        stack = (void*) 0x160000;
    stack -= 0x1000;
    return stack;
}

int createProcess ( void* pc )
{
    if (numProcesses>=31) 
        return -1;
    struct pcb* p = &processes[numProcesses];
    
    p->lastUse = 0;
    p->deadline = 0;
    p->etime = 0;
    p->state = ACTIVE;
    p->priority = 0;
    p->arrival = 0;
    
    p->id = (numProcesses+1);
    processes[(numProcesses+1)].id=-1;
    p->sp = findNewStack ();
    numProcesses++;
    
    return p->id;
}

struct pcb* schedule ( struct pcb* (*func) ( struct pcb*, struct pcb* ), void (*winning) (struct pcb*) )
{
    struct pcb* tmp = processes;
    for (int i=0; i<numProcesses; i++)
    {
        tmp = func ( tmp, &processes[i] );
    }
    winning ( tmp );
    return tmp;
}
