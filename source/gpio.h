#ifndef PIOS_GPIO
#define PIOS_GPIO

#include "platform.h"
#include <stdint.h>

#define PIOS_GPIO_BASE 0x200000;

#define PIOS_GPIO_LOW 0
#define PIOS_GPIO_HIGH 1

#define PIOS_GPIO_INPUT 0
#define PIOS_GPIO_OUTPUT 1
#define PIOS_GPIO_ALT0 4
#define PIOS_GPIO_ALT1 5
#define PIOS_GPIO_ALT2 6
#define PIOS_GPIO_ALT3 7
#define PIOS_GPIO_ALT4 3
#define PIOS_GPIO_ALT5 2

#define PIOS_GPIO_PULL_UP 2
#define PIOS_GPIO_PULL_DOWN 1
#define PIOS_GPIO_PULL_OFF 0 

struct _pios_gpio_t 
{
    uint32_t fn_select[6];
    uint32_t pad1;
    uint32_t outputset[2];
    uint32_t pad2;
    uint32_t outputclear[2];
    uint32_t pad3;
    uint32_t level[2];
    uint32_t pad4;
    uint32_t eventDetect[2];
    uint32_t pad5;
    uint32_t risingEdgeDetect[2];
    uint32_t pad6;
    uint32_t fallingEdgeDetect[2];
    uint32_t pad7;
    uint32_t highDetect[2];
    uint32_t pad8;
    uint32_t lowDetect[2];
    uint32_t pad9;
    uint32_t asyncRisingEdgeDetect[2];
    uint32_t pad10;
    uint32_t asyncFallingEdgeDetect[2];
    uint32_t pad11;
    uint32_t pullControlEnable;
    uint32_t pullControlClock[2];
} typedef pios_gpio_t;

extern volatile pios_gpio_t* const pios_gpio;

/**
 * \brief pull up or down a set of pins
 * \param p an array which represents the GPIO-pins and their on-values for pulling
 * \param pull the direction for the voltage-pull
 * \note this is a lower-level implementation of the pios_gpio_pudControl-function, use that if unsure
 **/
void pios_gpio_pullBulk ( uint32_t p[2], uint32_t pull );
/**
 * \brief pull up or down a specific pin 
 * \param pin GPIO-Pin
 * \param pull the direction for the voltage-pull
 **/
void pios_gpio_pullControl ( uint32_t pin, uint32_t pull );
/**
 * \brief reads the value of the GPIO-pin (set as input)
 * \param pin the GPIO pin you want the state of (in [0, 53])
 * \return the state of the GPIO-Pin (i.e. HIGH or LOW-signal level), 
 * always returns LOW if the input is invalid
 **/
uint32_t pios_gpio_read ( uint32_t pin );
/**
 * \brief writes a value to the GPIO-Pin (i.e. writes 1 to set or 1 to clear)
 * \param pin GPIO-Pin
 * \param val output-value (1 for on, 0 for off)
 **/
void pios_gpio_write ( uint32_t pin, uint32_t val );
/**
 * \brief sets the PIN into the mode we want
 * \param pin GPIO-Pin
 * \param val GPIO-Mode
 * \return NONE
 **/
void pios_gpio_pinmode ( uint32_t pin, uint32_t val );

#endif
