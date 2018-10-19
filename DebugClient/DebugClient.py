from pymemcache.client.base import Client as mcClient
import time as systime
import requestcodes
import serial
import sys
import datetime
import serial.tools.list_ports
from MagnetometerReading_pb2 import MagnetometerReading
from PwmOutputReading_pb2 import PwmOutputReading
from SensorReading_pb2 import SensorReading
from StateMachineStateReading_pb2 import StateMachineStateReading
from TorqueOutputReading_pb2 import TorqueOutputReading
from BDotEstimate_pb2 import BDotEstimate
from Tle_pb2 import Tle
from Time_pb2 import Time
from LocationReading_pb2 import LocationReading
import time
import logging
import struct
import csv
import os
import subprocess
from messagecodes import message_codes

# Config dict for storing global parameters relating to operation
config = {}

### Constants
sync_byte_1 = bytes(b'\xaf')
sync_byte_2 = bytes(b'\xfa')

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
the HIL simulation.
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
        # Use sync bytes to align
        serial_byte = debug_serial_port.read(1)
        while serial_byte != sync_byte_1:
            serial_byte = debug_serial_port.read(1)
            logger.debug("received serial byte " + str(serial_byte))
            logger.debug("Chewing a byte looking for sync")
        # The first sync byte has now been read

        serial_byte = debug_serial_port.read(1)
        if serial_byte != sync_byte_2:
            return (None, None, None)

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


def measurable_filename(measurable_name):
    return 'sensors_' + \
           time.strftime('%Y-%m-%d_%H-%M-%S', config['session_timestamp']) + \
           '_' + \
           measurable_name + \
           '.csv'


def write_to_csv(message):
    measurable_name = message.DESCRIPTOR.name
    fpath = message_log_dir() + \
        measurable_filename(measurable_name)
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
        config['mc'] = mcClient(('localhost',11211))
        config['mc'].set('hello',
               'you should see this on the Memcached stdout when run with -vv')

    with serial.Serial(**serial_arguments) as debug_serial_port:
        # flush input buffer, discarding all its contents
        debug_serial_port.flushInput()
        logger.info("Port " + debug_serial_port.portstr + " opened.")
        config['debug_serial_port'] = debug_serial_port
        # Get a timestamp for the CSV files generated
        config['session_timestamp'] = time.localtime()
        logger.debug(f"Session timestamp: {config['session_timestamp']}")
        testLoop(debug_serial_port, logger, config['mc'])


def testLoop(debug_serial_port, logger, mc):
    # Persistent var for flight-computer-side unit test
    test_message_value = 0
    test_message_timestamp = 0

    try:
        while True:
            message_code, message_size, payload = wait_for_message(debug_serial_port)
            if message_code == None:
                continue
            if message_code in message_actions:
                message_actions[message_code](payload)
            else:
                logger.debug(f"Received message with unknown code: {message_code} size: {message_size}")
    except KeyboardInterrupt as e:
        logger.info("Exiting debug loop")


def message_to_memcached(message, memcached_prefix):
    for field in message.DESCRIPTOR.fields:
        if field.name is 'timestamp_ms':
            binary_spec = '>Q'
        else:
            binary_spec = '>d'
        config['mc'].set(f"{memcached_prefix}_{field.name.upper()}",
               struct.pack(binary_spec, getattr(message, field.name)))


def post_action(nanopb_type,
                memcached_prefix=None,
                log_to_csv=False):
    def action(payload):
        message = nanopb_type()
        message.ParseFromString(payload)
        logger.debug(f"Received message data: {message}")

        if memcached_prefix is not None:
            message_to_memcached(message, memcached_prefix)

        if log_to_csv:
            logger.debug(f"Logging {nanopb_type} to CSV")
            write_to_csv(message)
    return action


