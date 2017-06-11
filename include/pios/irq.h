/**
 * \file irq.h
 * \date 31st May 2017
 * \author Stefan Naumann, skeleton from http://www.valvers.com/open-software/raspberry-pi/step04-bare-metal-programming-in-c-pt4/
 * \brief definition of the IRQ-controller BCM2835
 **/

#ifndef PIOS_IRQ_CONTROLLER
#define PIOS_IRQ_CONTROLLER

#include <stdint.h>
#include <stdio.h>

#include <pios_port_config.h>

/// the offset address of the IRQ-controller (offset from I/O base address (PBASE))
#define PIOS_IRQ_BASE 0xB200 

/**
 * \brief mapping out the memory mapped registers of the interrupt controller of the BCM2835-chip
 **/
struct _pios_irq_controller_t
{
    uint32_t basic_pending;     ///< are basic interrupts pending?
    uint32_t pending[2];        ///< are interrupts pending
    uint32_t fiq_control;       ///< control register for fast interrupts
    uint32_t enable[2];         ///< enable interrupts
    uint32_t basic_enable;      ///< enable basic interrupts
    uint32_t disable[2];        ///< disable interrupts
    uint32_t disable_basic;     ///< disable basic interrupts
} typedef pios_irq_controller_t;

extern volatile pios_irq_controller_t* const pios_irq_controller;

/** @brief Bits in the Enable_Basic_IRQs register to enable various interrupts.
    See the BCM2835 ARM Peripherals manual, section 7.5 */
#define PIOS_IRQ_BASIC_ARM_TIMER_IRQ         (1 << 0)
#define PIOS_IRQ_BASIC_ARM_MAILBOX_IRQ       (1 << 1)
#define PIOS_IRQ_BASIC_ARM_DOORBELL_0_IRQ    (1 << 2)
#define PIOS_IRQ_BASIC_ARM_DOORBELL_1_IRQ    (1 << 3)
#define PIOS_IRQ_BASIC_GPU_0_HALTED_IRQ      (1 << 4)
#define PIOS_IRQ_BASIC_GPU_1_HALTED_IRQ      (1 << 5)
#define PIOS_IRQ_BASIC_ACCESS_ERROR_1_IRQ    (1 << 6)
#define PIOS_IRQ_BASIC_ACCESS_ERROR_0_IRQ    (1 << 7)

/*
enum _pios_cpu_mode_t
{
    PIOS_CPU_ARM_JAZELLE = (1 << 24),
    PIOS_CPU_ARM_THUMB = (1 << 5),
    PIOS_CPU_ARM_ARM = 0
} typedef pios_cpu_mode_t;*/

void pios_irq_enable();
void pios_fiq_enable();

#endif
