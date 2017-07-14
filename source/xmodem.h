#ifndef PIOS_UART_XMODEM
#define PIOS_UART_XMODEM

#include <stdint.h>
#include <pios/uart.h>

#define PIOS_XMODEM_SOH 0x01
#define PIOS_XMODEM_EOT 0x04
#define PIOS_XMODEM_ACK 0x06
#define PIOS_XMODEM_NAK 0x15
#define PIOS_XMODEM_CAN 0x18

#define getchar_asynch pios_uart_getchar_asynch 

/**
 * \brief the state of the XModem protocol handler
 **/
enum _pios_xmodem_state_t
{
    SOH,        ///< start of header will be next to receive
    BLOCK,      ///< the blocknumber will be next
    NBLOCK,     ///< negated block number will be next
    DATA,       ///< data byte(s) will be next
    CHKSM       ///< the checksum will be next (beware: 16 bit checksum!!)
};

/**
 * \brief inits the xmodem-interface (sets up the UART-GPIO-pins)
 **/
int pios_xmodem_init ( );
/**
 * \brief sends a simple signal to the sender
 * \param[in] signal a Byte which shall be sent
 **/
void pios_xmodem_signal ( uint8_t signal );
/**
 * \brief reads a block of XMODEM-transmission
 * \param[out] data the buffer for the block-data (should be at least 128 Bytes!)
 * \param[inout] lastBlock the number of the last successfully read block
 * \return one of the following numbers:
 *    -  0 if successfully read a block
 *    - -1 for not receiving a character (first try, will probably be returned really often!)
 *    - -2 if the block has already been read
 *    - -3 the transmission was cancelled by the sender (CAN)
 *    - 1 an EOT was received -> end of transmission
 **/
int pios_xmodem_read ( uint8_t* data, uint8_t* lastBlock );

#endif
