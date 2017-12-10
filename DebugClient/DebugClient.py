import time, serial, sys, binascii

def getType(version, type):
    if version == 0:
        if type == 0:
            return "Temperature Sensor"
        elif type == 1:
            return "Radiation Sensor"
        elif type == 3:
            return "Test Sensor"
    return "Undefined Type"

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
    rawData = buf[2:]
    if type == "Temperature Sensor":
        data = parseTemperature(rawData)
    elif type == "Radiation Sensor":
        data = parseRadiation(rawData)
    elif type == "Test Sensor":
        data = parseTest(rawData)
    else :
        data = None
    return "V{} {}: {}".format(version, type, data)

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

        if bytesToRead > 0:
            buf = ser.read(bytesToRead)
            if buf != None:
                print(processReceived(buf))
                # f.write(binascii.hexlify(buf))
    except Exception as e:
        print("Error:" + str(e))
        f.close()
        exit()

print("Stop DebugClient")
f.close()


