#include "timer.h"

volatile pios_arm_timer_t* const pios_arm_timer = (volatile pios_arm_timer_t* const) (PBASE + PIOS_ARM_TIMER_BASE);

void pios_arm_timer_init ( uint32_t irq, uint32_t prescaler, uint32_t enable)
{
    if ( !(prescaler == PIOS_ARM_TIMER_PRESCALE_1 || prescaler == PIOS_ARM_TIMER_PRESCALE_16 
      || prescaler == PIOS_ARM_TIMER_PRESCALE_256 || prescaler == PIOS_ARM_TIMER_PRESCALE_UNDEF) )
    {
        prescaler = PIOS_ARM_TIMER_PRESCALE_256;
    }
    uint32_t val = PIOS_ARM_TIMER_32BIT | prescaler | (irq ? PIOS_ARM_TIMER_IRQ_ENABLE : 0) | PIOS_ARM_TIMER_FREERUNNING_ENABLE | (enable ? PIOS_ARM_TIMER_ENABLE:0);
    pios_arm_timer->control = val;
}

void pios_arm_timer_disableIrq ()
{
    pios_arm_timer->control &= ~PIOS_ARM_TIMER_IRQ_ENABLE;
}

void pios_arm_timer_enableIrq()
{
    pios_arm_timer->control |= PIOS_ARM_TIMER_IRQ_ENABLE;
}

void pios_arm_timer_setPrescale ( uint32_t prescaler )
{
    if ( !(prescaler == PIOS_ARM_TIMER_PRESCALE_1 || prescaler == PIOS_ARM_TIMER_PRESCALE_16 
      || prescaler == PIOS_ARM_TIMER_PRESCALE_256 || prescaler == PIOS_ARM_TIMER_PRESCALE_UNDEF) )
    {
        pios_arm_timer->control &= ~(3<<2);
        pios_arm_timer->control |= prescaler;
    }
}

void pios_arm_timer_setFreerunningPrescale ( uint32_t prescaler )
{
    pios_arm_timer->control &= ~(0xff << 16);
    pios_arm_timer->control |= PIOS_ARM_TIMER_FREERUNNING_PRESCALER(prescaler);
}

void pios_arm_timer_enableFreerunning()
{
    pios_arm_timer->control |= PIOS_ARM_TIMER_FREERUNNING_ENABLE;
}

void pios_arm_timer_disableFreerunning()
{
    pios_arm_timer->control &= ~PIOS_ARM_TIMER_FREERUNNING_ENABLE;
}

void pios_arm_timer_enableTimer()
{
    pios_arm_timer->control |= PIOS_ARM_TIMER_ENABLE;
}

void pios_arm_timer_disableTimer()
{
    pios_arm_timer->control &= ~PIOS_ARM_TIMER_ENABLE;
}

void pios_arm_timer_setControl ( uint32_t flags )
{
    pios_arm_timer->control = flags;
}

void pios_arm_timer_set32Bit ( bool bitmode32 )
{
    if ( bitmode32 )
    {
        pios_arm_timer->control |= PIOS_ARM_TIMER_32BIT;
    }
    else
    {
        pios_arm_timer->control &= ~PIOS_ARM_TIMER_32BIT;
    }
}

bool pios_arm_timer_getIrqState ()
{
    return (pios_arm_timer->irqraw & 1);
}

void pios_arm_timer_setLoad ( uint32_t load )
{
    pios_arm_timer->load = load;
}
void pios_arm_timer_setReload ( uint32_t reload )
{
    pios_arm_timer->reload = reload;
}
