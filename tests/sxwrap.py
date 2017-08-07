import subprocess, serial
import threading

s = serial.Serial ( "/dev/ttyUSB1", 115200, timeout=0 );
p = subprocess.Popen( ["sx", "abcd"], stdin=PIPE, stdout=PIPE, stderr = None );

def read_sx ( ser, pro ):
    while True:
        r = ser.read().decode();
        pro.stdin.write ( r );
        print ("SX-OUTPUT: " + str(r));
        
def read_pi ( pro, ser ):
    while True:
        r = pro.stdout.read().decode();
        ser.write ( r );
        print ("PI-OUTPUT: " + str(r));


sxToPi = threading.Thread ( target = read_sx, args=(s,p) );
PiToSx = threading.Thread ( target = read_pi, args=(s,p) );
sxToPi.start();
PiToSx.start();
