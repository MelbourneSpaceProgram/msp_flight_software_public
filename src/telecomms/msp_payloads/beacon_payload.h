#ifndef SRC_TELECOMMS_MSP_PAYLOADS_BEACON_PAYLOAD_H_
#define SRC_TELECOMMS_MSP_PAYLOADS_BEACON_PAYLOAD_H_

#include <external/etl/array.h>
#include <src/messages/Time.pb.h>
#include <src/messages/message.h>
#include <src/sensors/measurable_manager.h>
#include <src/telecomms/msp_payloads/transmit_payload.h>
#include <src/util/data_types.h>
#include <stdint.h>

class BeaconPayload : public TransmitPayload {
   public:
    BeaconPayload();
    SerialisedMessage SerialiseTo(byte* serial_buffer) const;
    uint16_t GetSerialisedSize() const;
    uint8_t GetPayloadCode() const;

    float com_out_i1;
    float com_out_v1;
    float com_out_i2;
    float com_out_v2;
    float com_t2;
    float eps_adc_bat_v1;
    float eps_load_i1;
    float eps_adc_bat_v2;
    float eps_boost_in_i2;
    float eps_rail1;
    float eps_rail2;
    float eps_top_panel_v;
    float eps_top_panel_i;
    float eps_t1;
    float eps_t2;
    float x_pos_v;
    float x_pos_i;
    float x_pos_t1;
    uint16_t x_pos_rad;
    float y_pos_v;
    float y_pos_i;
    float y_pos_t1;
    float x_neg_v;
    float x_neg_i;
    float x_neg_t1;
    float y_neg_v;
    float y_neg_i;
    float y_neg_t1;
    float z_neg_v;
    float z_neg_i;
    float z_neg_t1;
    float z_pos_t;
    int16_t util_ntc_v1;  // TODO(naverill) clarify data type
    int16_t util_ntc_v2;  // TODO(naverill) clarify data type
    int16_t util_heat_v;  // TODO(naverill) clarify data type
    float util_t;
    float fs_torquer_xi;
    float fs_torquer_yi;
    float fs_torquer_zi;
    float fs_hb_xt;
    float fs_hb_yt;
    float fs_hb_zt;
    uint16_t fs_rad1;  // TODO(naverill) clarify data type
    Time cdh_time;
    int16_t cdh_t;
    uint32_t com_rx_bytes;
    uint32_t com_tx_bytes;
    uint8_t com_antenna_flags;
    uint16_t com_lithium_ops;
    uint8_t com_lithium_rssi;
    int16_t com_lithium_t;
    Time com_lithium_time;
    int16_t sw_fs_angular_velocity1;  // TODO(naverill) clarify data type
    int16_t sw_fs_angular_velocity2;  // TODO(naverill) clarify data type
    int16_t sw_fs_angular_velocity3;  // TODO(naverill) clarify data type
    int16_t sw_fs_attitude_cov;
    int16_t sw_fs_attitude_distance;
    int16_t sw_fs_attitude_quaternion1;  // TODO(naverill) clarify data type
    int16_t sw_fs_attitude_quaternion2;  // TODO(naverill) clarify data type
    int16_t sw_fs_attitude_quaternion3;  // TODO(naverill) clarify data type
    int16_t sw_fs_attitude_quaternion4;  // TODO(naverill) clarify data type
    int16_t sw_fs_control1;
    int16_t sw_fs_control2;
    int16_t sw_fs_control3;
    uint16_t sw_fs_health_flags;
    uint16_t sw_fs_attitude_flags;
    uint16_t sw_fs_flags;
    uint16_t sw_fs_location;
    uint16_t sw_fs_ir;
    Time sw_cdh_last_reboot;
    uint16_t sw_cdh_memory;
    uint16_t sw_cdh_memory_available;
    uint16_t sw_cdh_mcu1;
    uint16_t sw_mcu_reset_count1;
    uint16_t sw_sequence;
    const char* comms_outreach;  // TODO(dingbenjamin): Convert to etl::string

    static constexpr uint8_t kFsHealthFlagByteCount = 20;
    static constexpr uint8_t kFsFsAttitudeFlagByteCount = 2;
    static constexpr uint8_t kSwFsLocationFloatCount = 2;
    static constexpr uint8_t kOutreachMessageSize = 25;

    static constexpr uint8_t kVoltageUpperBound =
        20;  // assumed [-20, 20] value range
    static constexpr uint8_t kCurrentUpperBound =
        20;  // assumed [-20, 20] value range
    static constexpr uint8_t kTempUpperBound =
        128;  // assumed [-128, 128] value range
    static constexpr int16_t kInvalidScaled16BitValue = INT16_MIN;

    // TODO(dingbenjamin): Make these private
    static int16_t ScaleArbitraryInt16(float data, uint16_t abs_max,
                                       float invalid_value = kInvalidDouble);
    static int16_t ScaleCurrent(float data);
    static int16_t ScaleVoltage(float data);
    static int16_t ScaleTemp(float data);
    static float ConstrainToRange(float data, uint16_t abs_max);

   private:
    template <typename T>
    static T ReadCachedMeasurable(uint16_t measurable_id) {
        return MeasurableManager::GetInstance()->ReadNanopbMeasurable<T>(
            measurable_id, 0, true);
    }

    double ReadCachedTemperature(uint16_t measurable_id);
    double ReadCachedVoltage(uint16_t measurable_id);
    double ReadCachedCurrent(uint16_t measurable_id);
};

#endif  // SRC_TELECOMMS_MSP_PAYLOADS_BEACON_PAYLOAD_H_
