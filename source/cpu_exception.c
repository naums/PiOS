#include <pios/gpio.h>
#include <pios/irq.h>
#include <pios/timer.h>

void __attribute__((interrupt("UNDEF"))) pios_exception_undef(void)
{
    /*void* lr, *sp;
    __asm volatile ( "mov %0, lr\n"
                   "mov %1, sp\n"
                   : "=r" (lr), "=r" ( sp ) );*/
    pios_uart_puts ("UNDEF :( \n");
    //printf ("SP: 0x%08x\nLR: 0x%08x\n", sp, lr);
    while( 1 )
    {
        /* Do Nothing! */
    }
}
void __attribute__((interrupt("IRQ"))) pios_exception_irq(void)
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
void __attribute__((interrupt("FIQ"))) pios_exception_fiq(void)
{
    pios_uart_puts ("FIQ :( \n");
    while( 1 )
    {
        /* Do Nothing! */
    }
}
void __attribute__((interrupt("SWI"))) pios_exception_swi(void)
{
    void* lr, *sp;
    /*__asm volatile ( "mov %0, lr\n"
                   "mov %1, sp\n"
                   : "=r" (lr), "=r" ( sp ) );*/
    pios_uart_puts (" -> ! SWI :( \n");
    //printf ("SP: 0x%08x\nLR: 0x%08x\n", sp, lr);
    while( 1 )
    {
        /* Do Nothing! */
    }
}
void __attribute__((interrupt("ABORT"))) pios_exception_abort(void)
{
    pios_uart_puts ("Data ABORT :(\n");
    while( 1 )
    {
        /* Do Nothing! */
    }
}
