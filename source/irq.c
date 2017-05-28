#include "irq.h"
#include "gpio.h"
#include "timer.h"

static struct rpi_irq_controller_t* rpiIRQController = (struct rpi_irq_controller_t*)IRQ_BASE;

struct rpi_irq_controller_t* RPI_GetIrqController( void )
{
    return rpiIRQController;
}

void enable_timer_irq ()
{
    RPI_GetIrqController()->Enable_Basic_IRQs = RPI_BASIC_ARM_TIMER_IRQ;
}

void __attribute__((interrupt("UNDEF"))) undef_vector(void)
{
    /*void* lr, *sp;
    __asm volatile ( "mov %0, lr\n"
                   "mov %1, sp\n"
                   : "=r" (lr), "=r" ( sp ) );*/
    printf ("UNDEF :( \n");
    //printf ("SP: 0x%08x\nLR: 0x%08x\n", sp, lr);
    while( 1 )
    {
        /* Do Nothing! */
    }
}
void __attribute__((interrupt("IRQ"))) irq_vector(void)
{
    static int val = PIOS_GPIO_HIGH;
    //void* lr, *sp;
    /*asm volatile ( "mov %0, lr\n"
                   "mov %1, sp\n"
                   : "=r" (lr), "=r" ( sp ) );*/
    pios_uart_puts (" -> ! IRQ :) \n");
    
    pios_arm_timer->irqack = 1;
    val = ((val == PIOS_GPIO_HIGH) ? PIOS_GPIO_LOW : PIOS_GPIO_HIGH);
    pios_gpio_write ( 2, val );
    
}
void __attribute__((interrupt("FIQ"))) fiq_vector(void)
{
    printf ("FIQ :( \n");
    while( 1 )
    {
        /* Do Nothing! */
    }
}
void __attribute__((interrupt("SWI"))) swi_vector(void)
{
    void* lr, *sp;
    __asm volatile ( "mov %0, lr\n"
                   "mov %1, sp\n"
                   : "=r" (lr), "=r" ( sp ) );
    printf (" -> ! SWI :( \n");
    printf ("SP: 0x%08x\nLR: 0x%08x\n", sp, lr);
    while( 1 )
    {
        /* Do Nothing! */
    }
}
void __attribute__((interrupt("ABORT"))) abort_vector(void)
{
    printf ("ABORT :(\n");
    while( 1 )
    {
        /* Do Nothing! */
    }
}
