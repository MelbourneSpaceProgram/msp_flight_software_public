import time, serial, io

class Sensor:
    Temperature, Radiation = range(2)

userPort = input("Enter port (COM1):") or "COM1"
userBaud = input("Enter baud rate (115200):") or 115200

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
        buf = ser.readline()
        print(buf)
        f.write(buf)
    except Exception as e:
        print("Error opening serial port:" + str(e))
        f.close()

print("Stop DebugClient")
f.close()


