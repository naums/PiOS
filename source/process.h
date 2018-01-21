#ifndef PIOS_PROCESS
#define PIOS_PROCESS

#include "pitypes.h"

enum pios_process_state_t {
    PIOS_FREE,
    PIOS_RUNNABLE,
    PIOS_WAIT,
    PIOS_ZOMBIE
};

struct pios_pcb_t {
    pios_pid_t pid;
    void* krnl;
    void* sp;
    void* pc;
    enum pios_process_state_t state;
};

pios_pid_t pios_fork ( void* startPC, void* startSP );

void pios_switch ();
//void pios_yield ();
void pios_resume ( pios_pid_t pid );
void pios_sched ();
extern void pios_setup_stack ( void* startPC, void* startSP );

#endif 
