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

    uint8_t* ptr = (void*)0x0001234;
    uint32_t* p;
    for ( int i = 0; i<=4; i++ )
    {
        p = (uint32_t*)ptr;
        *p = (uint32_t)p;
        ptr+=0x00100000;
    }
    
    ptr = (void*)0x0001234;
    for ( int i = 0; i<=4; i++ )
    {
        p = (uint32_t*)ptr;
        printNum ( *p, 16, 8 );
        ptr+=0x00100000;
    }
    pios_mmu_scrap_table ();
    pios_uart_puts ("\nEnable MMU\n");
    
    // set all sections to 1:1 virtual = physical translation, no cache, no buffers
    for ( uint32_t sections = 0 ; ; sections += 0x00100000 )
    {
        pios_mmu_section ( sections, sections, 0 );
        if ( sections == 0x0fff00000 ) 
            break;        
    }
    
    pios_mmu_section ( 0x00100000, 0x00300000, 0 );
    pios_mmu_section ( 0x00200000, 0x00100000, 0 );
    pios_mmu_section ( 0x00300000, 0x00200000, 0 );
    
    pios_mmu_init();
    pios_mmu_enable ( PIOS_CONTROL_M  );
    pios_uart_puts ("\nEnabled MMU\n");
    
    ptr = (void*)0x0001234;
    for ( int i = 0; i<=4; i++ )
    {
        p = (uint32_t*)ptr;
        printNum ( *p, 16, 8 );
        ptr+=0x00100000;
    }

    blinkloop();
    
    //while ( 1 ) ;
}
