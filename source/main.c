#include "uart.h"
#include "image.h"

#include <stdio.h>

int _write ( int fd, const char* ptr, int size )
{
    uart_puts ( ptr );
}

extern void blinkloop();

int main ()
{
    uart_init();
    
    uart_puts("\r\nHello Lads!\r\n\0");
    
    // print the header and some build-info    
    //uart_puts("\r\Booting PiOS newlib :)\r\n");
    char str[]="\r\nBooting PiOS newlib :)\r\n\0";
    //write (0, str, strlen(str));
    printf ("Hello Lads, How are you? %d, 0x%03x\r\n", 123, 0xec );

    struct Image fb = createImage ( RGB, 1920, 1080, (void*) framebufferInfo_ptr );
    
    uart_puts ( "After IMG create\n\0");

    pios_window_init ( &fb );
    
    uart_puts ( "After Win Init\n\0");

    int hwnd = pios_window ( 50, 50, 380, 220 );

    int button = pios_window_control_button ( hwnd, 30, 10, "What have I become?\0");
    int label = pios_window_control_label ( hwnd, 30, 35, "My closest friend\0");
    int checkbox = false;
    int chkbx = pios_window_control_checkbox ( hwnd, 30, 60, "Everyone I know\0", &checkbox );
    int c2 = true;  
    int cb2 = pios_window_control_checkbox ( hwnd, 30, 85, "Goes Away\0", &c2 );
    int r1 = pios_window_control_radiobox ( hwnd, 1, 30, 110, "In the end\0", &c2 );
    int r2 = pios_window_control_radiobox ( hwnd, 1, 30, 135, "My empire of shite\0", &checkbox );
    int tbox = pios_window_control_textbox ( hwnd, 30, 160, 200, 20, "NiN - Hurt" );
    
    uart_puts ( "After Win DESCR\n\0");
    
    pios_windowTitle ( hwnd, "PiOS SettingsManager\0");
    
    pios_window_control_focus ( hwnd, cb2 );
    pios_window_invalidate ( hwnd );
    
    uart_puts ( "After INVALIDATE\n\0");
    
    blinkloop();
}
