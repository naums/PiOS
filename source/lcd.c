#include "lcd.h"

enum _pios_lcd_command_t lcd_bitmode;

// D0 ... D7
int lcd_gpio[8] = { 11,9,10,24,23,22,27,17 };
int lcd_gpio_en = 2;
int lcd_gpio_rw = 3;
int lcd_gpio_rs = 4;

int lcd_debug = 25;

void  __attribute__((optimize("O0"))) pios_lcd_delay_ms ( uint32_t ms )
{
    for ( ; ms>=1; ms--)
    {
        uint32_t i = 0;
        for ( ; i<1000000; i++);
    }
}

void pios_lcd_reset ()
{    
    pios_lcd_sendCommand ( 0xff );
    pios_lcd_delay_ms(20);
    pios_lcd_sendCommand ( 0x03 );
    pios_lcd_delay_ms(10);
    pios_lcd_sendCommand ( 0x03 );
    pios_lcd_sendCommand ( 0x03 );
    pios_lcd_sendCommand ( 0x02 );  
    
    pios_lcd_sendCommand ( 0x01 );  
}

void pios_lcd_init ( enum _pios_lcd_command_t mode )
{
    #if LCD_8BIT_ENABLE
    if ( !(mode == LCD_FN4_1 || mode == LCD_FN4_2 || mode == LCD_FN8_1 || mode == LCD_FN8_2 ))
        return;
    lcd_bitmode = mode;
    
    for ( int i = 0; i<8; i++ )
        pios_gpio_pinmode ( lcd_gpio[i], 1 );
    pios_gpio_pinmode ( lcd_gpio_en, 1 );
    pios_gpio_pinmode ( lcd_gpio_rs, 1 );
    pios_gpio_pinmode ( lcd_gpio_rw, 1 );
    pios_gpio_pinmode ( lcd_debug, 1 );
    
    pios_gpio_write ( lcd_debug, 0 );
    pios_gpio_write ( lcd_gpio_rw, 0 );
    pios_gpio_write ( lcd_gpio_rs, 0 );
    pios_gpio_write ( lcd_gpio_en, 0 );
    #else
    pios_i2c_init(100000, false, false, false);
    pios_i2c_setAddress ( 0x27 );
    if ( !(mode == LCD_FN4_1 || mode == LCD_FN4_2 ))
        return;
    lcd_bitmode = mode;
    #endif
    
    pios_lcd_reset();
    
    // send 0x28 for 2 line 4bit, 0x20 for 1 line 4bit
    pios_lcd_delay_ms(1);
    pios_lcd_command ( lcd_bitmode );  
    pios_lcd_delay_ms(1);
    
    pios_lcd_command ( LCD_ONBLINK );
    pios_lcd_delay_ms(1);
    pios_lcd_command ( LCD_CLEAR );
    pios_lcd_delay_ms(1);
    pios_lcd_command ( LCD_ENTRY );
    pios_lcd_delay_ms(1);
    pios_lcd_command ( LCD_SETADDR );
    pios_lcd_delay_ms(1);
}

void pios_lcd_write ( const char* str, size_t len )
{
    for ( ; len > 0; len-- )
    {
        pios_lcd_putchar ( *str );
        str++; 
    }
}

void pios_lcd_putchar ( const char chr )
{
    #if LCD_8BIT_ENABLE
    if ( lcd_bitmode == LCD_FN8_1 || lcd_bitmode == LCD_FN8_2 )
        pios_lcd_sendGPIO ( chr, LCD_RS );
    else
    {
        // higher 4 bytes
        pios_lcd_sendByte ( (chr>>4) | LCD_EN | LCD_RS );
        pios_lcd_sendByte ( (chr>>4) | LCD_RS );
        
        // lower 4 bytes
        pios_lcd_sendByte ( (chr&0x0f) | LCD_EN | LCD_RS );
        pios_lcd_sendByte ( (chr&0x0f) | LCD_RS ); 
    }
    #else
    // higher 4 bytes
    pios_lcd_sendByte ( (chr>>4) | LCD_EN | LCD_RS );
    pios_lcd_sendByte ( (chr>>4) | LCD_RS );
    
    // lower 4 bytes
    pios_lcd_sendByte ( (chr&0x0f) | LCD_EN | LCD_RS );
    pios_lcd_sendByte ( (chr&0x0f) | LCD_RS ); 
    #endif
}

void pios_lcd_sendCommand ( uint32_t cmd )
{
    #if LCD_8BIT_ENABLE
    if ( lcd_bitmode == LCD_FN8_1 || lcd_bitmode == LCD_FN8_2 )
        pios_lcd_sendGPIO ( cmd, 0 );
    else
    {
        // higher 4 bytes
        pios_lcd_sendByte ( (cmd>>4) | LCD_EN  );
        pios_lcd_sendByte ( (cmd>>4) );
        
        // lower 4 bytes
        pios_lcd_sendByte ( (cmd&0x0f) | LCD_EN );
        pios_lcd_sendByte ( (cmd&0x0f) ); 
    }
    #else
    // higher 4 bytes
    pios_lcd_sendByte ( (cmd>>4) | LCD_EN );
    pios_lcd_sendByte ( (cmd>>4) );
    
    // lower 4 bytes
    pios_lcd_sendByte ( (cmd&0x0f) | LCD_EN );
    pios_lcd_sendByte ( (cmd&0x0f) );
    #endif
}

void pios_lcd_sendGPIO ( const char data, const char flags )
{
    #if LCD_8BIT_ENABLE
    if ( lcd_bitmode == LCD_FN8_1 || lcd_bitmode == LCD_FN8_2 )
    {
        pios_gpio_write ( lcd_debug, 1 );
        pios_gpio_write ( lcd_gpio_en, 1 );
        for ( int i=0; i<8; i++ )
        {
            pios_gpio_write ( lcd_gpio[i], ((data>>i)&1) );
        }
        
        
        pios_gpio_write ( lcd_gpio_rs, (flags & LCD_RS ? 1 : 0) );
        pios_gpio_write ( lcd_gpio_rw, (flags & LCD_RW ? 1 : 0) );
        pios_lcd_delay_ms(1);
        pios_gpio_write ( lcd_gpio_en, 0 );
        pios_lcd_delay_ms(1);
        pios_gpio_write ( lcd_debug, 0 );
    }
    else
    {
    
    }
    #endif
}

void pios_lcd_sendByte ( const char data )
{
    pios_i2c_write ( &data, 1 );
    pios_i2c_start();
    pios_i2c_wait();
}

void pios_lcd_command ( enum _pios_lcd_command_t cmd )
{
    pios_lcd_sendCommand ( cmd );
}
