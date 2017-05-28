#include "uart.h"

volatile uint32_t* const pios_aux = (volatile uint32_t* const) (PBASE + AUX_BASE_ADDR);
void dummy ( unsigned int );


// TODO: Gpio.s
#define GPFSEL1         (PBASE+0x00200004)
#define GPSET0          (PBASE+0x0020001C)
#define GPCLR0          (PBASE+0x00200028)
#define GPPUD           (PBASE+0x00200094)
#define GPPUDCLK0       (PBASE+0x00200098)

void pios_uart_init ( )
{    
    // set the GPIO-pins into the correct state   
    pios_aux[AUX_ENABLE] = AUX_UART;
    pios_aux[AUX_MU_IER] = 0;
    pios_aux[AUX_MU_CNTL] = 0;
    pios_aux[AUX_MU_LCR] = 3;       // undocumented second bit (?)
    pios_aux[AUX_MU_MCR] = 0;
    pios_aux[AUX_MU_IER] = 0;
    pios_aux[AUX_MU_IIR] = 0xc6;
    pios_aux[AUX_MU_BAUD] = 270;
                
                
    pios_gpio_pinmode ( 14, PIOS_GPIO_ALT5 );
    pios_gpio_pinmode ( 15, PIOS_GPIO_ALT5 );
    /**
     * set-up of Pull Up / Down: 
       *    1. Write to GPPUD to set the required control signal (i.e. Pull-up or Pull-Down or neither to remove the current Pull-up/down)
       *    2. Wait 150 cycles – this provides the required set-up time for the control signal
       *    3. Write to GPPUDCLK0/1 to clock the control signal into the GPIO pads you wish to modify – NOTE only the pads which receive a clock will be modified, all others will retain their previous state.
       *    4. Wait 150 cycles – this provides the required hold time for the control signal
       *    5. Write to GPPUD to remove the control signal
       *    6. Write to GPPUDCLK0/1 to remove the clock
       * see: Manual pg 101
    **/

    uint32_t p[2] = { (1<<14)|(1<<15), 0 };
    pios_gpio_pullBulk ( p, PIOS_GPIO_PULL_OFF );
    
    pios_aux[AUX_MU_CNTL] = 3;
}

void pios_uart_puts ( const char* str )
{
    while ( *str )
    {
        pios_uart_putchar(*str);
        str++;
    }
}

void pios_uart_write ( const char* str, size_t len )
{
    for ( int i=0; i<len; i++ )
    {
        pios_uart_putchar ( str[i] );
    }
}
void pios_uart_read ( char* buff, size_t len )
{
    for ( int i=0; i<len; i++ )
    {
        buff[i] = (char) pios_uart_getchar();
    }
}

void pios_uart_putchar ( const char c )
{
    while ( 1 ) 
    {
        if ((pios_aux[AUX_MU_LSR] & AUX_TX_EMPTY) != 0)
            break;
    }
    pios_aux[AUX_MU_IO] = ((0xff) & c);
}

uint32_t pios_uart_getchar ( )
{
    while ( 1 ) 
    {
        if ((pios_aux[AUX_MU_LSR] & AUX_RX_DATA) != 0 ) 
            break;
    }
    return (0xff & pios_aux[AUX_MU_IO]);
}

void pios_uart_setBaud ( uint32_t baud )
{
    pios_aux[AUX_MU_BAUD] = (PIOS_AUX_SYSCLOCK / (baudfactor<<3)) + 1);
}

void pios_uart_setDataSize ( int size )
{
    uint32_t val = pios_aux[AUX_MU_LCR];
    val = (val & 0xfffffffe) | ((size == 8) ? 1 : 0);   // 8 bit mode
    pios_aux[AUX_MU_LCR] = val;
}

bool pios_uart_rxReady ()
{
    return ((pios_aux[AUX_MU_LSR] & AUX_RX_DATA) == 0 );
}

bool pios_uart_txReady ()
{
    return ((pios_aux[AUX_MU_LSR] & AUX_TX_EMPTY) == 0 );
}

int pios_uart_rxQueue ()
{
    return (((0x0f000000) & pios_aux[AUX_MU_STAT]) >> 24);
}
int pios_uart_txQueue ()
{
    return (((0x000f0000) & pios_aux[AUX_MU_STAT]) >> 16);
}


