#include <stdint.h>
#include <stdbool.h>
#include "pios/xmodem.h"

#define pc pios_uart_putchar
#define gc pios_uart_getchar
#define puts pios_uart_puts

enum xmodem_state
{
    SOH, 
    BLOCK,
    NBLOCK,
    DATA,
    CHKSUM,
    UNKNOWN    
};

struct pios_xmodem_pkg pios_xmodem_lastPackage;
volatile enum xmodem_state pios_xmodem_state;
volatile uint8_t pios_xmodem_block;
volatile uint8_t pios_xmodem_next_nblock;
volatile uint8_t pios_xmodem_dataSize;
volatile bool pios_xmodem_discard;
volatile uint8_t pios_xmodem_lastSentBlock;
volatile uint8_t pios_xmodem_checksum;

void pios_xmodem_init ()
{
    pios_xmodem_state=SOH;
    pios_xmodem_block=0x00;
    pios_xmodem_dataSize=128;
    pios_xmodem_discard=false;
    pios_xmodem_checksum=0;
    uint8_t* p = (uint8_t*) &pios_xmodem_lastPackage;
    for ( int i=0; i<sizeof(pios_xmodem_lastPackage); i++)
    {
        *p = 0;
        p++;
    }
}

int pios_xmodem_read ( uint8_t* input, size_t size )
{
    enum pios_xmodem_return retval = PIOS_XMODEM_UNKNOWN;
    for ( int i = 0; i<size; i++ )
    {
        retval = pios_xmodem_getchar ( input[i] );
        switch ( retval )
        {
            case PIOS_XMODEM_BLOCKFIN :
                //pios_xmodem_signal ( PIOS_XMODEM_ACK );
                return size;
            case PIOS_XMODEM_DISCARD: case PIOS_XMODEM_CHKSUMFAIL:
                //pios_xmodem_signal ( PIOS_XMODEM_NAK );
                return size;
            case PIOS_XMODEM_ENDOFTRANSMISSION:
                return -PIOS_XMODEM_ENDOFTRANSMISSION;
            case PIOS_XMODEM_CANCEL:
                return -PIOS_XMODEM_CANCEL;
            default:
                return retval;
        }
    }
    return retval;
}

int pios_xmodem_write ( uint8_t* output, size_t size )
{
    if ( size == sizeof ( struct pios_xmodem_pkg ) )
    {
        struct pios_xmodem_pkg* pkg = (struct pios_xmodem_pkg*) output;
        pkg->soh = PIOS_XMODEM_SOH;
        pkg->block = 1+pios_xmodem_lastSentBlock;
        pkg->nblock = 255-pkg->block;
        pkg->chksum = 0;
        
        // checksum-calculation
        for ( int i=0; i<128; i++)
        {
            pkg->chksum += pkg->data[i];
        }
        
        uint8_t * p = output;
        for ( int i=0; i<sizeof( struct pios_xmodem_pkg ); i++)
        {
            pc ( *p );
            p++;
        }
        
        return sizeof ( struct pios_xmodem_pkg );
    }
    else
    {
        return 0;
    }
}

void pios_xmodem_eot ()
{
    pios_xmodem_lastSentBlock = 0x00;
}

void pios_xmodem_incBlock ()
{
    pios_xmodem_lastSentBlock++;
}


void pios_xmodem_signal ( uint8_t signal )
{
    pc ( signal );
}

int pios_xmodem_getchar ( uint8_t input )
{
    if ( input == PIOS_XMODEM_CAN )
    {
        pios_xmodem_init();
        return PIOS_XMODEM_CANCEL;
    }
    
    if ( input == PIOS_XMODEM_EOT && pios_xmodem_state == SOH )
    {   
        return PIOS_XMODEM_ENDOFTRANSMISSION;
    }
    
    switch ( pios_xmodem_state )
    {
        case SOH:
            if ( input == PIOS_XMODEM_SOH )
            {
                pios_xmodem_state = BLOCK;
                return PIOS_XMODEM_READ_SOH;
            }
        case BLOCK:
            pios_xmodem_state = NBLOCK;
            if ( pios_xmodem_block == input )
            {
                pios_xmodem_discard = true;
                return PIOS_XMODEM_DUPLICATE;
            }
            
            if ( pios_xmodem_block == 255 )
            {
                if ( input == 0 )
                {
                    pios_xmodem_next_nblock = ~input;
                    return PIOS_XMODEM_READ_BLOCK;
                }
                else
                {
                    pios_xmodem_discard = true;
                    return PIOS_XMODEM_WRONGBLOCK;
                }
            }
            else
            {
                if  ( input == (pios_xmodem_block+1) )
                {
                    pios_xmodem_next_nblock = ~input;
                    return PIOS_XMODEM_READ_BLOCK;
                }
                else
                {
                    pios_xmodem_discard = true;
                }
            }
        case NBLOCK:
            pios_xmodem_state = DATA;
            pios_xmodem_dataSize = 128;
            pios_xmodem_checksum=0;
            if ( input != pios_xmodem_next_nblock )
            {
                pios_xmodem_discard = true;
                return PIOS_XMODEM_WRONGBLOCK;
            }
            else
            {
                pios_xmodem_dataSize = 128;
                return PIOS_XMODEM_READ_NBLOCK;
            }
        case DATA:
            if ( !pios_xmodem_discard )
            {
                pios_xmodem_lastPackage.data[128-pios_xmodem_dataSize] = input;
                pios_xmodem_checksum += input;
                pios_xmodem_dataSize--;
                if ( pios_xmodem_dataSize == 0 )
                {
                    pios_xmodem_state = CHKSUM;
                    return PIOS_XMODEM_DATAFIN;
                }
                return PIOS_XMODEM_READ_DATA;
            }
            else
            {
                return PIOS_XMODEM_DISCARD;
            }   
        case CHKSUM:
            if ( !pios_xmodem_discard )
            {
                pios_xmodem_lastPackage.chksum = input;
                pios_xmodem_state = SOH;
                if ( pios_xmodem_checksum == input )
                {
                    pios_xmodem_block++;
                    return PIOS_XMODEM_BLOCKFIN;
                }
                else
                {
                    return PIOS_XMODEM_CHKSUMFAIL;
                }
            }
            else
            {
                return PIOS_XMODEM_DISCARD;
            }
        default: 
            pios_xmodem_init();
    }
    
    return PIOS_XMODEM_READ_INVALID;
}

void pios_xmodem_copyBlock ( uint8_t* to )
{
    uint8_t* p = (uint8_t*) &pios_xmodem_lastPackage.data[0];
    for ( int i=0; i<128; i++ )
    {
        to[i] = p[i];
    }
}

