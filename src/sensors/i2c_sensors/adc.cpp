#include <src/sensors/i2c_sensors/adc.h>

const double Adc::AdcGainAmplifierFullScaleRangeVoltages[6] = {
    kAdcVoltage6v144, kAdcVoltage4v096, kAdcVoltage2v048,
    kAdcVoltage1v024, kAdcVoltage0v512, kAdcVoltage0v256};

Adc::Adc(const I2c* bus, int address)
    : I2cSensor<double>(bus, address),
      operational_status(kAdcDefaultOperationalStatus),
      mux_mode(kAdcDefaultMuxMode),
      gain_amplifier_level(kAdcDefaultGainAmplifierLevel),
      operating_mode(kAdcDefaultOperatingMode),
      data_rate(kAdcDefaultDataRate),
      comparator_mode(kAdcDefaultComparatorMode),
      comparator_polarity(kAdcDefaultComparatorPolarity),
      latching_comparator(kAdcDefaultLatchingComparator),
      comparator_queue(kAdcDefaultComparatorQueue) {
    inputs.fill(NULL);
    SetAdcGainAmplifierFullScaleRange();
    SetConfiguration();
}

void Adc::SetConfiguration() {
    byte package[3];

    package[0] = Adc::kAdcConfigurationRegisterLocation;
    package[1] =
        (static_cast<byte>(operational_status)
         << kAdcOperationalStatusBitShift) +
        (static_cast<byte>(mux_mode) << kAdcInputStateBitShift) +
        (static_cast<byte>(gain_amplifier_level) << kAdcGainAmplifierBitShift) +
        (static_cast<byte>(operating_mode) << kAdcOperatingModeBitShift);
    package[2] =
        (static_cast<byte>(data_rate) << kAdcDataRateBitShift) +
        (static_cast<byte>(comparator_mode) << kAdcComparatorModeBitShift) +
        (static_cast<byte>(comparator_polarity)
         << kAdcComparatorPolarityBitShift) +
        (static_cast<byte>(latching_comparator)
         << kAdcLatchingComparatorBitShift) +
        (static_cast<byte>(comparator_queue) << kAdcComparatorQueueBitShift);

    bus->PerformWriteTransaction(address, package, 3);
}

bool Adc::ReadConversionRegister(etl::array<byte, 2>& read_buffer) {
    SelectRegister(kAdcConversionRegisterLocation);
    return ReadFromCurrentRegister(read_buffer);
}

bool Adc::ReadConfigurationRegister(etl::array<byte, 2>& read_buffer) {
    SelectRegister(kAdcConfigurationRegisterLocation);
    return ReadFromCurrentRegister(read_buffer);
}

bool Adc::ReadLoThreshRegister(etl::array<byte, 2>& read_buffer) {
    SelectRegister(kAdcLoThreshRegisterLocation);
    return ReadFromCurrentRegister(read_buffer);
}

bool Adc::ReadHiThreshRegister(etl::array<byte, 2>& read_buffer) {
    SelectRegister(kAdcHiThreshRegisterLocation);
    return ReadFromCurrentRegister(read_buffer);
}

void Adc::SelectRegister(byte register_address) {
    bus->PerformWriteTransaction(address, &register_address, 1);
}

bool Adc::TakeReading(void) {
    etl::array<byte, 2> read_buffer;
    bool reading_successful = ReadConversionRegister(read_buffer);
    if (reading_successful) {
        reading = ConvertReadingToVoltage(read_buffer);
        NotifyObservers();
    }
    return reading_successful;
}

double Adc::ConvertReadingToVoltage(etl::array<byte, 2>& read_buffer) {
    // extract full 16-bit reading from the read_buffer
    uint16_t binary_reading;
    binary_reading = ((static_cast<uint16_t>(read_buffer.at(0))) << 8) |
                     static_cast<uint16_t>(read_buffer.at(1));
    // convert reading to voltage (as per datasheet)
    return 1.0 * binary_reading * gain_amplifier_full_scale_range /
           (1 << kAdcRegisterNumBits);
}

bool Adc::ReadFromCurrentRegister(etl::array<byte, 2>& read_buffer) {
    // TODO(dingbenjamin): Remove i2c buffer once i2c driver changed to use etl
    byte i2c_buffer[2];
    i2c_buffer[0] = read_buffer.at(0);
    i2c_buffer[1] = read_buffer.at(1);

    if (bus->PerformReadTransaction(address, i2c_buffer, 2)) {
        read_buffer.at(0) = i2c_buffer[0];
        read_buffer.at(1) = i2c_buffer[1];
        reading = 0;
        return true;
    } else {
        return false;
    }
}

AdcOperationalStatus Adc::GetOperationalStatus() const {
    return operational_status;
}

void Adc::SetOperationalStatus(AdcOperationalStatus operational_status) {
    this->operational_status = operational_status;
    SetConfiguration();
}

AdcMuxMode Adc::GetMuxMode() const { return mux_mode; }

void Adc::SetMuxMode(AdcMuxMode mux_mode) {
    this->mux_mode = mux_mode;
    SetConfiguration();
}

AdcGainAmplifierLevel Adc::GetGainAmplifierLevel() const {
    return gain_amplifier_level;
}

void Adc::SetGainAmplifierLevel(AdcGainAmplifierLevel gain_amplifier_level) {
    this->gain_amplifier_level = gain_amplifier_level;
    SetAdcGainAmplifierFullScaleRange();
    SetConfiguration();
}

AdcOperatingMode Adc::GetOperatingMode() const { return operating_mode; }

void Adc::SetOperatingMode(AdcOperatingMode operating_mode) {
    this->operating_mode = operating_mode;
    SetConfiguration();
}

AdcDataRate Adc::GetDataRate() const { return data_rate; }

void Adc::SetDataRate(AdcDataRate data_rate) {
    this->data_rate = data_rate;
    SetConfiguration();
}

AdcComparatorMode Adc::GetComparatorMode() const { return comparator_mode; }

void Adc::SetComparatorMode(AdcComparatorMode comparator_mode) {
    this->comparator_mode = comparator_mode;
    SetConfiguration();
}

AdcComparatorPolarity Adc::GetComparatorPolarity() const {
    return comparator_polarity;
}

void Adc::SetComparatorPolarity(AdcComparatorPolarity comparator_polarity) {
    this->comparator_polarity = comparator_polarity;
    SetConfiguration();
}

AdcLatchingComparatorMode Adc::GetLatchingComparatorMode() const {
    return latching_comparator;
}

void Adc::SetLatchingComparatorMode(
    AdcLatchingComparatorMode latching_comparator) {
    this->latching_comparator = latching_comparator;
    SetConfiguration();
}

AdcComparatorQueueMode Adc::GetComparatorQueue() const {
    return comparator_queue;
}

void Adc::SetComparatorQueue(AdcComparatorQueueMode comparator_queue) {
    this->comparator_queue = comparator_queue;
    SetConfiguration();
}

double Adc::GetAdcGainAmplifierFullScaleRange() const {
    return gain_amplifier_full_scale_range;
}

void Adc::SetAdcGainAmplifierFullScaleRange() {
    gain_amplifier_full_scale_range =
        AdcGainAmplifierFullScaleRangeVoltages[static_cast<uint8_t>(
            gain_amplifier_level)];
}
