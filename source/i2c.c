#include <pios/i2c.h>
#include <pios/native_io.h>

volatile pios_i2c_t* const pios_i2c = (volatile pios_i2c_t* const) (PIOS_IO_BASE + PIOS_I2C_BASE);


void pios_i2c_enable ()
{
    pios_i2c->control |= I2C_EN; 
}

void pios_i2c_disable ()
{
    pios_i2c->control &= ~I2C_EN;
}

void pios_i2c_start ()
{
    pios_i2c->control |= I2C_ST;
}

void pios_i2c_control ( uint32_t flags )
{
    pios_i2c->control = flags;
}

void pios_i2c_clearFifo ()
{
    pios_i2c->control |= I2C_CLEAR;
}

void pios_i2c_dlen ( uint32_t dlen )
{
    pios_i2c->datalen = dlen & (0xffff);
}

void pios_i2c_setAddress ( uint32_t address )
{
    pios_i2c->address = address & (0x7f);
}

uint32_t pios_i2c_getAddress ( )
{
    return pios_i2c->address & (0x7f);
}

void pios_i2c_putchar ( uint32_t data )
{
    while (1) 
    { 
        if ( pios_i2c->status & I2C_TXD ) 
            break;
    }
    pios_i2c->fifo = data & 0xff;
}

void pios_i2c_write ( const char* str, size_t len )
{
    pios_i2c_dlen ( (uint32_t) len );
    for ( int i = 0; i < len; i++ )
        pios_i2c_putchar( (uint32_t)str[i] );
}

void pios_i2c_wait ()
{
    while (1)
    {
        if ( pios_i2c->status & I2C_DONE )
        {
            pios_i2c->status = I2C_DONE;
            break;
        }
    }
}

uint32_t pios_i2c_getchar ()
{
    #if 0
    while (1) { 
        if ( pios_i2c->status & I2C_RXD ) break;
    }
    #else
    // prevent reading nonsense
    if ( !(pios_i2c->status & I2C_RXD) )
        return 0;
    #endif
    return pios_i2c->fifo & 0xff;
}

void pios_i2c_setDivider ( uint32_t div )
{
    pios_i2c->divider = div & 0xffff;
}

uint32_t pios_i2c_getDivider ()
{
    return pios_i2c->divider & 0xffff;
}

void pios_i2c_init ( uint32_t freq, bool irqt, bool irqr, bool irqd )
{
    freq = PIOS_I2C_CLOCKRATE/freq;
    uint32_t flags = ( (irqt ? I2C_INTT : 0) | (irqr ? I2C_INTR : 0) | (irqd ? I2C_INTD : 0) ) | I2C_EN;
    pios_i2c_setDivider ( freq );
    pios_i2c_control ( flags );
    
    pios_gpio_pinmode ( 2, 0x4 );
    pios_gpio_pinmode ( 3, 0x4 );
    pios_i2c_enable();
}
