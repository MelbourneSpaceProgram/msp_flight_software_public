from pymemcache.client.base import Client as mcClient
import time as systime
import requestcodes
import serial
import sys
import datetime
import serial.tools.list_ports
import SensorReading_pb2
import time
import logging

logger = logging.getLogger('debug_interface')
logger.setLevel(logging.DEBUG)

console_logger = logging.StreamHandler()
console_logger.setLevel(logging.DEBUG)

formatter = logging.Formatter('%(asctime)s - %(name)s - %(levelname)s - %(message)s')
console_logger.setFormatter(formatter)

logger.addHandler(console_logger)

"""
Sends a debug request to the MSP432 and returns the raw value of the received input

:param code: The code of the debug request to be sent, see requestcodes.py
:returns: The raw value of the received input, or False if read was unsuccessful
"""


def send_message(debug_serial_port, messageCode, serialisedMessage):
    debug_serial_port.flushInput()  # flush input buffer, discarding all its contents
    debug_serial_port.flushOutput()  # flush output buffer, aborting current output and discard all that is in buffer

    debug_serial_port.write(serialisedMessage)

    logger.debug("Sent {} bytes".format(len(serialisedMessage)))

    logger.info("Sent message with ID {}".format(messageCode))


def wait_for_message(debug_serial_port):

    try:
        logger.info("Waiting for messages from satellite")
        # Read response code
        message_code = int.from_bytes(debug_serial_port.read(1), byteorder='big')
        bytes_to_read = int.from_bytes(debug_serial_port.read(1), byteorder='big')

        logger.info("Messaged received with code " + str(message_code))
        logger.info("Will read {} byte message payload".format(bytes_to_read))

        # TODO(akremor): Set timeout
        response = debug_serial_port.read(bytes_to_read)
        logger.info("Read {} bytes".format(bytes_to_read))

        return (message_code, bytes_to_read, response)
    except KeyboardInterrupt as e:
        logger.info("Exiting debug loop")


def testLoop(debug_serial_port):
    try:
        while True:
            message_code, message_size, payload = wait_for_message(debug_serial_port)

            # TODO(akremor): Integrate this with request codes
            if message_code == 0x09:
                # Test Sensor Reading, echo it back

                send_message(debug_serial_port, 0x09, payload)

            elif message_code == 0x0A:
                # Test request, send known data

                test_sensor_reading = SensorReading_pb2.SensorReading()
                test_sensor_reading.value = 1234
                test_sensor_reading.timestamp_millis_unix_epoch = 4321
                send_message(debug_serial_port, 0x0A,
                             test_sensor_reading.SerializeToString())


            else:
                logger.info("Received unhandled message with ID ".format(message_code))

    except KeyboardInterrupt as e:
        logger.info("Exiting debug loop")


def detect_serial_port():
    ports = list(serial.tools.list_ports.comports())
    suggested_port = None
    for p in ports:
        if "XDS110 Class Application/User UART" in str(p):
            # Expected string is formatted like "COMX XDS110 Class Application/User UART (COMX)"
            # Only works on Windows systems but the fallback (user entry) is no worse than the previous version
            suggested_port = str(p).split(' ')[0]  # Grabs the first COMX instance
    return suggested_port


if __name__ == "__main__":

    suggested_port = detect_serial_port()
    userPort = input("Enter port ({}):".format(suggested_port)) or suggested_port
    userBaud = input("Enter baud rate (115200):") or 115200
    useMemcached_yn = input("Use Memcached as part of ADCS simulation? y/n (n)") or 'n'
    if useMemcached_yn == 'y':
        useMemcached = True
    else:
        useMemcached = False
    logger.info("Using Memcached: " + str(useMemcached))

    serial_arguments = {"port": userPort,
                        "baudrate": userBaud,
                        "bytesize": serial.EIGHTBITS,
                        "parity": serial.PARITY_NONE,
                        "stopbits": serial.STOPBITS_ONE,
                        "timeout": None,  # TODO: Check if should have a timeout
                        "xonxoff": False,  # disable software flow control
                        "rtscts": False,  # disable hardware (RTS/CTS) flow control
                        "dsrdtr": False,  # disable hardware (DSR/DTR) flow control
                        "writeTimeout": 2  # timeout for write
                        }


    # Connect to memcached process
    # Run Memcached on another terminal with:
    #     memcached -vv
    # and to generate magnetometer values,
    # in another terminal run:
    #     python StandinMemcachedPopulator.py
    if useMemcached:
        mc = mcClient(('localhost',11211))
        print(mc.set("hello","this should show on the memcached terminal if run with -vv"))


    with serial.Serial(**serial_arguments) as debug_serial_port:
        logger.info("Port " + debug_serial_port.portstr + " opened.")
        testLoop(debug_serial_port)
