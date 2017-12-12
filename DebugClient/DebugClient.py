import time as t, serial, sys, datetime
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
    return "{}".format(rawData[0])

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

def debugRequest(code):

    ser.flushInput()  # flush input buffer, discarding all its contents
    ser.flushOutput()  # flush output buffer, aborting current output and discard all that is in buffer

    # Write code
    ser.write([code])

    # Read code
    bytesToRead = ser.inWaiting()
    receivedResponse = False

    while receivedResponse == False: # TODO: Fix to exit via timeout instead of staying in infinite loop
        if bytesToRead > 0:
            buf = ser.readline(bytesToRead)
            if buf[0] == code:
                return buf[1:]
        bytesToRead = ser.inWaiting()
    return False


### Main Program ###

userPort = input("Enter port (COM4):") or "COM4"
userBaud = input("Enter baud rate (115200):") or 115200
userByteOrder = input("Set Endianness - [b]ig/[l]ittle/(s)ystem")
userLogName = input("Enter log file path (msplog.txt)") or "msplog.txt"

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
ser.timeout = 1      #TODO: Check if should have a timeout
ser.xonxoff = False     #disable software flow control
ser.rtscts = False     #disable hardware (RTS/CTS) flow control
ser.dsrdtr = False       #disable hardware (DSR/DTR) flow control
ser.writeTimeout = 2     #timeout for write

try:
    ser.open()
except Exception as e:
    print("Error opening serial port: " + str(e))
    exit()

print("Port " + ser.portstr + " opened.")
f = open(userLogName, "a+")
f.write("Start Log: " + str(datetime.datetime.now()) + "\n")

while ser.isOpen():
    try:
        buf = debugRequest(requestcodes.TEST)
        if buf == False:
            print("Received debug message invalid")
        else:
            time = datetime.datetime.now()
            print(time)
            print("Raw Data: {}".format(buf))
            print(processReceived(buf))
            f.write(str(time) + ": " + processReceived(buf) + "\n")
            f.flush()
            print('')
        t.sleep(1)

    except Exception as e:
        print("Error:" + str(e))
        f.close()
        exit()

print("Stop DebugClient")
f.close()


