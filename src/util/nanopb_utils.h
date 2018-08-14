#ifndef SRC_UTIL_NANOPB_UTILS_H_
#define SRC_UTIL_NANOPB_UTILS_H_

#include <external/etl/exception.h>
#include <external/nanopb/pb_decode.h>
#include <external/nanopb/pb_encode.h>
#include <src/messages/AccelerometerReading.pb.h>
#include <src/messages/BmsReadings.pb.h>
#include <src/messages/CurrentReading.pb.h>
#include <src/messages/GyroscopeReading.pb.h>
#include <src/messages/MagnetometerReading.pb.h>
#include <src/messages/TemperatureReading.pb.h>
#include <src/messages/Time.pb.h>
#include <src/messages/VoltageReading.pb.h>
#include <src/util/data_types.h>

#define NanopbEncode(NanopbMessageType)                              \
    NanopbUtils::Encode<NanopbMessageType, NanopbMessageType##_size, \
                        NanopbMessageType##_fields>

#define NanopbDecode(NanopbMessageType)                              \
    NanopbUtils::Decode<NanopbMessageType, NanopbMessageType##_size, \
                        NanopbMessageType##_fields>

class NanopbUtils {
   public:
    template <typename NanopbMessageType, uint16_t NanopbMessageType_size,
              const pb_field_t* NanopbMessageType_fields>
    static void Encode(byte buffer[], const NanopbMessageType& message) {
        pb_ostream_t stream =
            pb_ostream_from_buffer(buffer, NanopbMessageType_size);
        // TODO(dingbenjamin): Say which nanopb message failed to encode
        if (!pb_encode(&stream, NanopbMessageType_fields, &message)) {
            throw etl::exception("pb_encode failed", __FILE__, __LINE__);
        }
    }

    template <typename NanopbMessageType, uint16_t NanopbMessageType_size,
              const pb_field_t* NanopbMessageType_fields>
    static NanopbMessageType Decode(const byte buffer[]) {
        pb_istream_t stream =
            pb_istream_from_buffer(buffer, NanopbMessageType_size);
        NanopbMessageType message_struct;
        // TODO(dingbenjamin): Say which nanopb message failed to decode
        if (!pb_decode(&stream, NanopbMessageType_fields, &message_struct)) {
            throw etl::exception("pb_decode failed", __FILE__, __LINE__);
        }
        return message_struct;
    }

    // Conversion functions to Nanopb structs
    static TemperatureReading NanopbTemperature(double temperature, Time time);
    static VoltageReading NanopbVoltage(double voltage, Time time);
    static CurrentReading NanopbCurrent(double current, Time time);
    // Note that below function takes the same type arg as retval, it only adds
    // a timestamp as opposed to the other functions above which map between
    // structs
    static AccelerometerReading NanopbAccelerometer(AccelerometerReading accel,
                                                    Time time);
    static GyroscopeReading NanopbGyroscope(GyroscopeReading gyro, Time time);
    static BmsReadings NanopbBms(BmsReadings bms, Time time);
    // TODO(dingbenjamin): Decide whether to return nanopb values directly from
    // measurables
    static MagnetometerReading NanopbMagnetometer(MagnetometerReading mag,
                                                  Time time);
};

#endif  // SRC_UTIL_NANOPB_UTILS_H_
