#include <src/sensors/i2c_sensors/rtc.h>

Rtc::Rtc(I2c* bus, int address, std::string id)
    : I2cSensor<RTime>(bus, address, id) {
    Rtc::bit_mask_map[kTimeRegisterSec] = kLowest7BitMask;
    Rtc::bit_mask_map[kTimeRegisterMin] = kLowest7BitMask;
    Rtc::bit_mask_map[kTimeRegisterHour] = kLowest6BitMask;
    Rtc::bit_mask_map[kTimeRegisterDate] = kLowest6BitMask;
    Rtc::bit_mask_map[kTimeRegisterMonth] = kLowest5BitMask;
    Rtc::bit_mask_map[kTimeRegisterYear] = kLowest8BitMask;
}
// TODO(naverill): transition data type of address to uint8_t
// TODO(naverill): validate time is returned in 24h time

bool Rtc::TakeReading() {
    RTime real_time;
    byte read_buffer[kReadBufLen];

    real_time.sec = GetUnitTime(kTimeRegisterSec, read_buffer);
    real_time.min = GetUnitTime(kTimeRegisterMin, read_buffer);
    real_time.hour = GetUnitTime(kTimeRegisterHour, read_buffer);
    real_time.date = GetUnitTime(kTimeRegisterDate, read_buffer);
    real_time.month = GetUnitTime(kTimeRegisterMonth, read_buffer);
    real_time.year = kCurrCentury + GetUnitTime(kTimeRegisterYear, read_buffer);

    if (ValidTime(real_time)) {
        SetSensorData(real_time);
        return true;
    }
    return false;
}

byte Rtc::GetUnitTime(byte time_register, byte read_buffer[]) {
    byte write_buffer[1] = {time_register};
    byte bit_mask;

    RTCMaskMap::const_iterator iter = bit_mask_map.find(time_register);

    if (iter != bit_mask_map.end()) {
        bit_mask = iter->second;
    } else {
        bit_mask = kLowest8BitMask;
    }

    bus->PerformTransaction(address, read_buffer, kReadBufLen, write_buffer, 1);
    return BCDToBinary(read_buffer[0] & bit_mask);
}

byte Rtc::BCDToBinary(byte output) {
    return 10 * ((output & kHighest4BitMask) >> 4) + (output & kLowest4BitMask);
}

void Rtc::SetSensorData(RTime reading) { this->reading = reading; }

bool Rtc::ValidTime(RTime time) {
    byte s = time.sec;
    byte min = time.min;
    byte hour = time.hour;
    byte d = time.date;
    byte m = time.month;
    uint16_t y = time.year;

    if (!(s < 60)) return false;
    if (!(min < 60)) return false;
    if (!(hour < 24)) return false;
    if (!(1 <= m && m <= 12)) return false;
    if (!(1 <= d && d <= 31)) return false;
    if ((d == 30) && (m == Feb || m == Apr || m == Jun || m == Sep || m == Nov))
        return false;
    if ((d == 30) && (m == Feb)) return false;
    if ((m == Feb) && (d == 29) && (y % 4 != 0)) return false;
    if ((m == Feb) && (d == 29) && (y % 400 == 0)) return true;
    if ((m == Feb) && (d == 29) && (y % 100 == 0)) return false;
    if ((m == Feb) && (d == 29) && (y % 4 == 0)) return true;
    return true;
}
