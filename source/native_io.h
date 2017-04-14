#ifndef NAUMS_PIOS_NATIVE_IO
#define NAUMS_PIOS_NATIVE_IO

struct vc4_fbinfo 
{
    int width;
    int height;
    int vwidth;
    int vheight;
    int gpupitch;
    int colordepth;
    int x, y;
    int fb_ptr;
    int fb_size;
};

extern void* framebufferInfo_ptr;
extern struct vc4_fbinfo framebufferInfo;

/**
 * \brief waits for about r0 seconds
 * \param r0 amount of time the pi should wait (in seconds)
 * \return NONE
 **/
void wait ( int s );
/**
 * \brief returns the GPIO-base address in r0
 * \return r0 GPIO-base address
 **/
void* gpio_address ();
/**
 * \brief writes a value to the GPIO-Pin (i.e. writes 1 to set or 1 to clear)
 * \param pin GPIO-Pin
 * \param val output-value (1 for on, 0 for off)
 **/
void gpio_write ( int pin, int val );
/**
 * \brief sets the PIN into the mode we want
 * \param pin GPIO-Pin
 * \param val GPIO-Mode
 * \return NONE
 **/
void gpio_pinmode ( int pin, int val );

/**
 * \brief writes a message to a mailbox
 * \param val value to be written to the mailbox (lower 4 Bits MUST be 0!)
 * \param mailbox mailbox to be written to (only lower 4 Bits set!, therefor 0-15)
 **/
void postmanWrite ( int val, int mailbox );

/**
 * \brief reads from a mailbox
 * \param mailbox the mailbox you want to read from (0-15, only lower 4 Bits set)
 * \return the value read in the mailbox
 **/
int postmanRead ( int mailbox );

/**
 * \brief initiates the framebuffer
 * \param w framebuffer width
 * \param h framebuffer height
 * \param depth color depth
 * \return pointer to the framebufferInfo-Data or 0 on error
 **/
void* framebufferInit ( int w, int h, int depth );

#endif
