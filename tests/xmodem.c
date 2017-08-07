#include "pios/xmodem.h"
#include <stdio.h>
#include <stdlib.h>

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
    pios_xmodem_init();
    pios_uart_puts ("READY TO RECV XMODEM-PKG\r\n");
    uint8_t* ptr = data;
    pios_xmodem_signal ( PIOS_XMODEM_NAK );    
    while (1)
    {
        int c = pios_uart_getchar();
        int retval = pios_xmodem_getchar ( (char)c );
        pios_uart_puts ( "input: ");
        printNum (c, 16, 2 );
        pios_uart_puts("return value: \n\r");
        pios_uart_puts(strings[retval]);
        pios_uart_puts("\n\r");
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
        putchar ( data[i] );
    }   
    pios_uart_puts ("\n\r\nFIN!\n\r");
}


