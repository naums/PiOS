/**
 * \brief lcd.h
 * \author Stefan Naumann
 * \date 24th May 2017
 * \brief interfacing an LCD1602 character-display, using the i2c-implementation
 * \note the display I use uses an i2c-chip, which uses the 4-bit mode of the display.
 **/

#ifndef PIOS_LCD1602
#define PIOS_LCD1602

#include "pios/i2c.h"
#include "pios/gpio.h"

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define LCD_8BIT_ENABLE true

/**
 * \brief lists the available commands for the LCD-display
 **/
enum _pios_lcd_command_t
{
    LCD_FN8_1           = 0x30, ///< 8 bit mode with 1 line
    LCD_FN8_2           = 0x38, ///< 8 bit mode with 2 lines
    LCD_FN4_1           = 0x20, ///< 4 bit mode with 1 line
    LCD_FN4_2           = 0x28, ///< 4 bit mode with 2 lines
    LCD_ENTRY           = 0x06, ///< entering entry mode / automatic cursormove after display
    LCD_OFF             = 0x08, ///< switch the display off
    LCD_ONON            = 0x0e, ///< switch the display on with cursor on
    LCD_ONOFF           = 0x0c, ///< switch the display on with cursor off
    LCD_ONBLINK         = 0x0f, ///< switch the display on with blinking cursor
    LCD_SHIFTL          = 0x18, ///< shift the entire display to the left
    LCD_SHIFTR          = 0x1c, ///< shift the entire display to the right
    LCD_LEFT            = 0x10, ///< move the cursor to the left
    LCD_RIGHT           = 0x14, ///< move the cursor to the right
    LCD_CLEAR           = 0x01, ///< clear the entire display
    LCD_SETADDR         = 0x80, ///< set the cursor position in DDRAM (Data-RAM, 0x80 + position)
    LCD_SETCGRAMADDR    = 0x40  ///< set the pointer-address in CGRAM (Character Generation RAM, 0x40 + address)
} typedef pios_lcd_command_t;

/***
 * I think the following Bitformat is correct for the used chip (PCF8574T):
 * DDDD-ESW
 * D -> Data 7 to 4 on the LCD
 * E -> Enable (High to Low enables the operation!)
 * S -> Register Select (
 * W -> R/W-setting (High Read, Low Write)
 **/

#define LCD_EN 0x04
#define LCD_RS 0x02
#define LCD_RW 0x01

extern enum _pios_lcd_command_t lcd_bitmode;

void pios_lcd_init ( enum _pios_lcd_command_t mode );

void pios_lcd_putchar ( const char chr );
void pios_lcd_write ( const char* str, size_t len );
void pios_lcd_sendCommand ( uint32_t cmd );
void pios_lcd_sendByte ( const char data );
void pios_lcd_sendGPIO ( const char data, const char flags );
void pios_lcd_command ( enum _pios_lcd_command_t cmd );

#endif
