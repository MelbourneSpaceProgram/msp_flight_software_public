#include <src/board/i2c/i2c.h>
#include <src/config/satellite.h>
#include <src/messages/antenna_message.h>
#include <src/telecomms/antenna.h>
#include <src/util/task_utils.h>
#include <xdc/runtime/Log.h>

Antenna *Antenna::instance = NULL;

// Initial call is not thread safe
Antenna *Antenna::GetAntenna() {
    if (instance == NULL) {
        instance = new Antenna();
    }
    return instance;
}

Antenna::Antenna() {
    initialised = false;
    bus = NULL;
}

bool Antenna::SafeDeploy() const {
    if (!kDeployAntenna) return false;
    if (TryAlgorithm(Antenna::kCommandAllDoorsAlgorithm1)) {
        return true;
    }
    if (TryAlgorithm(Antenna::kCommandAllDoorsAlgorithm2)) {
        return true;
    }
    return false;
}

bool Antenna::TryAlgorithm(Antenna::AntennaCommand command) const {
    WriteCommand(command);
    TaskUtils::SleepMilli(kWaitTime);

    uint8_t iterations = 0;

    while (IsHeatersOn() && (iterations < kMaxNumberOfIterations)) {
        TaskUtils::SleepMilli(kWaitTime);
        iterations++;
    }

    WriteCommand(kCommandModeAllOff);

    return IsDoorsOpen();
}

bool Antenna::ForceDeploy() const {
    if (!kDeployAntenna) return false;

    I2cIoExpander io_expander(bus, kAntennaOverRideIoExpanderAddress);

    // Perform manual override of primary burners
    io_expander.SetDirection(kPrimaryOverridePin, I2cIoExpander::kIoOutput);
    io_expander.SetPolarity(kPrimaryOverridePin, I2cIoExpander::kIoActiveHigh);

    io_expander.SetPin(kPrimaryOverridePin, true);
    TaskUtils::SleepMilli(kWaitTimeManualOverride);
    io_expander.SetPin(kPrimaryOverridePin, false);
    if (IsDoorsOpen()) {
        return true;
    }

    // Perform manual override of backup burners
    io_expander.SetDirection(kBackupOverridePin, I2cIoExpander::kIoOutput);
    io_expander.SetPolarity(kBackupOverridePin, I2cIoExpander::kIoActiveHigh);

    io_expander.SetPin(kBackupOverridePin, true);
    TaskUtils::SleepMilli(kWaitTimeManualOverride);
    io_expander.SetPin(kBackupOverridePin, false);
    if (IsDoorsOpen()) {
        return true;
    }
    return false;
}

bool Antenna::WriteCommand(Antenna::AntennaCommand command) const {
    byte write_buffer = static_cast<unsigned char>(command);
    return bus->PerformWriteTransaction(Antenna::kAddress, &write_buffer, 1);
}

void Antenna::InitAntenna(I2c *bus) {
    if (IsInitialised()) {
        return;
    }
    this->bus = bus;
    if (WriteCommand(kCommandModeAllOff)) {
        initialised = true;
    } else {
        initialised = false;
    }
}

AntennaMessage Antenna::GetStatus() const {
    uint8_t read_buffer[3];
    bus->PerformReadTransaction(kAddress, read_buffer, 3);
    // Bits at the door masks should be 1 when doors opened, 0 otherwise
    // Byte 2 shows active heaters, if 0 then all heaters off
    AntennaMessage status(
        kDoorOneMask & read_buffer[0], kDoorTwoMask & read_buffer[0],
        kDoorThreeMask & read_buffer[0], kDoorFourMask & read_buffer[0],
        read_buffer[1] > 0, kStateMask & read_buffer[0], read_buffer[1],
        read_buffer[2]);

    return status;
}

bool Antenna::IsDoorsOpen() const {
    AntennaMessage status = GetStatus();
    return (status.IsDoorOneOpen() && status.IsDoorTwoOpen() &&
            status.IsDoorThreeOpen() && status.IsDoorFourOpen());
}

bool Antenna::IsHeatersOn() const {
    AntennaMessage status = GetStatus();
    return status.IsHeatersOn();
}

bool Antenna::IsInitialised() const { return initialised; }

I2c *Antenna::GetBus() const { return bus; }

void Antenna::DeployAntenna() {
    if (!IsDoorsOpen()) {
        Log_info0("Trying safe deploy");
        SafeDeploy();
    }
    if (!IsDoorsOpen()) {
        Log_info0("Trying force deploy");
        ForceDeploy();
    }
    if (IsDoorsOpen()) {
        Log_info0("Antenna deployed");
    } else {
        Log_error0("Antenna failed to deploy");
    }
}
