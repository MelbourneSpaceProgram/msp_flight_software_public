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
	AntennaStatus com_antenna_status;
    uint8_t com_lithium_rssi;
    int16_t com_lithium_t; 

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

    float util_t;

    float fs_torquer_xi;
    float fs_torquer_yi;
    float fs_torquer_zi;
    float fs_hb_xt;
    float fs_hb_yt;
    float fs_hb_zt;
    uint16_t fs_rad1;  // TODO(dingbenjamin): Delete or implement
    float fs_angular_velocity1;  
    float fs_angular_velocity2; 
    float fs_angular_velocity3;
    float fs_magno1;  
    float fs_magno2; 
    float fs_magno3;
    int16_t fs_attitude_cov; // Nice to have
    int16_t fs_attitude_quaternion1;  // TODO(naverill) clarify data type
    int16_t fs_attitude_quaternion2;  // TODO(naverill) clarify data type
    int16_t fs_attitude_quaternion3;  // TODO(naverill) clarify data type
    int16_t fs_attitude_quaternion4;  // TODO(naverill) clarify data type
    byte fs_flags;  // You get a byte of flags

    Time cdh_time; 
    int16_t cdh_t;

    uint16_t sw_sequence;
	Time sw_last_reboot;
	uint8_t sw_idle_load; // TODO(dingbenjamin): Make measurable 
	uint8_t sw_cpu_load; // TODO(dingbenjamin): Make measurable 
	uint16_t sw_num_resets
	byte sw_last_reset_cause;
	byte sw_last_reset_message;
	uint16_t sw_num_exceptions;
	byte sw_flags;
    const char* sw_outreach;  // TODO(dingbenjamin): Convert to etl::string

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
	static constexpr uint16_t kGyroUpperBound = 2000;
	static constexpr uint16_t kMagnoUpperBound = 4800
    static constexpr int16_t kInvalidScaled16BitValue = INT16_MIN;

    // TODO(dingbenjamin): Make these private
    static int16_t ScaleArbitraryInt16(float data, uint16_t abs_max,
                                       float invalid_value = kInvalidDouble);
    static int16_t ScaleCurrent(float data);
    static int16_t ScaleVoltage(float data);
    static int16_t ScaleTemp(float data);
	static int16_t ScaleGyro(float data);
	static int16_t ScaleMagno(float data);
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
