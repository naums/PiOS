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

#define AUX_UART ( 1 << 0 )
#define AUX_SPI0 ( 1 << 1 )
#define AUX_SPI1 ( 1 << 2 )

#define AUX_TX_IDLE ( 1 << 6 )
#define AUX_TX_EMPTY ( 1 << 5 )
#define AUX_RX_OVERRUN ( 1 << 1 )
#define AUX_RX_DATA ( 1 << 0 )

extern volatile uint32_t* const pios_aux;

void pios_uart_init ( );
void pios_uart_write ( const char* str, size_t len );
void pios_uart_read ( char* buff, size_t len );

void pios_uart_putchar ( const char c );
uint32_t pios_uart_getchar ( );
void pios_uart_setBaud ( uint32_t baud );
void pios_uart_setDataSize ( int size );
void pios_uart_puts ( const char* str );

bool pios_uart_rxReady ();
bool pios_uart_txReady ();

int pios_uart_rxQueue ();
int pios_uart_txQueue ();

void pios_uart_flush ();

#endif
