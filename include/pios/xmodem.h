/**
 * \file xmodem.h
 * \brief Xmodem implementation
 **/

#ifndef PIOS_XMODEM
#define PIOS_XMODEM

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

// Start of Header
#define PIOS_XMODEM_SOH 0x01
// End of Transmission
#define PIOS_XMODEM_EOT 0x04
// Acknowledge
#define PIOS_XMODEM_ACK 0x06
// Not Acknowledge / negative Acknowledge
#define PIOS_XMODEM_NAK 0x15
// Cancel
#define PIOS_XMODEM_CAN 0x18

/**
 * \brief defines return-values indicating the internal state of the XMODEM-package parser
 **/
enum pios_xmodem_return
{
    PIOS_XMODEM_CANCEL,                 ///< transmission has been cancelled
    PIOS_XMODEM_BLOCKACK,               ///< 
    PIOS_XMODEM_BLOCKFIN,               ///< a complete block has been read
    PIOS_XMODEM_ENDOFTRANSMISSION,      ///< an EOT has been read, i.e. all block have been received sucessfully
    PIOS_XMODEM_READ_SOH,               ///< a SOH has been received
    PIOS_XMODEM_READ_BLOCK,             ///< the block-number has been received
    PIOS_XMODEM_READ_NBLOCK,            ///< the negative block-number has been received
    PIOS_XMODEM_READ_DATA,              ///< a data byte has been received
    PIOS_XMODEM_DATAFIN,                ///< the end of the data has been recognized
    PIOS_XMODEM_READ_CHKSUM,            ///< checksum-byte has been received
    PIOS_XMODEM_READ_INVALID,           ///< something invalid has been received
    PIOS_XMODEM_WRONGBLOCK,             ///< a wrong block-number has been received and will discard that block
    PIOS_XMODEM_DUPLICATE,              ///< the block has already been received and will be discarded the second time
    PIOS_XMODEM_CHKSUMFAIL,             ///< the checksum-test failed -> don't use the block!
    PIOS_XMODEM_DISCARD,                ///< the block-data has been discarded due to an error early on
    PIOS_XMODEM_UNKNOWN                 ///< uhm uh...
};

#pragma pack(1)
/**
 * \brief structure defining an xmodem-package
 **/
struct pios_xmodem_pkg 
{
    uint8_t soh;            ///< start if header-byte
    uint8_t block;          ///< block-number
    uint8_t nblock;         ///< negative block number (ones-compliment)
    uint8_t data[128];      ///< the data itself (always 128 bytes)
    uint8_t chksum;         ///< checksum (sum over all bytes)
};

/**
 * \brief resets the parser
 **/
void pios_xmodem_init ();
/**
 * \brief read an array of bytes and parse it as XMODEM-structure
 * \param[in] input the input-array
 * \param[in] size the size of the input-array (number of bytes)
 * \return either negative: see pios_xmodem_return, or positive: number of bytes into the input-array
 **/
int pios_xmodem_read ( uint8_t* input, size_t size );
/**
 * \brief writes an array of bytes as XMODEM-structure (output needs to be a ptr to struct pios_xmodem_pkg)
 * \param[in] output pointer to the package-data
 * \param[in] size the size of that package (will abort if != sizeof(struct pios_xmodem_pkg))
 * \return number of bytes written
 **/
int pios_xmodem_write ( uint8_t* output, size_t size );
/**
 * \brief write a single byte to the XMODEM-device
 * \param[in] signal the signal to be sent
 **/
void pios_xmodem_signal ( uint8_t signal );
/**
 * \brief parses an XMODEM-structure byte per byte
 * \param[in] input the input-byte
 * \return an error code according to the internal state and the input-byte
 * \note has an internal state!, there is no output in here
 **/
int pios_xmodem_getchar ( uint8_t input );

/**
 * \brief copies the lastly received block somewhere into the memory. 
 * \param[inout] to pointer to the memory block, where you want the data to be copied to
 * \note do not use while a transfer is ongoing!
 **/
void pios_xmodem_copyBlock ( uint8_t* to );

#endif


