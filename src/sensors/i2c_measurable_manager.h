#ifndef SRC_SENSORS_I2C_MEASURABLE_MANAGER_H_
#define SRC_SENSORS_I2C_MEASURABLE_MANAGER_H_

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

    template <class T>
    I2cMeasurable<T> *GetMeasurable(uint16_t id) {
        I2cMeasurable<T> *i2c_measurable =
            dynamic_cast<I2cMeasurable<T> *>(measurables.at(id));
        if (i2c_measurable == NULL) {
            etl::exception e("Cannot cast to specified measurable type",
                             __FILE__, __LINE__);
            throw e;
        }
        return i2c_measurable;
    }

    template <class T>
    T ReadI2cMeasurable(uint16_t id, uint64_t max_cache_time_milliseconds,
                        bool always_use_cached = false) {
        try {
            Measurable *measurable = measurables.at(id);
            I2cMeasurable<T> *i2c_measurable =
                dynamic_cast<I2cMeasurable<T> *>(measurable);
            if (i2c_measurable == NULL) {
                etl::exception e("Cannot cast to specified measurable type",
                                 __FILE__, __LINE__);
                throw e;
            }

            Time timestamp = i2c_measurable->GetTimestamp();
            Time earliest_acceptable_time = SatelliteTimeSource::GetTime();
            earliest_acceptable_time.timestamp_millis_unix_epoch -=
                max_cache_time_milliseconds;

            if (!always_use_cached &&
                (max_cache_time_milliseconds == 0 || !timestamp.is_valid ||
                 SatelliteTimeSource::TimeDifferenceMilli(
                     timestamp, earliest_acceptable_time) > 0)) {
                // TODO(dingbenjamin): Lock the relevant bus/sensor with a
                // semaphore
                i2c_measurable->TakeReading();
            }

            // TODO(dingbenjamin): Account for error if no reading exists yet
            return i2c_measurable->GetReading();
        } catch (etl::exception &e) {
            // TODO(dingbenjamin): Handle the exception
            // Possible exceptions are bad cast or nonexistent sensor
            throw e;
        }
    }

    // TODO(dingbenjamin): Make this not a template
    template <class T>
    Time GetMeasurableTimeStamp(uint16_t id) {
        try {
            Measurable *measurable = measurables.at(id);
            I2cMeasurable<T> *i2c_measurable =
                dynamic_cast<I2cMeasurable<T> *>(measurable);
            if (i2c_measurable == NULL) {
                etl::exception e("Cannot cast to specified measurable type",
                                 __FILE__, __LINE__);
                throw e;
            }
            return i2c_measurable->GetTimestamp();
        } catch (etl::exception &e) {
            // TODO(dingbenjamin): Handle the exception
            // Possible exceptions are bad cast or nonexistent sensor
            throw e;
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
    void AddBmsReadingsMeasurable(MeasurableId id, Bms *bms);
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
