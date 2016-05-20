/**
 * \file process.h
 * \author Stefan Naumann
 * \brief contains interfaces for scheduling and pcb
 **/

#ifndef PILOT_PROCESSES
#define PILOT_PROCESSES

#define MAX_PROCESSES 32

#include "NULL.h"

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

/// pointer to the pcb of the currently running process
struct pcb* running=NULL;
/// list of processes in the system (32 max)
struct pcb processes[MAX_PROCESSES];
/// the number of processes in the system
unsigned int numProcesses=0;

/**
 * \brief saves the context, recollects the context of the next process and continues with it
 * \param[in] oldPC the program counter of the process of the process (return address)
 * \param[in] oldSP the stack pointer before saving the context 
 * \return the ID of the next process
 * \note will call schedule
 **/
int dispatch ();

/**
 * \brief determines the next process for execution with a comparison-function
 * \param[in] func the function for comparing two processes
 **/
struct pcb* schedule ( struct pcb* (*func) ( struct pcb*, struct pcb* ), void (*winning) (struct pcb*) );

/**
 * \brief the calling process will be preempted and a waiting process will be executed
 **/
void yield ();

/**
 * \brief creates a new process and puts it into the waiting-group
 * \param[in] firstPC instruction pointer to the first instruction of the new process
 * \return the ID of the new process
 **/
int createProcess ( void* firstPC );

/**
 * \brief find a new place for a stack
 **/
void* findNewStack ();

#endif
