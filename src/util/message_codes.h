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

    // A message code of zero looks like a read timeout to the DebugClient
    kNoMessage = 0x00,
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
    kTestRequestCode = 0x0B,
    kTleRequestCode = 0x0C,
    kLocationReadingCode = 0x0D,
    kPwmOutputReadingCode = 0x0E,
    kCalibratedMagnetometerReadingCode = 0x0F,
    kPayloadProcessorInjection = 0x10,
    kBDotEstimateCode = 0x11
};

// This needs to be kept current with the Python decoder in the DebugClient
// folder
enum DebugMessageVersion { kV1 = 0 };

enum PayloadType {
    kMspTestPayloadCode = 4,
    kBeaconPayloadCode = 5,
    kTestOnesPayloadCode = 6,
    kSciencePayloadCode = 7,
    kLithiumGetConfigurationUplinkPayloadCode = 8,
    kQuerySectionsPayloadCode = 9,
    kExceptionsPayloadCode = 10,
    kNumExceptionsPayloadCode = 11,
};

enum Uplinks {
    kNoUplink = 0,
    kTestUplink = 1,
    kLithiumEnableUplink = 2,
    kTleUpdateUplink = 3,
    kForceResetUplink = 4,
    kLithiumBeaconPeriodUplink = 5,
    kScienceDataUplink = 6,
    kLithiumFastPaUplink = 7,
    kLithiumTestUplink = 8,
    kDeployAntennaUplink = 9,
    kEnableDataloggerUplink = 10,
    kFormatSdUplink = 11,
    kIoExpanderToggleUplink = 12,
    kLithiumGetConfigurationUplink = 13,
    kLithiumSetConfigurationUplink = 14,
    kLithiumWriteFlashUplink = 15,
    kSectionUplink = 16,
    kClearSectionsUplink = 17,
    kExecuteSectionsUplink = 18,
    kQuerySectionsUplink = 19,
    kClearExceptionsUplink = 20,
    kQueryExceptionsUplink = 21,
    kQueryNumExceptionsUplink = 22,
    kUplinksLength
};

enum ErrorId {
    kSdMutexCreateFail = 0,
    kSdOpenFail,
    kFileCloseLockFail,
    kFileOpenNotFoundFail,
    kFileOpenExistsFail,
    kFileOpenFail,
    kFileWriteFail,
    kFileReadFail,
    kFileFlushFail,
    kFileSeekFail,
    kFileCloseNullFail,
    kFileCloseWrongFail,
    kFileCloseFail,
    kFileDeleteOpenFail,
    kFileDeleteFail,
    kFileFormatFail,
    kFileThreadFail,
};

enum CatchId {
    kUncaught = 0,
    kTopLevelCatch,
};

#endif  // SRC_UTIL_MESSAGE_CODES_H_
