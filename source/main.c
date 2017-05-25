//#include "uart.h"
//#include "image.h"
#include "lcd.h"

#include <stdio.h>


extern void blinkloop();

int main ()
{
    pios_lcd_init ( LCD_FN4_2 );
    
    pios_lcd_write ("Hello World!", 12 );
    
    blinkloop();
}
