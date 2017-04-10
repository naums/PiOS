#include "uart.h"

int _write ( int fd, const char* ptr, int size )
{
    uart_puts ( ptr );
}

int main ()
{
    uart_init();
    
    // print the header and some build-info    
    //uart_puts("\r\Booting PiOS newlib :)\r\n");
    write (0, "\r\nBooting PiOS newlib :)\r\n");
    printf ("Hello Lads, How are you? %d, 0x%03x\r\n", 123, 0xec );
    
    while (1);
}
