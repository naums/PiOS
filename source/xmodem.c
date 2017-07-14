#include "xmodem.h"

int pios_xmodem_init ( )
{
    if ( !pios_uart_checkPins () )
    {
        pios_uart_init();
    }
    
    pios_xmodem_signal ( PIOS_XMODEM_NAK );
    return 0;
}

void pios_xmodem_signal ( uint8_t signal )
{
    pios_uart_putchar ( signal );
}

int pios_xmodem_read ( uint8_t* data, uint8_t *lastBlock )
{
    uint8_t * ptr = data;
    enum _pios_xmodem_state_t state = SOH;
    uint8_t currentBlock=0;
    uint8_t blockCompl = ~currentBlock;
    uint8_t size;
    uint8_t checksum[2];
    uint16_t *chkptr = checksum;
    uint8_t chksumbytes=0;
    uint8_t current;
    int retval=0;

    // wait for first character
    // TODO: timeout
    while ( 0 != getchar_asynch ( &current ))
    {
        return -1;
    }
    
    while ( current != PIOS_XMODEM_EOT )
    {
        // cancel / abort the transmission
        if ( current == PIOS_XMODEM_CAN )
            return -3;
        
        switch (state)
        {
            case SOH:
                if ( current == PIOS_XMODEM_SOH )
                {
                    state = BLOCK;
                }
                break;
            case BLOCK:
                if ( current == *lastBlock )
                {
                    // modify the return value to indicate, that this block has already been read
                    retval = -2;
                }
                currentBlock = current;
                state = NBLOCK;
                break;
            case NBLOCK:
                if ( blockCompl != current )
                {
                    pios_xmodem_signal ( PIOS_XMODEM_NAK );
                    return -1;
                }   
                state = DATA;
                size = 128;
                break;
            case DATA:
                *ptr = current;
                ptr++;
                size--;
                if ( size == 0)
                {
                    state = CHKSM;
                    chksumbytes = 0;
                }
                break;         
            case CHKSM:
                checksum[chksumbytes] = current;
                chksumbytes++;
                if (chksumbytes >= 2)
                {
                    // TODO: cheksum!
                    *lastBlock = currentBlock;
                    pios_xmodem_signal ( PIOS_XMODEM_ACK );
                    return retval;
                }   
        }
        
        // wait for next character
        // TODO: timeout
        while ( 0 != getchar_asynch ( &current ) );
    }
    
    // successfully read entire transmission
    return 1;
}

