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
    
    numProcesses++;
    p->id = numProcesses;
    processes[numProcesses].id=-1;
    p->sp = findNewStack ();
    
    return p->id;
}

struct pcb* schedule ( struct pcb* (*func) ( struct pcb*, struct pcb* ), void (*winning) (struct pcb*) )
{
    struct pcb* tmp = processes;
    struct pcb* t2 = processes;
    for (int i=0; i<numProcesses; i++)
    {
        if (t2->state != BLOCKED)
        {
            tmp = func ( tmp, &processes[i] );
        }
        t2++;
    }
    if (tmp->id > 0)
    {
        winning ( tmp );
        return tmp;
    }
    else
    {
        return NULL;
    }
}
