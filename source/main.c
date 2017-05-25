#include "uart.h"
//#include "image.h"
#include "lcd.h"

#include <stdio.h>

int _write ( int fd, const char* ptr, int size )
{
    pios_uart_write ( ptr, size );
}

extern void blinkloop();

int main ()
{
    /*pios_uart_init();
    
    pios_uart_puts("\r\nHello Lads!\r\n\0");
    
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
        
    pios_lcd_init ( LCD_FN8_2 );
    
    pios_lcd_write ("Hello World!", 12 );
    
    blinkloop();
}
