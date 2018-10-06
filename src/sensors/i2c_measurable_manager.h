#ifndef SRC_SENSORS_I2C_MEASURABLE_MANAGER_H_
#define SRC_SENSORS_I2C_MEASURABLE_MANAGER_H_

#include <src/config/unit_tests.h>
#include <src/messages/Time.pb.h>
#include <src/sensors/i2c_sensors/adc.h>
#include <src/sensors/i2c_sensors/measurables/i2c_measurable.h>
#include <src/sensors/measurable_id.h>
#include <src/util/matrix.h>

class I2cMultiplexer;
class I2c;
class Measurable;
class Rtc;
class Mcp9808;
class Bms;
class MPU9250MotionTracker;
class BmsBatteryTemperatureMeasurable;

class I2cMeasurableManager {
   public:
    static I2cMeasurableManager *GetInstance();
    void Init(const I2c *bus_a, const I2c *bus_b, const I2c *bus_c,
              const I2c *bus_d);

    template <class TimestampedNanopbType>
    I2cMeasurable<TimestampedNanopbType> *GetMeasurable(uint16_t id) {
        I2cMeasurable<TimestampedNanopbType> *i2c_measurable =
            dynamic_cast<I2cMeasurable<TimestampedNanopbType> *>(
                measurables.at(id));
        if (i2c_measurable == NULL) {
            etl::exception e("Cannot cast to specified measurable type",
                             __FILE__, __LINE__);
            throw e;
        }
        return i2c_measurable;
    }

    template <class TimestampedNanopbType>
    TimestampedNanopbType ReadI2cMeasurable(uint16_t id, uint64_t max_cache_ms,
                                            bool always_use_cached = false) {
        Measurable *measurable = measurables.at(id);
        I2cMeasurable<TimestampedNanopbType> *i2c_measurable =
            dynamic_cast<I2cMeasurable<TimestampedNanopbType> *>(measurable);
        if (i2c_measurable == NULL) {
            throw etl::exception("Cannot cast to specified measurable type",
                                 __FILE__, __LINE__);
        }

        try {
            uint64_t earliest_acceptable_ms;
            Time time = SatelliteTimeSource::GetTime();
            if (time.is_valid) {
                earliest_acceptable_ms = time.timestamp_ms - max_cache_ms;
            } else {
                if (kI2cAvailable) {
                    Log_error0(
                        "Failed to acquire time for cache, taking new sensor "
                        "reading");
                }
                earliest_acceptable_ms = 0;
            }

            uint64_t cache_time_ms = i2c_measurable->GetReading().timestamp_ms;
            if (i2c_measurable->first_reading ||
                (!always_use_cached &&
                 (cache_time_ms >= earliest_acceptable_ms))) {
                // TODO(dingbenjamin): Lock the relevant bus/sensor with a
                // semaphore
                i2c_measurable->TakeReading();
                i2c_measurable->first_reading = false;
            }
            return i2c_measurable->GetReading();
        } catch (etl::exception &e) {
            return i2c_measurable->failure_reading;
        }
    }

   private:
    I2cMeasurableManager();

    void InitTelecomms(const I2cMultiplexer *mux_a);
    void InitPower(const I2cMultiplexer *mux_a);
    void InitFlightSystems(const I2cMultiplexer *mux_a);
    void InitUtilities(const I2cMultiplexer *mux_c);
    void InitCdh(const I2cMultiplexer *mux_a);
    void InitSolarPanels(const I2cMultiplexer *mux_c);

    void AddVoltage(MeasurableId id, Adc *adc, AdcMuxMode line,
                    float scaling_factor);
    void AddCurrent(MeasurableId id, Adc *adc, AdcMuxMode line,
                    float scaling_factor, float zero_bias_point);
    void AddTemperature(MeasurableId id, Mcp9808 *temp_sensor);
    void AddBmsDieTempMeasurable(MeasurableId id, Bms *temp_sensor);
    BmsBatteryTemperatureMeasurable *AddBmsBatteryTempMeasurable(
        MeasurableId id, Bms *temp_sensor);
    void AddBmsSettingsMeasurable(MeasurableId id, Bms *bms);
    void AddBmsChargingInfoMeasurable(MeasurableId id, Bms *bms);
    void AddBmsOperationValuesMeasurable(MeasurableId id, Bms *bms);
    void AddBmsTemperatureMeasurable(MeasurableId id, Bms *bms);
    void AddBmsVoltagesMeasurable(MeasurableId id, Bms *bms);
    void AddBmsCurrentsMeasurable(MeasurableId id, Bms *bms);
    void AddImuGyrometerMeasurable(MeasurableId id,
                                   MPU9250MotionTracker *imu_sensor);
    void AddImuAcceleromterMeasurable(MeasurableId id,
                                      MPU9250MotionTracker *imu_sensor);
    void AddImuTemperatureMeasurable(MeasurableId id,
                                     MPU9250MotionTracker *imu_sensor);
    void AddImuMagnetometerMeasurable(MeasurableId id,
                                      MPU9250MotionTracker *imu_sensor,
                                      const Matrix &frame_mapping,
                                      const Matrix &initial_biases,
                                      const Matrix &initial_scale_factors);
    void CheckValidId(MeasurableId id);

    static I2cMeasurableManager *instance;
    static const uint16_t kMaxMeasurables = 400;

    const I2c *bus_a;
    const I2c *bus_b;
    const I2c *bus_c;
    const I2c *bus_d;

    etl::array<Measurable *, kMaxMeasurables> measurables;
};

#endif  // SRC_SENSORS_I2C_MEASURABLE_MANAGER_H_
