#ifndef SRC_UTIL_MESSAGE_CODES_H_
#define SRC_UTIL_MESSAGE_CODES_H_

// This needs to be kept current with the Python decoder in the DebugClient
// folder
// TODO(rskew) make a DebugClient python function that can parse this file
// automatically for synchronising endpoints
enum DebugMessageType {
    // TODO(rskew) check if these are still used
    kMockTemperatureSensor = 0,
    kMockRadiationSensor = 1,
    kMockTestContainer = 2,
    kMockTestSensor = 3,
    kPopMessageQueue = 4,
    kAntenna = 5,

    kBms1InputCurrentReadingCode = 0x01,
    kBms1InputVoltageReadingCode = 0x02,
    kPrimaryMcuRegulatorCurrentReadingCode = 0x03,
    kMagnetorquerXCurrentReadingCode = 0x04,
    kAdcsSystemStateReadingCode = 0x05,
    kMagnetometerReadingRequestCode = 0x06,
    kTorqueOutputReadingCode = 0x07,
    kMagnetometerReadingCode = 0x08,
    kTestSensorReadingCode = 0x09,
    kTestSensorReadingRequestCode = 0x0A,
    kTestRequestCode = 0x0B
};

// This needs to be kept current with the Python decoder in the DebugClient
// folder
enum DebugMessageVersion { kV1 = 0 };

enum PayloadType { kTestPayloadCode = 4 };

#endif  // SRC_UTIL_MESSAGE_CODES_H_
