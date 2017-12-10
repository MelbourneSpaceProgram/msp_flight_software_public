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
    resultString = "( "
    for testNumber in rawData:
        resultString = resultString + str(testNumber) + ","
    return resultString + " )"

def processReceived(buf):
    version = buf[1]
    type = getType(version, buf[0])

    if version == requestcodes.VERSION:
        rawData = buf[2:]
        if buf[0] == requestcodes.TEMPERATURE:
            data = parseTemperature(rawData)
        elif buf[0] == requestcodes.RADIATION:
            data = parseRadiation(rawData)
        elif buf[0] == requestcodes.TEST:
            data = parseTest(rawData)
        else:
            return "Invalid Type Received"
    else:
        return "Invalid Version Received"

    return "V{} {}: {}".format(version, type, data)

### Main Program ###

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
        # Write
        ser.flushOutput()  # flush output buffer, aborting current output and discard all that is in buffer
        ser.write(requestcodes.TEST)

        # Read
        ser.flushInput() #flush input buffer, discarding all its contents
        bytesToRead = ser.inWaiting()

        if bytesToRead > 0:
            buf = ser.readline()
            if buf != None:
                print(processReceived(buf))
                # f.write(binascii.hexlify(buf))
        time.sleep(0.5)

    except Exception as e:
        print("Error:" + str(e))
        f.close()
        exit()

print("Stop DebugClient")
f.close()


