import time, serial, sys, binascii

class Sensor:
    Temperature, Radiation = range(2)

userPort = input("Enter port (COM1):") or "COM1"
userBaud = input("Enter baud rate (115200):") or 115200
userByteOrder = input("Set Endianness - [b]ig/[l]ittle/(s)ystem")

if userByteOrder.lower() == "b":
    userByteOrder = 'big'
elif userByteOrder.lower() == 'l':
    userByteOrder = 'little'
else:
    userByteOrder = sys.byteorder

ser = serial.Serial()
ser.port = userPort
ser.baudrate = userBaud
ser.bytesize = serial.EIGHTBITS
ser.parity = serial.PARITY_NONE
ser.stopbits = serial.STOPBITS_ONE
ser.timeout = None      #TODO: Check if should have a timeout
ser.xonxoff = False     #disable software flow control
ser.rtscts = False     #disable hardware (RTS/CTS) flow control
ser.dsrdtr = False       #disable hardware (DSR/DTR) flow control
ser.writeTimeout = 2     #timeout for write

try:
    ser.open()
except Exception as e:
    print("Error opening serial port: " + str(e))
    exit()

print("Port " + ser.portstr + " will be used.")

f = open("msplog.txt", "a+")

while ser.isOpen():
    try:
        ser.flushInput() #flush input buffer, discarding all its contents
        ser.flushOutput() #flush output buffer, aborting current output and discard all that is in buffer
        time.sleep(0.5)

        bytesToRead = ser.inWaiting()
        buf = ser.read()
        if buf != None:
            print(binascii.hexlify(buf))
            f.write(binascii.hexlify(buf))
    except Exception as e:
        print("Error:" + str(e))
        f.close()
        exit()

print("Stop DebugClient")
f.close()


