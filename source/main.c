#include <pios/uart.h>
#include <pios/gpio.h>
#include <pios/mmu.h>

#include <pios/irq.h>
#include "pios_port_config.h"
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


int _write ( int fd, const char* ptr, size_t size )
{
    size_t i=0;
    for ( ; i<size; i++ )
    {
        pc( ptr[i] );
    }
    return i;
}

void __attribute__ ((noreturn)) blinkloop() 
{
    pios_gpio_pinmode ( PIOS_IO_ACT_LED, PIOS_GPIO_OUTPUT );
    while ( 1 )
    {
        pios_gpio_write ( PIOS_IO_ACT_LED, PIOS_GPIO_LOW );
        wait(1);
        pios_gpio_write ( PIOS_IO_ACT_LED, PIOS_GPIO_HIGH );
        wait(1);
    }
}
 
int main ()
{    
    pios_uart_puts("\r\nHello Lads!\r\n\0");

    uint32_t* ptr = 0x0001234;
    for ( int i = 0; i<=4; i++ )
    {
        *ptr = ptr;
        ptr+=0x00100000;
    }
    
    ptr = 0x0001234;
    for ( int i = 0; i<=4; i++ )
    {
        printNum ( *ptr, 16, 8 );
        ptr+=0x00100000;
    }
    
    printf ("\nEnable MMU\n");
    
    ptr=0x00000000;
    for ( int i = 0; i<=4; i++ )
    {
        pios_mmu_section ( ptr, ptr, PIOS_MMU_CACHE | PIOS_MMU_BUFFER );
    }
    
    pios_mmu_init();
    pios_mmu_enable ( PIOS_CONTROL_M | PIOS_CONTROL_U | PIOS_CONTROL_I | PIOS_CONTROL_Z | PIOS_CONTROL_C );
    
    ptr = 0x0001234;
    for ( int i = 0; i<=4; i++ )
    {
        printNum ( *ptr, 16, 8 );
        ptr+=0x00100000;
    }

    blinkloop();
    
    //while ( 1 ) ;
}
