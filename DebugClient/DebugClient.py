from pymemcache.client.base import Client as mcClient
import time as systime
import requestcodes
import serial
import sys
import datetime
import serial.tools.list_ports
import MagnetometerReading_pb2
import SensorReading_pb2
import StateMachineStateReading_pb2
import TorqueOutputReading_pb2
import time
import logging
import struct

logger = logging.getLogger('debug_interface')
logger.setLevel(logging.DEBUG)

console_logger = logging.StreamHandler()
console_logger.setLevel(logging.DEBUG)

formatter = logging.Formatter('%(asctime)s - %(name)s - %(levelname)s - %(message)s')
console_logger.setFormatter(formatter)

logger.addHandler(console_logger)

"""
Receives messages from the MSP432 and responds when appropriate.

Stores/retrieves data from the Memcached buffer for transfers with
the HIL simulation and the data dashboard.
"""

console_logger = logging.StreamHandler()
console_logger.setLevel(logging.DEBUG)

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
            if message_code == 0x06:
                # Magnetometer reading has been requested
                # It should have a zero command size and payload
                magnetometer_reading = \
                    MagnetometerReading_pb2.MagnetometerReading()
                if useMemcached and mc.get("Simulation_Magnetometer_X") != None:
                    magnetometer_reading.x = \
                        struct.unpack('>d', mc.get("Simulation_Magnetometer_X"))[0]
                    magnetometer_reading.y = \
                        struct.unpack('>d', mc.get("Simulation_Magnetometer_Y"))[0]
                    magnetometer_reading.z = \
                        struct.unpack('>d', mc.get("Simulation_Magnetometer_Z"))[0]

                    magnetometer_reading.timestamp_millis_unix_epoch = \
                        round(time.time()*1000)
                else:
                    magnetometer_reading.x = 0
                    magnetometer_reading.y = 0
                    magnetometer_reading.z = 0
                    magnetometer_reading.timestamp_millis_unix_epoch = \
                        round(time.time()*1000)

                logger.info("Sending message: " + str(magnetometer_reading))
                serialised_message = magnetometer_reading.SerializeToString()
                send_message(debug_serial_port, 0x06, serialised_message)


            elif message_code == 0x01:
                bms1_input_current_reading = SensorReading_pb2.SensorReading()
                bms1_input_current_reading.ParseFromString(payload)
                logger.info("Received message data: " + \
                            str(bms1_input_current_reading.value))
                if useMemcached:
                     mc.set("BMS1_Input_Current",
                            struct.pack('>d',
                                bms1_input_current_reading.value))


            elif message_code == 0x02:
                bms1_input_voltage_reading = SensorReading_pb2.SensorReading()
                bms1_input_voltage_reading.ParseFromString(payload)
                logger.info("Received message data: " + \
                            str(bms1_input_voltage_reading.value))
                if useMemcached:
                    mc.set("BMS1_Input_Voltage",
                           struct.pack('>d',
                               bms1_input_voltage_reading.value))


            elif message_code == 0x03:
                primary_mcu_regulator_current_reading = \
                    SensorReading_pb2.SensorReading()
                primary_mcu_regulator_current_reading.ParseFromString(payload)
                logger.info("Received message data: " + \
                            str(primary_mcu_regulator_current_reading.value))
                if useMemcached:
                       mc.set("Primary_MCU_Regulator_Current",
                              struct.pack('>d',
                                  primary_mcu_regulator_current_reading.value))


            elif message_code == 0x04:
                magnetorquer_x_current_reading = \
                    SensorReading_pb2.SensorReading()
                magnetorquer_x_current_reading.ParseFromString(payload)
                logger.info("Received message data: " + \
                            str(magnetorquer_x_current_reading.value))
                if useMemcached:
                    mc.set("Magnetorquer_X_Current",
                           struct.pack('>d',magnetorquer_x_current_reading.value))


            elif message_code == 0x05:
                adcs_system_state_reading = \
                    StateMachineStateReading_pb2.StateMachineStateReading()
                adcs_system_state_reading.ParseFromString(payload)
                logger.info("Received message data: " + \
                            str(adcs_system_state_reading.state))
                if useMemcached:
                    mc.set("ADCS_System_State",
                           struct.pack('>i',adcs_system_state_reading.state))


            elif message_code == 0x07:
                # Torque Output reading has been sent

                torque_output_reading = TorqueOutputReading_pb2.TorqueOutputReading()
                torque_output_reading.ParseFromString(payload)
                logger.info("Received message data: " + str(torque_output_reading))
                if useMemcached:
                    mc.set("Simulation_Torque_X",
                           struct.pack('>d',torque_output_reading.x))
                    mc.set("Simulation_Torque_Y",
                           struct.pack('>d',torque_output_reading.y))
                    mc.set("Simulation_Torque_Z",
                           struct.pack('>d',torque_output_reading.z))


            elif message_code == 0x08:
                # Magnetometer Reading Echo

                magnetometer_reading_echo = MagnetometerReading_pb2.MagnetometerReading()
                magnetometer_reading_echo.ParseFromString(payload)
                logger.info("Message data: " + str(magnetometer_reading_echo))
                if useMemcached:
                    mc.set("Magnetometer_X",
                           struct.pack('>d',magnetometer_reading_echo.x))


            elif message_code == 0x09:
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
        mc.set('hello',
               'you should see this on the Memcached stdout when run with -vv')

    with serial.Serial(**serial_arguments) as debug_serial_port:
        logger.info("Port " + debug_serial_port.portstr + " opened.")
        testLoop(debug_serial_port)
