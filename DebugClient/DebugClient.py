import time, serial, sys, binascii
import requestcodes

### Functions ###

def parseTemperature(rawData):
    rawTemperature = rawData[0:4] #TODO: Convert to 32 bit float
    rawSensorId = rawData[4]
    rawTimeStamp = rawData[5]
    return "( Temp: {}, ID: {}, Time: {} )".format(rawTemperature, rawSensorId, rawTimeStamp)

def parseRadiation(rawData):
    #TODO: Implement
    return "This function is yet to be implemented"

def parseTest(rawData):
    return "({})".format(str(rawData))

def processReceived(buf):
    version = buf[1]
    if version == requestcodes.VERSION:
        rawData = buf[2:]
        if buf[0] == requestcodes.TEMPERATURE:
            data = parseTemperature(rawData)
            type = "Temperature"
        elif buf[0] == requestcodes.RADIATION:
            data = parseRadiation(rawData)
            type = "Radiation"
        elif buf[0] == requestcodes.TEST:
            data = parseTest(rawData)
            type = "Test"
        else:
            return "Invalid Type Received"
    else:
        return "Invalid Version Received"

    return "V{} {}: {}".format(version, type, data)

### Main Program ###

userPort = input("Enter port (COM4):") or "COM4"
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
ser.timeout = 5      #TODO: Check if should have a timeout
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
        ser.flushInput()  # flush input buffer, discarding all its contents
        ser.flushOutput()  # flush output buffer, aborting current output and discard all that is in buffer

        # Write
        ser.write(bytes([requestcodes.TEST]))
        print("Sent TEST request")

        # Read
        bytesToRead = ser.inWaiting()
        receivedResponse = False;
        while receivedResponse == False:
            if bytesToRead > 0:
                buf = ser.readline(bytesToRead)
                if buf != b'':
                    print(buf)
                    print(processReceived(buf))
                    receivedResponse = True
                    # f.write(binascii.hexlify(buf))
            bytesToRead = ser.inWaiting()

        time.sleep(1)

    except Exception as e:
        print("Error:" + str(e))
        f.close()
        exit()

print("Stop DebugClient")
f.close()


