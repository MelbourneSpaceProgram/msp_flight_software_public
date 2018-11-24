#include <src/board/MSP432E.h>
#include <src/board/i2c/i2c.h>
#include <src/config/satellite.h>
#include <src/messages/antenna_message.h>
#include <src/telecomms/antenna.h>
#include <src/util/tirtos_utils.h>
#include <ti/drivers/GPIO.h>
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
    burning_now = false;
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
    TirtosUtils::SleepMilli(kWaitTime);

    uint8_t iterations = 0;

    while (IsHeatersOn() && (iterations < kMaxNumberOfIterations)) {
        TirtosUtils::SleepMilli(kWaitTime);
        iterations++;
    }

    WriteCommand(kCommandModeAllOff);

    return IsDoorsOpen();
}

bool Antenna::ForceDeploy() const {
    if (!kDeployAntenna) return false;

    Log_info0("Trying override pin 1");
    GPIO_write(ANT_OVERRIDE_1, 1);
    TirtosUtils::SleepMilli(kWaitTimeManualOverride);
    GPIO_write(ANT_OVERRIDE_1, 0);
    if (IsDoorsOpen()) {
        Log_info0("Force deploy successful");
        return true;
    }

    Log_info0("Trying override pin 2");
    GPIO_write(ANT_OVERRIDE_2, 1);
    TirtosUtils::SleepMilli(kWaitTimeManualOverride);
    GPIO_write(ANT_OVERRIDE_2, 0);
    if (IsDoorsOpen()) {
        Log_info0("Force deploy successful");
        return true;
    }
    Log_info0("Force deploy finished");
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
    AntennaMessage status(read_buffer);

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

bool Antenna::IsBurning() const { return burning_now; };

bool Antenna::DeployAntenna() {
    if (!IsDoorsOpen()) {
        burning_now = true;
        Log_info0("Trying safe deploy");
        SafeDeploy();
    }
    if (!IsDoorsOpen()) {
        burning_now = true;
        Log_info0("Trying force deploy");
        ForceDeploy();
    }
    burning_now = false;
    if (IsDoorsOpen()) {
        Log_info0("Antenna deployed");
        return true;
    } else {
        Log_error0("Antenna failed to deploy");
        return false;
    }
}
