
#include <stdint.h>

extern void printString ( const char*, int, int );
extern uint32_t* frameBuffer;
extern const char* hex32 ( uint32_t );

struct framebufferinfo
{
	uint32_t fb_width;
	uint32_t fb_height;
	uint32_t vfb_width;
	uint32_t vfb_height;
	uint32_t gpu_pitch;
	uint32_t color_depth;
	uint32_t x;
	uint32_t y;
	uint32_t fb_ptr;
	uint32_t fb_size;
};

extern struct framebufferinfo framebufferInfo;

void drawCircle ( unsigned int x, unsigned int y, uint32_t* fb, unsigned int diameter )
{
	uint32_t width = framebufferInfo.fb_width;
	uint32_t height = framebufferInfo.fb_height;
	
	for (unsigned int i=y; i<(y+diameter); i++)
	{
		for (unsigned int j=x; j<(x+diameter); j++)
		{
			fb[(i*width) + j]=0;
		}
	}
}	

int cmain()
{
	uint32_t* fb = frameBuffer;

	drawCircle ( 120, 240, fb, 64 );
	printString ( "Hello World.\nI'm Jimbob!\n\0", 160, 206 );
}



