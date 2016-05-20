#ifndef PILOTOS_SCHEDULING_POLICY
#define PILOTOS_SCHEDULING_POLICY

#include "process.h"

struct pcb* schedule_lastUsed ( struct pcb* p1, struct pcb* p2 );
void schedule_lastUsed_winning ( struct pcb* p1 );  

#endif
