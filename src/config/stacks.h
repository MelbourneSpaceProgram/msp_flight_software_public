#ifndef SRC_CONFIG_STACKS_H_
#define SRC_CONFIG_STACKS_H_

#include <src/util/data_types.h>

constexpr uint16_t kRadioListenerStackSize = 1200;
constexpr uint16_t kTransmitShutoffStackSize = 1024;
constexpr uint16_t kUnitTestsStackSize = 20000;
constexpr uint16_t kBeaconStackSize = 2048;
constexpr uint16_t kMemoryLoggerStackSize = 1024;
constexpr uint16_t kTimeSourceStackSize = 1500;
constexpr uint16_t kPayloadProcessorStackSize = 2600;
constexpr uint16_t kConsoleLoggerStackSize = 2048;
constexpr uint16_t kConsoleListenerStackSize = 2048;
constexpr uint16_t kStateManagementStackSize = 1024;
constexpr uint16_t kOrientationControlStackSize = 4096;
constexpr uint16_t kPreDeploymentMagnetometerPollerStackSize = 2048;
constexpr uint16_t kSystemHealthCheckStackSize = 4096;

#endif  //  SRC_CONFIG_STACKS_H_
