from pymemcache.client.base import Client as mcClient
import time as systime
import requestcodes
import serial
import sys
import datetime
import serial.tools.list_ports
import MagnetometerReading_pb2
import PwmOutputReading_pb2
import SensorReading_pb2
import StateMachineStateReading_pb2
import TorqueOutputReading_pb2
import BDotEstimate_pb2
import Tle_pb2
import Time_pb2
import LocationReading_pb2
import time
import logging
import struct
import csv
import os
import subprocess
from messagecodes import message_codes

def message_log_dir():
    repo_dir = \
        subprocess.run(['git', 'rev-parse', '--show-toplevel'],
                       stdout=subprocess.PIPE)\
        .stdout\
        .decode('utf-8').rstrip()
    return repo_dir + '/DebugClient/logs/'

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
        # Read response code
        message_code = int.from_bytes(debug_serial_port.read(1), byteorder='big')
        if message_code == 0:
            return (None, None, None)
        bytes_to_read = int.from_bytes(debug_serial_port.read(1), byteorder='big')

        logger.info("Messaged received with code " + str(message_code))
        logger.info("Will read {} byte message payload".format(bytes_to_read))

        # TODO(akremor): Set timeout
        response = debug_serial_port.read(bytes_to_read)
        logger.info("Read {} bytes".format(bytes_to_read))

        return (message_code, bytes_to_read, response)
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

def measurable_filename(measurable_name, session_timestamp):
    return 'sensors_' + \
           time.strftime('%Y-%m-%d_%H-%M-%S', session_timestamp) + \
           '_' + \
           measurable_name + \
           '.csv'


def log_to_csv(message, session_timestamp):
    measurable_name = message.DESCRIPTOR.name
    fpath = message_log_dir() + measurable_filename(measurable_name, session_timestamp)
    field_names = [field.name for field in list(message.DESCRIPTOR.fields)]

    if not(os.path.isfile(fpath)):
        csvfile = open(fpath, 'w+')
        csvwriter = csv.writer(csvfile)
        csvwriter.writerow(field_names + ['ground_timestamp_ms'])
        csvfile.close()

    with open(fpath, 'a+') as csvfile:
        csvwriter = csv.writer(csvfile)
        data = [getattr(message, field) for field in field_names]
        csvwriter.writerow(data + [time.time()])


def main():
    suggested_port = detect_serial_port()
    print("Enter port ({}):".format(suggested_port))
    userPort = sys.stdin.readline().strip() or suggested_port
    print("Enter baud rate (115200):")
    userBaud = sys.stdin.readline().strip() or 115200
    print("Use Memcached as part of ADCS simulation? y/n (n)")
    useMemcached_yn = sys.stdin.readline().strip() or 'n'
    useMemcached_yn = 'y'
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
    if useMemcached:
        mc = mcClient(('localhost',11211))
        mc.set('hello',
               'you should see this on the Memcached stdout when run with -vv')

    with serial.Serial(**serial_arguments) as debug_serial_port:
        # flush input buffer, discarding all its contents
        debug_serial_port.flushInput()
        logger.info("Port " + debug_serial_port.portstr + " opened.")
        testLoop(debug_serial_port, logger, mc)