def message_from_memcached(message, memcached_prefix, defaults):
    for field in message.DESCRIPTOR.fields:
        memcached_key = f"{memcached_prefix}_{field.name.upper()}"
        if field.name == 'timestamp_ms':
            setattr(message, field.name, round(time.time()*1000))
        else:
            if config['mc'].get(memcached_key) != None:
                print('sending mc vals')
                setattr(message, field.name,
                        struct.unpack('>d', config['mc'].get(memcached_key))[0])
            else:
                if defaults is not None:
                    setattr(message, field.name, defaults[field.name])
                else:
                    setattr(message, field.name, 0)
    return message


def request_action(nanopb_type,
                   response_id,
                   memcached_prefix=None,
                   log_to_csv=False,
                   defaults=None):
    def action(payload):
        message = nanopb_type()

        if memcached_prefix is not None:
            message = message_from_memcached(message, memcached_prefix, defaults)

        if log_to_csv:
            logger.debug(f"Logging {nanopb_type} to CSV")
            write_to_csv(message)

        logger.debug(f"Sending message: {message}")
        serialised_message = message.SerializeToString()
        send_message(config['debug_serial_port'], response_id, serialised_message)
    return action


def test_action(payload):
     test_sensor_reading = SensorReading_pb2.SensorReading()
     test_sensor_reading.ParseFromString(payload)

     config['test_message_value'] = test_sensor_reading.value
     config['test_message_timestamp'] = \
         test_sensor_reading.timestamp_ms


def test_follow_up_action(payload):
    test_sensor_reading = SensorReading_pb2.SensorReading()
    test_sensor_reading.value = config['test_message_value']
    test_sensor_reading.timestamp_ms = \
                                config['test_message_timestamp']
    send_message(debug_serial_port, 0x0A,
                 test_sensor_reading.SerializeToString())


def test_request_action(payload):
    test_sensor_reading = SensorReading_pb2.SensorReading()
    test_sensor_reading.value = 1234
    test_sensor_reading.timestamp_ms = 4321
    send_message(debug_serial_port, 0x0B,
                 test_sensor_reading.SerializeToString())


message_actions = {
    message_codes["magnetometer_reading_request_code"]:
        request_action(MagnetometerReading,
                       0x06,
                       memcached_prefix='Simulation_Magnetometer',
                       defaults={'x': 5, 'y': 5, 'z': 5}),

    message_codes["adcs_system_state_reading_code"]:
        post_action(StateMachineStateReading,
                    memcached_prefix='ADCS'),

    message_codes['torque_output_reading_code']:
        post_action(TorqueOutputReading,
                    memcached_prefix='Simulation_Torque'),

    message_codes['pwm_output_reading_code']:
        post_action(PwmOutputReading,
                    memcached_prefix='Satellite_PWM',
                    log_to_csv=True),

    message_codes['magnetometer_reading_code']:
        post_action(MagnetometerReading,
                    memcached_prefix='Magnetometer',
                    log_to_csv=True),

    message_codes['calibrated_magnetometer_reading_code']:
        post_action(MagnetometerReading,
                    memcached_prefix='Calibrated_Magnetometer'),

    message_codes['b_dot_estimate_code']:
        post_action(BDotEstimate,
                    memcached_prefix='B_Dot_Estimate',
                    log_to_csv=True),
    message_codes['test_sensor_reading_code']:
        test_action,

    message_codes['test_sensor_reading_linked_request_code']:
        test_follow_up_action,

    message_codes['test_sensor_reading_request_code']:
        test_request_action,

    message_codes["tle_request_code"]:
        request_action(Tle,
                       0x0C,
                       memcached_prefix='Simulation_TLE',
                       defaults={
                           'epoch': 00179.78495062,
                           'mean_motion': 10.824191574,
                           'eccentricity_1e7': 1859667,
                           'inclination': 34.2682,
                           'raan': 348.7242,
                           'argument_of_perigee': 331.7664,
                           'mean_anomaly': 19.3264,
                           'bstar_drag': 0.000028098,
                       }),

    message_codes['location_reading_code']:
        post_action(LocationReading,
                    memcached_prefix='Location',
                    log_to_csv=True),
}

if __name__ == "__main__":
    main()

