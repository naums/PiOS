import subprocess, serial
import sys, os, io, struct, time

def myread ( s ):
    r = s.read(1);
    if ( len(r) > 0 ) :# and r!=b'\x00'):
        try:
            print (r.decode(), end="");
        except:
            print (r, end="");
    return r;


if ( len ( sys.argv ) < 2 ):
    print ("Usage: python xmodem.py (filename)")
    sys.exit(1);

try:
    s = serial.Serial ( "/dev/ttyUSB1", 115200, timeout=0 );
except:
    print ("Could not open serial interface /dev/ttyUSB1")
    sys.exit(1);

filename = sys.argv[1];
print ("Transmitting " + filename);

gotNak = False;
while (gotNak == False):
    r = myread(s);
    if ( r == b'\x15' ):
        gotNak = True;

print ("Beginning transmission of file "+filename);
f = io.FileIO ( filename, "rb")
contents = f.readall ();
f.close();
filesize = os.stat ( filename );
filesize = filesize.st_size;

print ( "FIlesize: " + str(filesize) );
index=0;
block=1;
while ( index < filesize ):
    if ( block > 255 ):
        block = 0;    
    s.write ( (1).to_bytes(1, byteorder="big") );
    time.sleep(.1);
    s.write ( (block).to_bytes(1, byteorder="big") );    
    time.sleep(.1);
    s.write ( (255-block).to_bytes(1, byteorder="big") );    
    time.sleep(.1);
    i = 0;
    chksum = 0;

    r = myread(s);
    while (len(r) > 0):
        r = myread(s);
    
    while ( i < 128 ):
        if ( index < filesize ):
            s.write ( contents[index].to_bytes(1, byteorder="big") );
            print ( str(index) + " -- " + chr(contents[index]) );
            chksum += contents[index]
        else:
            s.write ( b'\x00' );
            print ( str ( index) + " !! 0" );
        index+=1;
        i+=1;    
        r = myread(s);
        while (len(r) > 0):
            r = myread(s);
        time.sleep(.1);
        
    chksum = chksum & 0xff;
    s.write ( chksum.to_bytes(1, byteorder="big") );
    print ( str ( chksum ) );

    block+=1;
    if ( block > 255 ):
        block = 0;
    r = myread(s);
    while (len(r) < 1):
        r = myread(s);
    if ( r == b'\x15' ):
        block-=1;
        print ("Transmission of block " + str(block) + " unsuccessful, retry");
        index-=128;
    elif ( r == b'\x18' ):
        print ("Cancelled by client");
        #sys.exit(0);
    elif ( r != b'\x06' ):
        print ("Error, returned: " + str(r));
        #sys.exit(0);

s.write ( b'\x04' ); # EOT
print ("Transmission sucessful");

while True:
    r = myread(s);
