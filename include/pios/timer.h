/**
 * \file timer.h
 * \author Stefan Naumann
 * \date 28th May 2017
 * \brief interface for the arm-timer of the Raspberry Pi (see pg. 196 of the manual)
 **/

#ifndef PIOS_ARMTIMER
#define PIOS_ARMTIMER

#include <stdint.h>
#include <stdbool.h>

#include "platform.h"
#include "irq.h"

#define PIOS_ARM_TIMER_INTERRUPT_ENABLE() pios_irq_controller->basic_enable = PIOS_IRQ_BASIC_ARM_TIMER_IRQ;

#define PIOS_ARM_TIMER_BASE 0x0000B400

/**
 * \brief mapping the memory registers of the ARM timer
 **/
struct _pios_arm_timer_t
{
    uint32_t load;          ///< load a specific value into the counter, after counted-down an IRQ occurs
    uint32_t value;         ///< a value, counted down. On 0 IRQ is set and the value is reloaded from reload-register
    uint32_t control;       ///< control register, which allows for setting some settings
    uint32_t irqack;        ///< write only for clearing the IRQ-line
    uint32_t irqraw;        ///< is an interrupt pending? (i.e. LSB is set to 1)
    uint32_t irqmasked;     ///< is the IRQ pending bit set and the interrupt enable bit?
    uint32_t reload;        ///< copy of load, but writing will not trigger an overwrite of value, just when value reaches 0
    uint32_t prescale;      ///< setting a prescaler for the timer - LSB 10 Bits (timer_clock = apb_clock/(pre_divider+1))
    uint32_t freerunning;   ///< a free running value (has its own prescaler in control-register)
} typedef pios_arm_timer_t;

extern volatile pios_arm_timer_t* const pios_arm_timer;

/// the timer can be set to 32 bit mode or 16 bit mode (control:1 -> 0)
#define PIOS_ARM_TIMER_32BIT 2
/// 
#define PIOS_ARM_TIMER_PRESCALE_1 (0 << 2)
#define PIOS_ARM_TIMER_PRESCALE_16 (1 << 2)
#define PIOS_ARM_TIMER_PRESCALE_256 (2 << 2)
#define PIOS_ARM_TIMER_PRESCALE_UNDEF (3 << 2)

#define PIOS_ARM_TIMER_IRQ_ENABLE (1 << 5)
#define PIOS_ARM_TIMER_ENABLE (1 << 7)
#define PIOS_ARM_TIMER_HALT_IN_DEBUG (1 << 8)
#define PIOS_ARM_TIMER_FREERUNNING_ENABLE (1 << 9)
#define PIOS_ARM_TIMER_FREERUNNING_PRESCALER(a) ((a&0xff) << 16)

void pios_arm_timer_init ( uint32_t irq, uint32_t prescaler, uint32_t enable );
void pios_arm_timer_disableIrq ();
void pios_arm_timer_enableIrq();
void pios_arm_timer_setPrescale ( uint32_t prescaler );
void pios_arm_timer_setFreerunningPrescale ( uint32_t prescaler );
void pios_arm_timer_enableFreerunning();
void pios_arm_timer_disableFreerunning();
void pios_arm_timer_enableTimer();
void pios_arm_timer_disableTimer();
void pios_arm_timer_setControl ( uint32_t flags );
void pios_arm_timer_set32Bit ( bool bitmode32 );

bool pios_arm_timer_getIrqState ();

void pios_arm_timer_setLoad ( uint32_t load );
void pios_arm_timer_setReload ( uint32_t reload );

#endif
