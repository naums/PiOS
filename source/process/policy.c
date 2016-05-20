#include "policy.h"

struct pcb* schedule_lastUsed ( struct pcb* p1, struct pcb* p2 )
{
    if (p1->lastUse > p2->lastUse)
        return p2;
    else
        return p1;
}

void schedule_lastUsed_winning ( struct pcb* p1 )
{
    p1->lastUse++;
}   
