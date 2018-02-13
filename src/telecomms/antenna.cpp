#include <src/telecomms/antenna.h>
#include <ti/sysbios/knl/Clock.h>

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

// Algorithm 1 from "Antenna Deployment tester.pdf" of CS1-DD-544
void Antenna::SafeDeploy() const {
    if (!initialised) {
        return;
    }

    if (IsDoorsOpen()) {
        return;
    }

    if (TryHeater(Antenna::kCommandModeAutoHeat, Antenna::kWaitTimeShort)) {
        return;
    }

    if (TryHeater(Antenna::kCommandModeAutoBackupHeat,
                  Antenna::kWaitTimeShort)) {
        return;
    }

    if (TryHeater(Antenna::kCommandModeAutoCombinedHeat,
                  Antenna::kWaitTimeShort)) {
        return;
    }

    // TODO(dingbenjamin): Find a better way to get around passing const
    // commands as args without an explicit copy
    byte copy_command_mode_off = Antenna::kCommandModeAllOff;

    bus->PerformWriteTransaction(Antenna::kAddress, &copy_command_mode_off, 1);
}

// Algorithm 2 from "Antenna Deployment tester.pdf" of CS1-DD-544
void Antenna::ForceDeploy() const {
    if (initialised) {
        TryHeater(Antenna::kCommandModeHeaterOneTwoOnDefTime,
                  Antenna::kWaitTimeLong);
    }
}

bool Antenna::TryHeater(const byte heat_command, uint32_t wait_time) const {
    // TODO(dingbenjamin): Find a better way to get around passing const
    // commands as args without an explicit copy
    byte copy_heat_command = heat_command;
    byte copy_command_mode_off = Antenna::kCommandModeAllOff;

    if (initialised) {
        bus->PerformWriteTransaction(Antenna::kAddress, &copy_heat_command, 1);
        Task_sleep(wait_time * 1000 / Clock_tickPeriod);
        if (IsDoorsOpen()) {
            bus->PerformWriteTransaction(Antenna::kAddress,
                                         &copy_command_mode_off, 1);
            // TODO(wschuetz): Remove Task_sleep on overflow bug fix
            Task_sleep(100);
            return true;
        }
    }
    return false;
}

void Antenna::InitAntenna(I2c *bus) {
    if (IsInitialised()) {
        return;
    }
    this->bus = bus;
    if (bus->PerformWriteTransaction(Antenna::kAddress,
                                     Antenna::kCommandModeAllOff, 1)) {
        initialised = true;
    } else {
        initialised = false;
    }
}

AntennaMessage Antenna::GetStatus() const {
    uint8_t read_buffer;
    bus->PerformReadTransaction(kAddress, &read_buffer, 1);
    // Bit at the zero mask should ALWAYS be 0 as specified by data sheet
    // Bits at the door masks should be 1 when doors opened, 0 otherwise
    AntennaMessage status(
        (kZeroMask ^ read_buffer) & kZeroMask, kDoorOneMask & read_buffer,
        kDoorTwoMask & read_buffer, kDoorThreeMask & read_buffer,
        kDoorFourMask & read_buffer, kStateMask & read_buffer);
    // TODO(wschuetz): Remove Task_sleep on overflow bug fix
    Task_sleep(100);
    return status;
}

bool Antenna::IsDoorsOpen() const {
    AntennaMessage status = GetStatus();
    return (status.IsDoorOneOpen() && status.IsDoorTwoOpen() &&
            status.IsDoorThreeOpen() && status.IsDoorFourOpen());
}

bool Antenna::IsInitialised() const { return initialised; }

I2c *Antenna::GetBus() const { return bus; }
