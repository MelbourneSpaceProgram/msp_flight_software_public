#ifndef SRC_UTIL_MESSAGE_CODES_H_
#define SRC_UTIL_MESSAGE_CODES_H_

// This needs to be kept current with the Python decoder in the DebugClient
// folder
enum DebugMessageType {
    kMockTemperatureSensor = 0,
    kMockRadiationSensor = 1,
    kMockTestContainer = 2,
    kMockTestSensor = 3,
    kPopMessageQueue = 4,
    kAntenna = 5
};

// This needs to be kept current with the Python decoder in the DebugClient
// folder
enum DebugMessageVersion { kV1 = 0 };

enum PayloadType { kTestPayloadCode = 4 };

#endif  // SRC_UTIL_MESSAGE_CODES_H_
