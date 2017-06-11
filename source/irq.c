#include <pios/irq.h>

volatile pios_irq_controller_t* const pios_irq_controller = (volatile pios_irq_controller_t* const) (PIOS_IO_BASE + PIOS_IRQ_BASE);

void pios_irq_enable()
{
    __asm volatile (
        "mrs r0, cpsr\n"
        "bic r0, r0, #0x80\n"
        "msr cpsr_c, r0\n" );
}
void pios_fiq_enable()
{
    __asm volatile (
        "mrs r0, cpsr\n"
        "bic r0, r0, #0x40\n"
        "msr cpsr_c, r0\n" );
}

void pios_irq_disable ()
{
    __asm volatile (
        "mrs r0, cpsr\n"
        "orr r0, r0, #0x80\n"
        "msr cpsr_c, r0\n" );
}

void pios_fiq_disable ()
{
    __asm volatile (
        "mrs r0, cpsr\n"
        "orr r0, r0, #0x40\n"
        "msr cpsr_c, r0\n" );
}


