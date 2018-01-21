#include "pitypes.h"
#include "piconfig.h"
#include "process.h"

struct pios_pcb_t pios_proc[PIOS_MAX_PROCESSES];
pios_pid_t pios_pid_next;
pios_pid_t pios_running;

void pios_process_init()
{
    for ( int i = 0; i < PIOS_MAX_PROCESSES; i++ )
    {
        pios_proc[i].pid = -1;
        pios_proc[i].sp = (void*) 0;
        pios_proc[i].state = PIOS_FREE;
        pios_proc[i].pc = (void*) 0;
    }
    pios_pid_next = 0;
    pios_running = 0;
}

pios_pid_t pios_fork ( void* startPC, void* startSP )
{
    if ( pios_pid_next >= PIOS_MAX_PROCESSES )
        return -1;
    pios_proc[pios_pid_next].pid = pios_pid_next;
    pios_proc[pios_pid_next].pc = startPC;
    pios_proc[pios_pid_next].state = PIOS_RUNNABLE;
    pios_proc[pios_pid_next].sp = startSP;
    pios_pid_next++;
    
    pios_setup_stack ( startPC, startSP );
    // todo kernel-stack
    
    return pios_pid_next-1;
}

void pios_yield ()
{
    pios_sched();
}

void pios_resume ( pios_pid_t pid )
{
    pios_resume_do ( pios_proc[pid].sp );
}
void pios_sched ()
{
    do {
        pios_running++;
        if ( pios_proc[pios_running].state == PIOS_FREE )
            pios_running = 0;
    }
    while  ( pios_proc[pios_running].state != PIOS_RUNNABLE );
    
    pios_resume ( pios_running );
}
