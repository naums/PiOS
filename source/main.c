#include "uart.h"

int main ()
{
    uart_init();
    
    // print the header and some build-info    
    uart_puts("\r\Booting PiOS newlib :)\r\n");
}
