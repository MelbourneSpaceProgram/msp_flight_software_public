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

    kTestSensorReadingCode = 0x09,
    kTestRequestCode = 0x0A
};

// This needs to be kept current with the Python decoder in the DebugClient
// folder
enum DebugMessageVersion { kV1 = 0 };

enum PayloadType { kTestPayloadCode = 4 };

#endif  // SRC_UTIL_MESSAGE_CODES_H_