def testLoop(debug_serial_port, logger, mc):
    # Persistent var for flight-computer-side unit test
    test_message_value = 0
    test_message_timestamp = 0

    # Get a timestamp for the CSV files generated
    session_timestamp = time.localtime()

    try:
        while True:
            message_code, message_size, payload = wait_for_message(debug_serial_port)
            if message_code == None:
                continue

            # TODO(akremor): Integrate this with request codes
            if message_code == \
                message_codes["magnetometer_reading_request_code"]:
                magnetometer_reading = \
                    MagnetometerReading_pb2.MagnetometerReading()
                if mc.get("Simulation_Magnetometer_X") != None:
                    magnetometer_reading.x = \
                        struct.unpack('>d', mc.get("Simulation_Magnetometer_X"))[0]
                    magnetometer_reading.y = \
                        struct.unpack('>d', mc.get("Simulation_Magnetometer_Y"))[0]
                    magnetometer_reading.z = \
                        struct.unpack('>d', mc.get("Simulation_Magnetometer_Z"))[0]
                    magnetometer_reading.timestamp_ms = \
                        round(time.time()*1000)
                else:
                    magnetometer_reading.x = 0
                    magnetometer_reading.y = 0
                    magnetometer_reading.z = 0
                    magnetometer_reading.timestamp_ms = \
                        round(time.time()*1000)

                logger.info("Sending message: " + str(magnetometer_reading))
                serialised_message = magnetometer_reading.SerializeToString()
                send_message(debug_serial_port, 0x06, serialised_message)


            elif message_code == \
                message_codes["magnetorquer_x_current_reading_code"]:
                magnetorquer_x_current_reading = \
                    SensorReading_pb2.SensorReading()
                magnetorquer_x_current_reading.ParseFromString(payload)
                logger.info("Received message data: " + \
                            str(magnetorquer_x_current_reading.value))
                mc.set("Magnetorquer_X_Current",
                    struct.pack('>d',magnetorquer_x_current_reading.value))


            elif message_code == \
                message_codes["adcs_system_state_reading_code"]:
                adcs_system_state_reading = \
                    StateMachineStateReading_pb2.StateMachineStateReading()
                adcs_system_state_reading.ParseFromString(payload)
                logger.info("Received message data: " + \
                            str(adcs_system_state_reading.state))
                mc.set("ADCS_System_State",
                       struct.pack('>i',adcs_system_state_reading.state))


            elif message_code == \
                message_codes["torque_output_reading_code"]:

                torque_output_reading = TorqueOutputReading_pb2.TorqueOutputReading()
                torque_output_reading.ParseFromString(payload)
                logger.info("Received message data: " + str(torque_output_reading))
                mc.set("Simulation_Torque_X",
                       struct.pack('>d',torque_output_reading.x))
                mc.set("Simulation_Torque_Y",
                       struct.pack('>d',torque_output_reading.y))
                mc.set("Simulation_Torque_Z",
                       struct.pack('>d',torque_output_reading.z))


            elif message_code == \
                message_codes["pwm_output_reading_code"]:

                pwm_output_reading = PwmOutputReading_pb2.PwmOutputReading()
                pwm_output_reading.ParseFromString(payload)
                logger.info("Received message data: " + str(pwm_output_reading))
                mc.set("Satellite_PWM_X",
                       struct.pack('>d',pwm_output_reading.x))
                mc.set("Satellite_PWM_Y",
                       struct.pack('>d',pwm_output_reading.y))
                mc.set("Satellite_PWM_Z",
                       struct.pack('>d',pwm_output_reading.z))

                logger.info("Logging pwm reading to CSV")
                log_to_csv(pwm_output_reading, session_timestamp)


            elif message_code == \
                message_codes["magnetometer_reading_code"]:
                # Magnetometer Reading Echo

                magnetometer_reading_echo = MagnetometerReading_pb2.MagnetometerReading()
                magnetometer_reading_echo.ParseFromString(payload)
                logger.info("Message data: " + str(magnetometer_reading_echo))
                mc.set("Magnetometer_X",
                       struct.pack('>d',magnetometer_reading_echo.x))

                logger.info("Logging magnetometer reading to CSV")
                log_to_csv(magnetometer_reading_echo, session_timestamp)


            elif message_code == \
                message_codes["calibrated_magnetometer_reading_code"]:

                magnetometer_reading_echo = MagnetometerReading_pb2.MagnetometerReading()
                magnetometer_reading_echo.ParseFromString(payload)
                logger.info("Message data: " + str(magnetometer_reading_echo))
                mc.set("Calibrated_Magnetometer_X",
                       struct.pack('>d',magnetometer_reading_echo.x))
                mc.set("Calibrated_Magnetometer_Y",
                       struct.pack('>d',magnetometer_reading_echo.y))
                mc.set("Calibrated_Magnetometer_Z",
                       struct.pack('>d',magnetometer_reading_echo.z))


            elif message_code == \
                message_codes["b_dot_estimate_code"]:
                b_dot_estimate = \
                    BDotEstimate_pb2.BDotEstimate()
                b_dot_estimate.ParseFromString(payload)
                logger.info("Received message data: " + \
                            str(b_dot_estimate))
                mc.set("B_Dot_Estimate_X",
                       struct.pack('>d',b_dot_estimate.x))
                mc.set("B_Dot_Estimate_Y",
                       struct.pack('>d',b_dot_estimate.y))
                mc.set("B_Dot_Estimate_Z",
                       struct.pack('>d',b_dot_estimate.z))

                logger.info("Logging b-dot estimate to CSV")
                log_to_csv(b_dot_estimate, session_timestamp)


            elif message_code == \
                message_codes["test_sensor_reading_code"]:
                # Test Sensor Reading, store it for the test reading request

                test_sensor_reading = SensorReading_pb2.SensorReading()
                test_sensor_reading.ParseFromString(payload)

                test_message_value = test_sensor_reading.value
                test_message_timestamp = \
                    test_sensor_reading.timestamp_ms


            elif message_code == \
                message_codes["test_sensor_reading_linked_request_code"]:
                # Follow-up test request, send stored data

                test_sensor_reading = SensorReading_pb2.SensorReading()
                test_sensor_reading.value = test_message_value
                test_sensor_reading.timestamp_ms = \
                                            test_message_timestamp
                send_message(debug_serial_port, 0x0A,
                             test_sensor_reading.SerializeToString())


            elif message_code == \
                message_codes["test_sensor_reading_request_code"]:
                # Test request, send known data

                test_sensor_reading = SensorReading_pb2.SensorReading()
                test_sensor_reading.value = 1234
                test_sensor_reading.timestamp_ms = 4321
                send_message(debug_serial_port, 0x0B,
                             test_sensor_reading.SerializeToString())


            elif message_code == \
                message_codes["tle_request_code"]:

                tle = Tle_pb2.Tle()
                if mc.get("Simulation_TLE_Mean_Motion") != None:
                    tle.mean_motion = \
                        struct.unpack('>d',
                                      mc.get("Simulation_TLE_Mean_Motion"))[0]
                    tle.mean_anomaly = \
                        struct.unpack('>d',
                                      mc.get("Simulation_TLE_Mean_Anomaly"))[0]
                    tle.inclination = \
                        struct.unpack('>d',
                                      mc.get("Simulation_TLE_Inclination"))[0]
                    tle.raan = \
                        struct.unpack('>d',
                                      mc.get("Simulation_TLE_Raan"))[0]
                    tle.bstar_drag = \
                        struct.unpack('>d',
                                      mc.get("Simulation_TLE_Bstar"))[0]
                    tle.epoch = \
                        struct.unpack('>d',
                                      mc.get("Simulation_TLE_Epoch"))[0]
                    tle.eccentricity_1e7 = \
                        struct.unpack('>d',
                                      mc.get("Simulation_TLE_Eccentricity_1e7"))[0]
                    tle.argument_of_perigee = \
                        struct.unpack('>d',
                                      mc.get("Simulation_TLE_Argument_Perigee"))[0]
                else:
                    # same TLE values as used in src/adcs/tests/sgp4_tests.cpp
                    tle.epoch = 00179.78495062
                    tle.mean_motion = 10.824191574
                    tle.eccentricity_1e7 = 1859667
                    tle.inclination = 34.2682
                    tle.raan = 348.7242
                    tle.argument_of_perigee = 331.7664
                    tle.mean_anomaly = 19.3264
                    tle.bstar_drag = 0.000028098

                logger.info("Sending message: " + str(tle))
                serialised_message = tle.SerializeToString()
                send_message(debug_serial_port, 0x0C, serialised_message)


            elif message_code == \
                message_codes["location_reading_code"]:

                location_reading = LocationReading_pb2.LocationReading()
                location_reading.ParseFromString(payload)
                logger.info("Received location reading: " + str(location_reading))
                mc.set("Location_Lattitude_Geodetic_Degrees",
                       struct.pack('>d',location_reading.lattitude_geodetic_degrees))
                mc.set("Location_Longitude_Degrees",
                       struct.pack('>d',location_reading.longitude_degrees))
                mc.set("Location_Altitude_Above_Ellipsoid_Km",
                       struct.pack('>d',
                                   location_reading.altitude_above_ellipsoid_km))
                mc.set("Location_Timestamp_Millis_Unix_Epoch",
                       struct.pack('>Q',location_reading.timestamp_ms))

                logger.info("Logging location reading to CSV")
                log_to_csv(location_reading, session_timestamp)


            else:
                logger.info("Received unhandled message with ID ".format(message_code))

    except KeyboardInterrupt as e:
        logger.info("Exiting debug loop")

if __name__ == "__main__":
    main()
