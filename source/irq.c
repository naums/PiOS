#include <irq.h>

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
    printf ("UNDEF :(\n");
    while( 1 )
    {
        /* Do Nothing! */
    }
}
void __attribute__((interrupt("IRQ"))) irq_vector(void)
{
    //void* lr, *sp;
    /*asm volatile ( "mov %0, lr\n"
                   "mov %1, sp\n"
                   : "=r" (lr), "=r" ( sp ) );*/
    printf (" -> ! IRQ :( \n");
    //printf ("SP: 0x%08x\nLR: 0x%08x\n", sp, lr);
 
    while( 1 )
    {
    
    }
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
