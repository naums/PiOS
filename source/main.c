#include "uart.h"

#include <stdio.h>

int _write ( int fd, const char* ptr, int size )
{
    uart_puts ( ptr );
}

extern void blinkloop();

int main ()
{
    uart_init();
    
    uart_puts("\r\nHello Lads!\r\n\0");
    
    // print the header and some build-info    
    //uart_puts("\r\Booting PiOS newlib :)\r\n");
    char str[]="\r\nBooting PiOS newlib :)\r\n\0";
    //write (0, str, strlen(str));
    printf ("Hello Lads, How are you? %d, 0x%03x\r\n", 123, 0xec );
    
    blinkloop();
}
