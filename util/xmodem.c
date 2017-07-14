#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <errno.h>

#define PIOS_XMODEM_SOH 0x01
#define PIOS_XMODEM_EOT 0x04
#define PIOS_XMODEM_ACK 0x06
#define PIOS_XMODEM_NAK 0x15
#define PIOS_XMODEM_CAN 0x18

#pragma pack(1)
struct xmodem
{
    uint8_t soh;
    uint8_t block;
    uint8_t nblock;
    uint8_t data[128];
    uint16_t checksum;
};

int fd;
int filesize(FILE* f)
{
    size_t filesize=0;
    if (f)
    {
        fseek(f, 0, SEEK_END);
        filesize=ftell(f);
        fseek(f, 0, SEEK_SET);
        return filesize;
    }   
    else
        return 0;
} 

void xmodem_sendBlock ( uint8_t *ptr, uint8_t size, uint8_t* lastBlock )
{
    struct xmodem d = {.soh = PIOS_XMODEM_SOH, .block = (*lastBlock)+1, .nblock = ~((*lastBlock)+1), .data={0}, .checksum=0 };
    uint8_t* dt = &d.data[0];
    for ( uint8_t i=128; i >0 && size > 0; i-- )
    {
        *dt = *ptr;
        dt++;
        ptr++;
    }
    
    uint8_t* p = &d;
    
    while ( p < d+sizeof(struct xmodem))
    {
        fputc ( *p, fd );
        p++;
    }
}

int main ( int argc, char** argv )
{
    if (argc < 3 )
    {
        printf ("Usage: %s (tty-file) (payload-file)\n", argv[0]);
        return 0;
    }

    char* filename = argv[1];
    
    fd = open ( filename, O_RDWR | O_NOCTTY );

    struct termios ttySettings;
    tcgetattr ( fd, &ttySettings );

    // set 9600 baud as baudrate
    cfsetispeed ( &ttySettings, B115200 );
    cfsetospeed ( &ttySettings, B115200 );

    // set 8 data bits
    ttySettings.c_cflag &= ~CSIZE;
    ttySettings.c_cflag |= CS8;
    // 1 stop bit
    ttySettings.c_cflag &= ~CSTOPB;
    // disable parity bit
    ttySettings.c_cflag &= ~PARENB;

    // disable hardware flow control
    ttySettings.c_cflag &= ~CRTSCTS;
    // disable software flow control
    ttySettings.c_iflag &= ~(IXON | IXOFF | IXANY); 	
    // enable reading from the serial port
    ttySettings.c_cflag |= CREAD | CLOCAL;	
    // read at least 1 Byte before read() returns
    ttySettings.c_cc[VMIN] = 1;
    // wait indefinetely for read to receive bytes
    ttySettings.c_cc[VTIME] = 0;

    // set in non-canonical mode 
    ttySettings.c_iflag &= ~(ICANON | ECHO | ECHOE | ISIG);

    FILE* f = fopen ( argv[2], "r" );
    int size = filesize ( f );
    
    bool start=false;
    uint8_t current;
    while ( start == false )
    {
        uint8_t current = fgetc ( fd );
        if ( current == PIOS_XMODEM_NAK )
            start=true;        
    }
    
    uint8_t *data = malloc ( size );
    fread ( data, size, 1, f );
    fclose ( f );
    
    uint8_t* ptr = data;
    uint8_t lastBlock = 0;
    while ( size > 0 )
    {
        xmodem_sendBlock ( ptr, size, &lastBlock );
        sleep ( 1 );
        current = fgetc ();
        if ( current == PIOS_XMODEM_ACK )
        {
            ptr += 128;
            size-=128;
            lastBlock ++;
        }
        
    }   
}


