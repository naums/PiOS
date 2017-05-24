/**
 * \file i2c.h
 * \author Stefan Naumann
 * \date 24. May 2017
 * 
 * Interface definition for the BCS (I2C-compatible) chip of the Raspberry Pi,
 * see page 28 of the BCM2835 manual
 **/

#ifndef PIOS_I2C_BCS
#define PIOS_I2C_BCS

#include <stdint.h>
#include "platform.h"

/// base address offset for BCS1
#define PIOS_I2C_BASE 0x00205000
/// base address offset for BCS2
#define PIOS_I2C_BASE2 0x00804000
/// base address offset for BCS3 (don't use! Used for HDMI!)
#define PIOS_I2C_BASE3 0x00805000

//** flags for the control register **//
#define I2C_EN    ( 1 << 15 )
#define I2C_INTR  ( 1 << 10 )
#define I2C_INTT  ( 1 <<  9 )
#define I2C_INTD  ( 1 <<  8 )
#define I2C_ST    ( 1 <<  7 )
#define I2C_CLEAR ( 1 <<  4 | 1 << 4 )
#define I2C_READ  ( 1 <<  0 )


//** flags for the status register **//
#define I2C_CLKT ( 1 << 9 )
#define I2C_ERR ( 1 << 8 )
#define I2C_RXF ( 1 << 7 )
#define I2C_TXE ( 1 << 6 )
#define I2C_RXD ( 1 << 5 )
#define I2C_TXD ( 1 << 4 )
#define I2C_RXR ( 1 << 3 )
#define I2C_TXW ( 1 << 2 )
#define I2C_DONE ( 1 << 1 )
#define I2C_TA  ( 1 << 0 )

/**
 * \brief datastructure to map the registers to names
 **/
struct _pios_bcs_t 
{
    uint32_t control;   ///< controls features of the i2c device like interrupt generation or enabling the device
    uint32_t status;    ///< information about the device, like is the FIFO full, can it take more data?
    uint32_t datalen;   ///< number of bytes of data to be transmitted or received in _a_ transfer 
    uint32_t address;   ///< the slave address for the communication
    uint32_t fifo;      ///< fifo buffer (15 Bytes long!) as send or receive buffer
    uint32_t divider;   ///< clock divider: frequency is calculated by: core_clock / CDIV (core_clk is 150Mhz)
    uint32_t delay;     ///< fine control over sampling / launch point
    uint32_t stretch;   ///< how long can the slave stretch the clock to decide that it has hung
} typedef pios_bcs_t, pios_i2c_t;

extern pios_i2c_t* pios_i2c;

inline void pios_i2c_enable ()
{
    pios_i2c->control |= I2C_EN; 
}

inline void pios_i2c_disable ()
{
    pios_i2c->control &= ~I2C_en;
}

inline void pios_i2c_start ()
{
    pios_i2c->control |= I2C_ST;
}

inline void pios_i2c_control ( uint32_t flags )
{
    pios_i2c->control = flags;
}

inline void pios_i2c_clearFifo ()
{
    pios_i2c->control |= I2C_CLEAR;
}

inline void pios_i2c_dlen ( uint32_t dlen )
{
    pios_i2c->datalen = dlen & (0xffff);
}

inline void pios_i2c_setAddress ( uint32_t address )
{
    pios_i2c->address = address & (0x7f);
}

inline uint32_t pios_i2c_getAddress ( )
{
    return pios_i2c->address & (0x7f);
}

inline void pios_i2c_send ( uint32_t data )
{
    pios_i2c->fifo = data & 0xff;
}

inline uint32_t pios_i2c_recv ()
{
    return pios_i2c->fifo & 0xff;
}

inline void pios_i2c_setDivider ( uint32_t div )
{
    pios_i2c->divider = div & 0xffff;
}

inline uint32_t pios_i2c_getDivider ()
{
    return pios_i2c->divider & 0xffff;
}

#endif
