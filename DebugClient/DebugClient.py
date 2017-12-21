import time as systime
import requestcodes, serial, sys, datetime, atexit

"""
Returns a formatted string from raw serial input

:param buf: Raw serial input
:returns: Formatted string for writing to console or log
"""


def processReceived(buf):
    if len(buf) > 2:
        version = buf[1]
        code = buf[0]
        if version == requestcodes.VERSION:
            rawData = buf[2:]
            data = requestcodes.dispatch[code](rawData)
        else:
            return "Invalid Version Received"

        return "V{} {}".format(version, data)
    return "Invalid data received"

"""
Sends a debug request to the MSP432 and returns the raw value of the received input

:param code: The code of the debug request to be sent, see requestcodes.py 
:returns: The raw value of the received input, or False if read was unsuccessful
"""


def debugRequest(code):
    ser.flushInput()  # flush input buffer, discarding all its contents
    ser.flushOutput()  # flush output buffer, aborting current output and discard all that is in buffer

    # Write code
    ser.write([code])

    # Read code
    bytesToRead = ser.inWaiting()
    receivedResponse = False

    while receivedResponse == False:  # TODO: Fix to exit via timeout instead of staying in infinite loop
        if bytesToRead > 2: # Must contain at least version and response code bytes
            buf = ser.read(bytesToRead)
            if buf[0] == code:
                return buf[1:]
        bytesToRead = ser.inWaiting()
    return False


"""
Closes open files and serial ports
"""


def closeHandles():
    print("Closing Log File")
    f.close()
    print("Closing Serial Port")
    ser.close()
    print("Debug Client Terminated")


### Main Program ###


atexit.register(closeHandles)
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
ser.timeout = None  # TODO: Check if should have a timeout
ser.xonxoff = False  # disable software flow control
ser.rtscts = False  # disable hardware (RTS/CTS) flow control
ser.dsrdtr = False  # disable hardware (DSR/DTR) flow control
ser.writeTimeout = 2  # timeout for write

try:
    ser.open()
except Exception as e:
    print("Error opening serial port: " + str(e))
    exit()

print("Port " + ser.portstr + " opened.")
f = open(userLogName, "a+")
f.write("Start Log: " + str(datetime.datetime.now()) + "\n")

toggle = True
while ser.isOpen():
    try:
        if toggle:
            buf = debugRequest(requestcodes.TEMPERATURE)
            toggle = False
        else:
            buf = debugRequest(requestcodes.TEST)
            toggle = True

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
        systime.sleep(1)

    except Exception as e:
        print("Error:" + str(e))
        f.close()
        exit()

closeHandles()
