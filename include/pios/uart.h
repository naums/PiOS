/**
 * \file uart.h
 * \author Stefan Naumann
 * \date 17. May 2017
 * \brief Code for the AUX-miniUART of the Raspberry Pi
 **/

#ifndef PIOS_UART
#define PIOS_UART

#include "platform.h"
#include "gpio.h"
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/// will be used as offset with the PBASE-address to get the AUX address 
/// in the I/O-device memory section
#define AUX_BASE_ADDR 0x00215000

/// system clock for the baudrate calculation
#define PIOS_AUX_SYSCLOCK 250000000

/**
 * \brief enum type for enumerating the registers of the AUX-peripheral of BCM2835
 * \note see section 2.1.1 in the manual
 **/
enum pios_aux_t
{
    AUX_IRQ         =  0,       ///< AUX Interrupt status (for hierarchical source-detection)
    AUX_ENABLE      =  1,       ///< AUX enable (last 3 bits: SPI2, SPI1, UART [2,1,0])
    
    AUX_MU_IO       = 16,       ///< UART I/O Data
    AUX_MU_IER      = 17,       ///< UART IRQ Enable
    AUX_MU_IIR      = 18,       ///< UART IRQ Identify
    AUX_MU_LCR      = 19,       ///< UART Line Control
    AUX_MU_MCR      = 20,       ///< UART Modem Control
    AUX_MU_LSR      = 21,       ///< UART Line Status
    AUX_MU_MSR      = 22,       ///< UART Modem Status
    AUX_MU_SCRATCH  = 23,       ///< UART Scratch ( an extra 8 bit buffer(?) )
    AUX_MU_CNTL     = 24,       ///< UART Extra Control
    AUX_MU_STAT     = 25,       ///< UART Extra Status
    AUX_MU_BAUD     = 26,       ///< UART Baudrate
    
    AUX_SPI0_CNTL0  = 32,       ///< SPI 0 Control Register 0
    AUX_SPI0_CNTL1  = 33,       ///< SPI 0 Control Register 1
    AUX_SPI0_STAT   = 34,       ///< SPI 0 Status
    AUX_SPI0_IO     = 35,       ///< SPI 0 Data
    AUX_SPI0_PEEK   = 36,       ///< SPI 0 Peek
    
    AUX_SPI1_CNTL0  = 48,       ///< SPI 1 Control Register 0
    AUX_SPI1_CNTL1  = 49,       ///< SPI 1 Control Register 1
    AUX_SPI1_STAT   = 50,       ///< SPI 1 Status
    AUX_SPI1_IO     = 51,       ///< SPI 1 Data
    AUX_SPI1_PEEK   = 52        ///< SPI 1 Peek
};

/// enable bit for the ENABLE-operation
#define AUX_UART ( 1 << 0 )
/// bit for SPI0 ENABLE-operation
#define AUX_SPI0 ( 1 << 1 )
/// bit for SPI1 ENABLE-operation
#define AUX_SPI1 ( 1 << 2 )

/// bit for finding out whether the Transmitter is idle
#define AUX_TX_IDLE ( 1 << 6 )
/// can the transmitter-queue receive another byte of data
#define AUX_TX_EMPTY ( 1 << 5 )
/// is the receiver overrung, i.e. were data lost
#define AUX_RX_OVERRUN ( 1 << 1 )
/// is there a byte in the receiver-queue?
#define AUX_RX_DATA ( 1 << 0 )

/**
 * \brief mapping of AUX-registers, use with pios_aux_t as indices
 **/
extern volatile uint32_t* const pios_aux;

/**
 * \brief initiate the miniUART of the AUX-peripheral
 **/
void pios_uart_init ( );
/**
 * \brief write a string to the UART
 * \param[in] str the string to be transmitted
 * \param[in] len the length of the string to be sent
 **/
void pios_uart_write ( const char* str, size_t len );
/**
 * \brief read a string of data from the UART-queue
 * \param[out] buff the buffer to be written to
 * \param[in] len the maximal length of data to be read
 **/
void pios_uart_read ( char* buff, size_t len );

/**
 * \brief write a single character to the transmitter-queue
 * \param[in] c the character to print
 **/
void pios_uart_putchar ( const char c );
/**
 * \brief read a single character from the UART
 * \return the single character
 **/
uint32_t pios_uart_getchar ( );
/**
 * \brief set a specific baudrate to the UART
 * \param[in] baud the wanted baudrate
 **/
void pios_uart_setBaud ( uint32_t baud );
/**
 * \brief set the data-size of the uart (7 or 8 bit)
 * \param[in] size the data-size of the characters in bits
 **/
void pios_uart_setDataSize ( int size );
/**
 * \brief print a string to the uart
 * \param[in] str the string to be transmitted
 **/
void pios_uart_puts ( const char* str );

/**
 * \brief is the receiver-queue ready?
 **/
bool pios_uart_rxReady ();
/**
 * \brief is the transmitter-queue ready?
 **/
bool pios_uart_txReady ();

/**
 * \brief get the receiver-queue-state
 **/
int pios_uart_rxQueue ();
/**
 * \brief get the transmitter-queue-state
 **/
int pios_uart_txQueue ();

/**
 * \brief wait until the transmitter is idle again, i.e. sent it's complete queue
 **/
void pios_uart_flush ();

#endif
