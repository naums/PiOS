#include "process.h"

extern char* hex32 ( unsigned int );
extern void printString ( char*, unsigned int, unsigned int );
extern void* initProcessStack ( void*, void* );


void* findNewStack () 
{
    static void* stack = (void*) 0x7000;
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
    void* sp = findNewStack ();
        
    p->sp = initProcessStack (sp, pc);
    
    if (running == NULL)
        running = p;
    
    return p->id;
}

struct pcb* schedule ( struct pcb* (*func) ( struct pcb*, struct pcb* ), void (*winning) (struct pcb*) )
{    
    struct pcb* tmp = processes;
    struct pcb* t2;
    for (t2=processes; t2->id>=-1; t2++)
    {
        if (t2->state != BLOCKED)
        {
            tmp = func ( tmp, t2 );
        }
    }
    
    if (tmp->id >= 0)
    {
        winning ( tmp );
        return tmp;
    }
    else
    {
        return NULL;
    }
}
