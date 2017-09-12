#include "pios/xmodem.h"
#include "pios/uart.h"

#define pc pios_uart_putchar
#define gc pios_uart_getchar
#define puts pios_uart_puts

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
    pc('\r');
    return length;
}

const char* strings[] =
{
    "CANCEL",
    "BLOCKACK",
    "BLOCKFIN",
    "ENDOFTRANSMISSION",
    "READ_SOH",
    "READ_BLOCK",
    "READ_NBLOCK",
    "READ_DATA",
    "DATAFIN",
    "READ_CHKSUM",
    "READ_INVALID",
    "WRONGBLOCK",
    "DUPLICATE",
    "CHKSUMFAIL",
    "DISCARD",
    "UNKNOWN"
};

uint8_t data[256] = {0};

int main ( int argc, char** argv )
{
    pios_uart_init();
    pios_uart_setBaud ( 115200 );
    pios_xmodem_init();
    
    uint8_t* ptr = data;
    printNum ( ptr, 16, 8 );
    pios_xmodem_signal ( PIOS_XMODEM_NAK );
    while (1)
    {
        uint32_t c = gc ();
        int retval = pios_xmodem_getchar ( (char)c );
        pc ( c );
        puts(" -- ");
        printNum ( (char)c, 16, 2 );

        switch ( retval )
        {
            case PIOS_XMODEM_ENDOFTRANSMISSION:
                pios_xmodem_signal ( PIOS_XMODEM_ACK );
                pios_xmodem_init ();
                goto down;
                break;
            case PIOS_XMODEM_BLOCKFIN:
                pios_xmodem_signal ( PIOS_XMODEM_ACK );
                pios_xmodem_copyBlock ( ptr );
                ptr+=128;
                break;
        }        
    }
down:
    for ( int i = 0; i<256; i++ )
    {
        pc ( data[i] );
    }   
    puts ("\n\r\nFIN!\n\r");
    
    while (1)
    {
        puts("UPS");
    }   
}


