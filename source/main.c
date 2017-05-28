#include "uart.h"
//#include "image.h"
#include "lcd.h"
#include "gpio.h"
#include "timer.h"

#include <stdio.h>

#define pc pios_uart_putchar

uint32_t printNum ( uint32_t num, uint32_t base, uint32_t length )
{
    if ( base == 16 )
    {
        pc('0');
        pc('x');
    }
    uint32_t count=0;
    uint32_t veryold=num;
    do 
    {
        count++;
        num /= base;
    }
    while ( num > 0 );
    
    uint32_t i=0;
    for (; i<(length-count); i++)
    {
        pc('0');
    }

    uint32_t b = 1;
    for (uint32_t j = 1; j<count; j++)   
    {
        b *= base;      
    }

    num=veryold;
    for (; i<length; i++)
    {
        uint32_t old=num;
        uint32_t a = num / b;
        num = old - (a * b);;
        b /= base;
        uint32_t res = a;

        if (res <= 9)
        {
            pc(('0'+res));
        }
        else
        {
            pc(('A'+(res-10)));
        }
    }
    pc('\n');
    return length;
}


int _write ( int fd, const char* ptr, int size )
{
    for ( int i=0; i<size; i++ )
    {
        pc( ptr[i] );
    }
}

void __attribute__ ((noreturn)) blinkloop() 
{
    pios_gpio_pinmode ( 3, PIOS_GPIO_OUTPUT );
    while ( 1 )
    {
        pios_gpio_write ( 3, PIOS_GPIO_LOW );
        wait(1);
        pios_gpio_write ( 3, PIOS_GPIO_HIGH );
        wait(1);
    }
}

int main ()
{    
    pios_uart_init();
    pios_uart_puts("\r\nHello Lads!\r\n\0");
    
    pios_gpio_pinmode ( 2, PIOS_GPIO_OUTPUT );
    
    pios_arm_timer_setLoad ( 0x2000 );
    pios_arm_timer_init ( true, PIOS_ARM_TIMER_PRESCALE_256, true );
    
    enable_timer_irq();
    
    blinkloop();
    
    //while ( 1 ) ;
}
