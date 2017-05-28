#include "uart_old.h"
#include "uart.h"
//#include "image.h"
#include "lcd.h"
#include "gpio.h"

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
    pios_gpio_pinmode ( 2, PIOS_GPIO_OUTPUT );

    while ( 1 )
    {
        pios_gpio_write ( 2, PIOS_GPIO_HIGH );
        wait ( 1 );
        pios_gpio_write ( 2, PIOS_GPIO_LOW );
        wait ( 1 );
    }
}

int main ()
{
    //pios_uart_init();
    
    //pios_uart_puts("\r\nHello Lads!\r\n\0");
    
    /*
    // print the header and some build-info    
    //uart_puts("\r\Booting PiOS newlib :)\r\n");
    char str[]="\r\nBooting PiOS newlib :)\r\n\0";
    //write (0, str, strlen(str));
    printf ("Hello Lads, How are you? %d, 0x%03x\r\n", 123, 0xec );

    char a = pios_uart_getchar();
    while (a != 'a')
    {
        printf ("No\n");
        a = pios_uart_getchar();
    }
    
    printf ("YES!\n");*/
        
    //pios_lcd_init ( LCD_FN8_2 );
    
    //pios_lcd_write ("Hello World!", 12 );
    //uart_init();
    
    //while (1)
    /*
    {
        printNum ( &pios_aux[AUX_MU_LSR_REG], 16, 8 ); 
        printNum ( pios_aux[AUX_MU_LSR_REG], 16, 8 );
        if ( (pios_aux[AUX_MU_LSR_REG] & AUX_TX_IDLE ) != 0);
            //break;
    }*/
    
    pios_uart_init();
    pios_uart_puts("\r\nHello Lads!\r\n\0");
    
    //printf ("GPIO-address: %08x\n", pios_gpio);
    blinkloop();
    
    //while ( 1 ) ;
}
