#include <src/sensors/i2c_sensors/adc.h>
#include <src/util/task_utils.h>
#include <src/util/msp_exception.h>

const double Adc::AdcGainAmplifierFullScaleRangeVoltages[6] = {
    kAdcVoltage6v144, kAdcVoltage4v096, kAdcVoltage2v048,
    kAdcVoltage1v024, kAdcVoltage0v512, kAdcVoltage0v256};

Adc::Adc(const I2c* bus, int address, const I2cMultiplexer* multiplexer,
         I2cMultiplexer::MuxChannel channel)
    : I2cDevice(bus, address, multiplexer, channel),
      operational_status(kAdcDefaultOperationalStatus),
      mux_mode(kAdcDefaultMuxMode),
      gain_amplifier_level(kAdcDefaultGainAmplifierLevel),
      operating_mode(kAdcDefaultOperatingMode),
      data_rate(kAdcDefaultDataRate),
      comparator_mode(kAdcDefaultComparatorMode),
      comparator_polarity(kAdcDefaultComparatorPolarity),
      latching_comparator(kAdcDefaultLatchingComparator),
      comparator_queue(kAdcDefaultComparatorQueue) {
    try {
        SetAdcGainAmplifierFullScaleRange();
        SetConfiguration();
    } catch (MspException& e) {
		MspException::LogException(e, kAdcCatch);
        // TODO(dingbenjamin): Not sure what we can really do about this
    }
}

bool Adc::SetConfiguration() {
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

    if (!PerformWriteTransaction(address, package, 3)) {
        return false;
    }

    return true;
}

bool Adc::ReadConversionRegister(etl::array<byte, 2>& read_buffer) {
    SelectRegister(kAdcConversionRegisterLocation);
    TaskUtils::SleepMilli(
        10);  // Ensure we give the ADC sufficient time to sample
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
    PerformWriteTransaction(address, &register_address, 1);
}

double Adc::TakeI2cReading(void) {
    etl::array<byte, 2> read_buffer;
    bool reading_successful = ReadConversionRegister(read_buffer);
    if (reading_successful) {
        return ConvertReadingToVoltage(read_buffer);
    } else {
        throw MspException("Failed ADC Reading", kAdcFail, __FILE__, __LINE__);
    }
}

double Adc::ConvertReadingToVoltage(etl::array<byte, 2>& read_buffer) {
    // extract full 16-bit reading from the read_buffer
    int16_t binary_reading = ((static_cast<uint16_t>(read_buffer.at(0))) << 8) |
                             static_cast<uint16_t>(read_buffer.at(1));
    // convert reading to voltage (as per datasheet)
    return 2.0 * binary_reading * gain_amplifier_full_scale_range /
           (1 << kAdcRegisterNumBits);
}

bool Adc::ReadFromCurrentRegister(etl::array<byte, 2>& read_buffer) {
    // TODO(dingbenjamin): Remove i2c buffer once i2c driver changed to use etl
    byte i2c_buffer[2] = {0};
    if (PerformReadTransaction(address, i2c_buffer, 2)) {
        read_buffer.at(0) = i2c_buffer[0];
        read_buffer.at(1) = i2c_buffer[1];
        return true;
    } else {
        return false;
    }
}

AdcOperationalStatus Adc::GetOperationalStatus() const {
    return operational_status;
}

bool Adc::SetOperationalStatus(AdcOperationalStatus operational_status) {
    this->operational_status = operational_status;
    return SetConfiguration();
}

AdcMuxMode Adc::GetMuxMode() const { return mux_mode; }

bool Adc::SetMuxMode(AdcMuxMode mux_mode) {
    this->mux_mode = mux_mode;
    return SetConfiguration();
}

AdcGainAmplifierLevel Adc::GetGainAmplifierLevel() const {
    return gain_amplifier_level;
}

bool Adc::SetGainAmplifierLevel(AdcGainAmplifierLevel gain_amplifier_level) {
    this->gain_amplifier_level = gain_amplifier_level;
    SetAdcGainAmplifierFullScaleRange();
    return SetConfiguration();
}

AdcOperatingMode Adc::GetOperatingMode() const { return operating_mode; }

bool Adc::SetOperatingMode(AdcOperatingMode operating_mode) {
    this->operating_mode = operating_mode;
    return SetConfiguration();
}

AdcDataRate Adc::GetDataRate() const { return data_rate; }

bool Adc::SetDataRate(AdcDataRate data_rate) {
    this->data_rate = data_rate;
    return SetConfiguration();
}

AdcComparatorMode Adc::GetComparatorMode() const { return comparator_mode; }

bool Adc::SetComparatorMode(AdcComparatorMode comparator_mode) {
    this->comparator_mode = comparator_mode;
    return SetConfiguration();
}

AdcComparatorPolarity Adc::GetComparatorPolarity() const {
    return comparator_polarity;
}

bool Adc::SetComparatorPolarity(AdcComparatorPolarity comparator_polarity) {
    this->comparator_polarity = comparator_polarity;
    return SetConfiguration();
}

AdcLatchingComparatorMode Adc::GetLatchingComparatorMode() const {
    return latching_comparator;
}

bool Adc::SetLatchingComparatorMode(
    AdcLatchingComparatorMode latching_comparator) {
    this->latching_comparator = latching_comparator;
    return SetConfiguration();
}

AdcComparatorQueueMode Adc::GetComparatorQueue() const {
    return comparator_queue;
}

bool Adc::SetComparatorQueue(AdcComparatorQueueMode comparator_queue) {
    this->comparator_queue = comparator_queue;
    return SetConfiguration();
}

double Adc::GetAdcGainAmplifierFullScaleRange() const {
    return gain_amplifier_full_scale_range;
}

bool Adc::SetAdcGainAmplifierFullScaleRange() {
    gain_amplifier_full_scale_range =
        AdcGainAmplifierFullScaleRangeVoltages[static_cast<uint8_t>(
            gain_amplifier_level)];
    return SetConfiguration();
}
