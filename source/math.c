#include <stdint.h>  

char* itoa ( uint32_t a )
{
    printNum ( a, 10, 10 );
}

char* hex32 ( uint32_t a )
{
    printNum ( a, 16, 8 );
}

void printString ( const char* a )
{
    pios_uart_puts ( a );
}
