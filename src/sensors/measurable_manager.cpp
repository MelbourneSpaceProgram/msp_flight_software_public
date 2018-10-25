#include <external/etl/exception.h>
#include <src/board/i2c/bms/bms.h>
#include <src/config/satellite.h>
#include <src/sensors/i2c_sensors/adc.h>
#include <src/sensors/i2c_sensors/mcp9808.h>
#include <src/sensors/i2c_sensors/measurables/bms_battery_temperature_measurable.h>
#include <src/sensors/i2c_sensors/measurables/bms_charging_info_measurable.h>
#include <src/sensors/i2c_sensors/measurables/bms_currents_measurable.h>
#include <src/sensors/i2c_sensors/measurables/bms_die_temperature_measurable.h>
#include <src/sensors/i2c_sensors/measurables/bms_operation_values_measurable.h>
#include <src/sensors/i2c_sensors/measurables/bms_settings_measurable.h>
#include <src/sensors/i2c_sensors/measurables/bms_temperature_measurable.h>
#include <src/sensors/i2c_sensors/measurables/bms_voltages_measurable.h>
#include <src/sensors/i2c_sensors/measurables/current_measurable.h>
#include <src/sensors/i2c_sensors/measurables/imu_accelerometer_measurable.h>
#include <src/sensors/i2c_sensors/measurables/imu_gyroscope_measurable.h>
#include <src/sensors/i2c_sensors/measurables/imu_magnetometer_measurable.h>
#include <src/sensors/i2c_sensors/measurables/imu_temperature_measurable.h>
#include <src/sensors/i2c_sensors/measurables/temperature_measurable.h>
#include <src/sensors/i2c_sensors/measurables/voltage_measurable.h>
#include <src/sensors/i2c_sensors/mpu9250_motion_tracker.h>
#include <src/sensors/magnetometer_calibration.h>
#include <src/sensors/measurable_id.h>
#include <src/sensors/measurable_manager.h>
#include <src/util/etl_utils.h>

MeasurableManager *MeasurableManager::instance = NULL;

MeasurableManager *MeasurableManager::GetInstance() {
    if (instance == NULL) {
        instance = new MeasurableManager();
    }
    return instance;
}

MeasurableManager::MeasurableManager()
    : telecomms_initialised(false),
      power_initialised(false),
      flight_systems_initialised(false),
      utilities_initialised(false),
      cdh_initialised(false),
      x_pos_initialised(false),
      y_pos_initialised(false),
      z_pos_initialised(false),
      x_neg_initialised(false),
      y_neg_initialised(false),
      z_neg_initialised(false) {
    measurables.fill(NULL);
}

void MeasurableManager::Init(const I2c *bus_a, const I2c *bus_b,
                             const I2c *bus_c, const I2c *bus_d) {
    this->bus_a = bus_a;
    this->bus_b = bus_b;
    this->bus_c = bus_c;
    this->bus_d = bus_d;

    I2cMultiplexer *mux_a = new I2cMultiplexer(bus_a, 0x76);
    I2cMultiplexer *mux_c = new I2cMultiplexer(bus_c, 0x71);

    // TODO(dingbenjamin): Gracefully terminate failure of subsystem
    // initialisation by deleting heap allocated objects
    try {
        if (!telecomms_initialised) InitTelecomms(mux_a);
    } catch (etl::exception &e) {
        EtlUtils::LogException(e);
    }

    try {
        if (!power_initialised) InitPower(mux_a);
    } catch (etl::exception &e) {
        EtlUtils::LogException(e);
    }

    try {
        if (!flight_systems_initialised) InitFlightSystems(mux_a);
    } catch (etl::exception &e) {
        EtlUtils::LogException(e);
    }

    try {
        if (!cdh_initialised) InitCdh(mux_a);
    } catch (etl::exception &e) {
        EtlUtils::LogException(e);
    }

    try {
        if (!utilities_initialised) InitUtilities(mux_c);
    } catch (etl::exception &e) {
        EtlUtils::LogException(e);
    }

    try {
        if (!x_pos_initialised) InitXPosPanel(mux_c);
    } catch (etl::exception &e) {
        EtlUtils::LogException(e);
    }

    try {
        if (!y_pos_initialised) InitYPosPanel(mux_c);
    } catch (etl::exception &e) {
        EtlUtils::LogException(e);
    }

    try {
        if (!z_pos_initialised) InitZPosPanel(mux_c);
    } catch (etl::exception &e) {
        EtlUtils::LogException(e);
    }

    try {
        if (!x_neg_initialised) InitXNegPanel(mux_c);
    } catch (etl::exception &e) {
        EtlUtils::LogException(e);
    }

    try {
        if (!y_neg_initialised) InitYNegPanel(mux_c);
    } catch (etl::exception &e) {
        EtlUtils::LogException(e);
    }

    try {
        if (!z_neg_initialised) InitZNegPanel(mux_c);
    } catch (etl::exception &e) {
        EtlUtils::LogException(e);
    }

    if (!(telecomms_initialised && power_initialised &&
          flight_systems_initialised && cdh_initialised &&
          utilities_initialised && x_pos_initialised && y_pos_initialised &&
          z_pos_initialised && x_neg_initialised && y_neg_initialised &&
          z_neg_initialised)) {
        throw etl::exception("Failed to fully initialise Measurable Manager",
                             __FILE__, __LINE__);
    }
}

void MeasurableManager::InitTelecomms(const I2cMultiplexer *mux_a) {
    static bool comms_adc_1_initialised = false;
    static bool comms_adc_2_initialised = false;
    static bool comms_temp_1_initialised = false;
    static bool comms_temp_2_initialised = false;

    Adc *comms_adc_1 = NULL;
    Adc *comms_adc_2 = NULL;
    Mcp9808 *comms_temp_1 = NULL;
    Mcp9808 *comms_temp_2 = NULL;

    if (!comms_adc_1_initialised) {
        try {
            comms_adc_1 =
                new Adc(bus_a, 0x48, mux_a, I2cMultiplexer::kMuxChannel3);
            comms_adc_1->SetGainAmplifierLevel(kAdc4v096);

            AddVoltage(kComInV1, comms_adc_1, kAdcP2NGnd, 2.0);
            AddVoltage(kComOutV1, comms_adc_1, kAdcP3NGnd, 3.0);
            AddCurrent(kComInI1, comms_adc_1, kAdcP0NGnd, 1, 0);
            AddCurrent(kComOutI1, comms_adc_1, kAdcP1NGnd, 1, 0);

            comms_adc_1_initialised = true;
        } catch (etl::exception &e) {
            if (comms_adc_1 != NULL) delete comms_adc_1;
            DeleteMeasurable(kComInV1);
            DeleteMeasurable(kComOutV1);
            DeleteMeasurable(kComInI1);
            DeleteMeasurable(kComOutI1);
        }
    }

    if (!comms_adc_2_initialised) {
        try {
            Adc *comms_adc_2 =
                new Adc(bus_a, 0x49, mux_a, I2cMultiplexer::kMuxChannel3);
            comms_adc_2->SetGainAmplifierLevel(kAdc4v096);

            AddVoltage(kComInV2, comms_adc_2, kAdcP2NGnd, 2.0);
            AddVoltage(kComOutV2, comms_adc_2, kAdcP3NGnd, 3.0);
            AddCurrent(kComInI2, comms_adc_2, kAdcP0NGnd, 1, 0);
            AddCurrent(kComOutI2, comms_adc_2, kAdcP1NGnd, 1, 0);

            comms_adc_2_initialised = true;
        } catch (etl::exception &e) {
            if (comms_adc_2 != NULL) delete comms_adc_2;
            DeleteMeasurable(kComInV2);
            DeleteMeasurable(kComOutV2);
            DeleteMeasurable(kComInI2);
            DeleteMeasurable(kComOutI2);
        }
    }

    if (!comms_temp_1_initialised) {
        try {
            comms_temp_1 =
                new Mcp9808(bus_a, 0x18, mux_a, I2cMultiplexer::kMuxChannel3);
            AddTemperature(kComT1, comms_temp_1);

            comms_temp_1_initialised = true;
        } catch (etl::exception &e) {
            if (comms_temp_1 != NULL) delete comms_temp_1;
            DeleteMeasurable(kComT1);
        }
    }

    if (!comms_temp_2_initialised) {
        try {
            comms_temp_2 =
                new Mcp9808(bus_a, 0x19, mux_a, I2cMultiplexer::kMuxChannel3);
            AddTemperature(kComT2, comms_temp_2);

            comms_temp_2_initialised = true;
        } catch (etl::exception &e) {
            if (comms_temp_2 != NULL) delete comms_temp_2;
            DeleteMeasurable(kComT2);
        }
    }

    if (!(comms_adc_1_initialised && comms_adc_2_initialised &&
          comms_temp_1_initialised && comms_temp_2_initialised))
        throw etl::exception("Telecomms not succesfully initialised", __FILE__,
                             __LINE__);

    telecomms_initialised = true;
}

void MeasurableManager::InitPower(const I2cMultiplexer *mux_a) {
    Adc *power_adc_1 = NULL;
    Adc *power_adc_2 = NULL;
    Adc *power_adc_3 = NULL;

    Mcp9808 *power_temp_1 = NULL;
    Mcp9808 *power_temp_2 = NULL;

    Bms *bms_bus_d = NULL;
    Bms *bms_bus_c = NULL;

    static bool power_adc_1_initialised = false;
    static bool power_adc_2_initialised = false;
    static bool power_adc_3_initialised = false;

    static bool power_temp_1_initialised = false;
    static bool power_temp_2_initialised = false;

    static bool bms_bus_d_initialised = false;
    static bool bms_bus_c_initialised = false;

    if (!power_adc_1_initialised) {
        try {
            power_adc_1 =
                new Adc(bus_a, 0x48, mux_a, I2cMultiplexer::kMuxChannel2);
            power_adc_1->SetGainAmplifierLevel(kAdc2v048);
            AddVoltage(kEpsAdcBatV1, power_adc_1, kAdcP0NGnd, 3.0);
            AddVoltage(kEpsBoostOutV1, power_adc_1, kAdcP1NGnd, 3.0);
            AddCurrent(kEpsBoostInI1, power_adc_1, kAdcP2NGnd, 5.0 / 3, 0);
            AddCurrent(kEpsLoadI1, power_adc_1, kAdcP3NGnd, 5.0 / 3, 0);
            power_adc_1_initialised = true;
        } catch (etl::exception &e) {
            if (power_adc_1 != NULL) delete power_adc_1;
            DeleteMeasurable(kEpsAdcBatV1);
            DeleteMeasurable(kEpsBoostOutV1);
            DeleteMeasurable(kEpsBoostInI1);
            DeleteMeasurable(kEpsLoadI1);
        }
    }

    if (!power_adc_2_initialised) {
        try {
            power_adc_2 =
                new Adc(bus_a, 0x49, mux_a, I2cMultiplexer::kMuxChannel2);
            power_adc_2->SetGainAmplifierLevel(kAdc2v048);
            AddVoltage(kEpsAdcBatV2, power_adc_2, kAdcP0NGnd, 3.0);
            AddVoltage(kEpsBoostOutV2, power_adc_2, kAdcP1NGnd, 3.0);
            AddCurrent(kEpsLoadI2, power_adc_2, kAdcP2NGnd, 5.0 / 3, 0);
            AddCurrent(kEpsBoostInI2, power_adc_2, kAdcP3NGnd, 5.0 / 3, 0);
            power_adc_2_initialised = true;
        } catch (etl::exception &e) {
            if (power_adc_2 != NULL) delete power_adc_2;
            DeleteMeasurable(kEpsAdcBatV2);
            DeleteMeasurable(kEpsBoostOutV2);
            DeleteMeasurable(kEpsLoadI2);
            DeleteMeasurable(kEpsBoostInI2);
        }
    }

    if (!power_adc_3_initialised) {
        try {
            power_adc_3 =
                new Adc(bus_a, 0x4A, mux_a, I2cMultiplexer::kMuxChannel2);
            power_adc_3->SetGainAmplifierLevel(kAdc2v048);
            AddVoltage(kEpsRail1, power_adc_3, kAdcP0NGnd, 3.0);
            AddVoltage(kEpsRail2, power_adc_3, kAdcP1NGnd, 3.0);
            power_adc_3_initialised = true;
        } catch (etl::exception &e) {
            if (power_adc_3 != NULL) delete power_adc_3;
            DeleteMeasurable(kEpsRail1);
            DeleteMeasurable(kEpsRail2);
        }
    }

    if (!power_temp_1_initialised) {
        try {
            power_temp_1 =
                new Mcp9808(bus_a, 0x18, mux_a, I2cMultiplexer::kMuxChannel2);
            AddTemperature(kEpsT1, power_temp_1);
            power_temp_1_initialised = true;
        } catch (etl::exception &e) {
            if (power_adc_2 != NULL) delete power_adc_2;
            DeleteMeasurable(kEpsT1);
        }
    }

    if (!power_temp_2_initialised) {
        try {
            power_temp_2 =
                new Mcp9808(bus_a, 0x19, mux_a, I2cMultiplexer::kMuxChannel2);
            AddTemperature(kEpsT2, power_temp_2);
            power_temp_2_initialised = true;
        } catch (etl::exception &e) {
            if (power_temp_2 != NULL) delete power_temp_2;
            DeleteMeasurable(kEpsT2);
        }
    }

    // TODO(hugorilla): Remove redundant BMS measurables here

    if (!bms_bus_c_initialised) {
        try {
            bms_bus_c =
                new Bms(bus_c, 0x68, NULL, I2cMultiplexer::kMuxNoChannel);
            AddBmsBatteryTempMeasurable(kEpsBmsBatT2, bms_bus_c);
            AddBmsDieTempMeasurable(kEpsBmsDieT2, bms_bus_c);
            AddBmsSettingsMeasurable(kEpsBmsSettingsReading2, bms_bus_c);
            AddBmsChargingInfoMeasurable(kEpsBmsChargingInfoReading2,
                                         bms_bus_c);
            AddBmsTemperatureMeasurable(kEpsBmsTemperatureReading2, bms_bus_c);
            AddBmsVoltagesMeasurable(kEpsBmsVoltagesReading2, bms_bus_c);
            AddBmsCurrentsMeasurable(kEpsBmsCurrentsReading2, bms_bus_c);
            AddBmsOperationValuesMeasurable(kEpsBmsOperationValuesReading2,
                                            bms_bus_c);
            bms_bus_c_initialised = true;
        } catch (etl::exception &e) {
            if (bms_bus_c != NULL) delete bms_bus_c;
            DeleteMeasurable(kEpsBmsDieT2);
            DeleteMeasurable(kEpsBmsSettingsReading2);
            DeleteMeasurable(kEpsBmsChargingInfoReading2);
            DeleteMeasurable(kEpsBmsTemperatureReading2);
            DeleteMeasurable(kEpsBmsVoltagesReading2);
            DeleteMeasurable(kEpsBmsCurrentsReading2);
            DeleteMeasurable(kEpsBmsOperationValuesReading2);
        }
    }

    if (!bms_bus_d_initialised) {
        try {
            bms_bus_d =
                new Bms(bus_d, 0x68, NULL, I2cMultiplexer::kMuxNoChannel);
            AddBmsBatteryTempMeasurable(kEpsBmsBatT1, bms_bus_d);
            AddBmsDieTempMeasurable(kEpsBmsDieT1, bms_bus_d);
            AddBmsSettingsMeasurable(kEpsBmsSettingsReading1, bms_bus_d);
            AddBmsChargingInfoMeasurable(kEpsBmsChargingInfoReading1,
                                         bms_bus_d);
            AddBmsTemperatureMeasurable(kEpsBmsTemperatureReading1, bms_bus_d);
            AddBmsVoltagesMeasurable(kEpsBmsVoltagesReading1, bms_bus_d);
            AddBmsCurrentsMeasurable(kEpsBmsCurrentsReading1, bms_bus_d);
            AddBmsOperationValuesMeasurable(kEpsBmsOperationValuesReading1,
                                            bms_bus_d);
            bms_bus_d_initialised = true;
        } catch (etl::exception &e) {
            if (bms_bus_d != NULL) delete bms_bus_d;
            DeleteMeasurable(kEpsBmsDieT1);
            DeleteMeasurable(kEpsBmsSettingsReading1);
            DeleteMeasurable(kEpsBmsChargingInfoReading1);
            DeleteMeasurable(kEpsBmsTemperatureReading1);
            DeleteMeasurable(kEpsBmsVoltagesReading1);
            DeleteMeasurable(kEpsBmsCurrentsReading1);
            DeleteMeasurable(kEpsBmsOperationValuesReading1);
        }
    }

    if (!(power_adc_1_initialised && power_adc_2_initialised &&
          power_temp_1_initialised && power_temp_2_initialised &&
          bms_bus_d_initialised && bms_bus_c_initialised))
        throw etl::exception("Power not successfully initialised", __FILE__,
                             __LINE__);

    power_initialised = true;
}

void MeasurableManager::InitFlightSystems(const I2cMultiplexer *mux_a) {
    Adc *fs_adc_x = NULL;
    Adc *fs_adc_y = NULL;
    Adc *fs_adc_z = NULL;
    Mcp9808 *fs_hb_xt = NULL;
    Mcp9808 *fs_hb_yt = NULL;
    Mcp9808 *fs_hb_zt = NULL;
    MPU9250MotionTracker *fs_imu_1 = NULL;
    MPU9250MotionTracker *fs_imu_2 = NULL;

    static bool fs_adc_x_initialised = false;
    static bool fs_adc_y_initialised = false;
    static bool fs_adc_z_initialised = false;
    static bool fs_hb_xt_initialised = false;
    static bool fs_hb_yt_initialised = false;
    static bool fs_hb_zt_initialised = false;
    static bool fs_imu_1_initialised = false;
    static bool fs_imu_2_initialised = false;

    if (!fs_adc_x_initialised) {
        try {
            fs_adc_x =
                new Adc(bus_a, 0x48, mux_a, I2cMultiplexer::kMuxChannel1);
            fs_adc_x->SetGainAmplifierLevel(kAdc4v096);
            AddVoltage(kFsTorquerXAV, fs_adc_x, kAdcP1NGnd, 2);
            AddVoltage(kFsTorquerXBV, fs_adc_x, kAdcP2NGnd, 2);
            AddCurrent(kFsTorquerXI, fs_adc_x, kAdcP0NGnd, 0.1, 1.65);
            AddCurrent(kFsTorquerTotalI, fs_adc_x, kAdcP3NGnd, 1.0 / 7.5, 0);
            fs_adc_x_initialised = true;
        } catch (etl::exception &e) {
            if (fs_adc_x_initialised != NULL) delete fs_adc_x;
            DeleteMeasurable(kFsTorquerXAV);
            DeleteMeasurable(kFsTorquerXBV);
            DeleteMeasurable(kFsTorquerXI);
            DeleteMeasurable(kFsTorquerTotalI);
        }
    }

    if (!fs_adc_y_initialised) {
        try {
            fs_adc_y =
                new Adc(bus_a, 0x49, mux_a, I2cMultiplexer::kMuxChannel1);
            fs_adc_y->SetGainAmplifierLevel(kAdc4v096);
            AddVoltage(kFsTorquerYAV, fs_adc_y, kAdcP1NGnd, 2);
            AddVoltage(kFsTorquerYBV, fs_adc_y, kAdcP2NGnd, 2);
            AddCurrent(kFsTorquerYI, fs_adc_y, kAdcP0NGnd, 0.1, 1.65);
            fs_adc_y_initialised = true;
        } catch (etl::exception &e) {
            if (fs_adc_y != NULL) delete fs_adc_y;
            DeleteMeasurable(kFsTorquerYAV);
            DeleteMeasurable(kFsTorquerYBV);
            DeleteMeasurable(kFsTorquerYI);
        }
    }

    if (!fs_adc_z_initialised) {
        try {
            fs_adc_z =
                new Adc(bus_a, 0x4B, mux_a, I2cMultiplexer::kMuxChannel1);
            fs_adc_z->SetGainAmplifierLevel(kAdc4v096);
            AddVoltage(kFsTorquerZAV, fs_adc_z, kAdcP1NGnd, 2);
            AddVoltage(kFsTorquerZBV, fs_adc_z, kAdcP2NGnd, 2);
            AddCurrent(kFsTorquerZI, fs_adc_z, kAdcP0NGnd, 0.1, 1.65);
            fs_adc_z_initialised = true;

        } catch (etl::exception &e) {
            if (fs_adc_z != NULL) delete fs_adc_z;
            DeleteMeasurable(kFsTorquerZAV);
            DeleteMeasurable(kFsTorquerZBV);
            DeleteMeasurable(kFsTorquerZI);
        }
    }

    if (!fs_hb_xt_initialised) {
        try {
            fs_hb_xt =
                new Mcp9808(bus_a, 0x18, mux_a, I2cMultiplexer::kMuxChannel1);
            AddTemperature(kFsHbXT, fs_hb_xt);
            fs_hb_xt_initialised = true;
        } catch (etl::exception &e) {
            if (fs_hb_xt != NULL) delete fs_hb_xt;
            DeleteMeasurable(kFsHbXT);
        }
    }

    if (!fs_hb_yt_initialised) {
        try {
            fs_hb_yt =
                new Mcp9808(bus_a, 0x19, mux_a, I2cMultiplexer::kMuxChannel1);
            AddTemperature(kFsHbYT, fs_hb_yt);
            fs_hb_yt_initialised = true;
        } catch (etl::exception &e) {
            if (fs_hb_yt != NULL) delete fs_hb_yt;
            DeleteMeasurable(kFsHbYT);
        }
    }

    if (!fs_hb_zt_initialised) {
        try {
            fs_hb_zt =
                new Mcp9808(bus_a, 0x1A, mux_a, I2cMultiplexer::kMuxChannel1);
            AddTemperature(kFsHbZT, fs_hb_zt);
            fs_hb_zt_initialised = true;
        } catch (etl::exception &e) {
            if (fs_hb_zt != NULL) delete fs_hb_zt;
            DeleteMeasurable(kFsHbZT);
        }
    }

    if (!fs_imu_1_initialised) {
        try {
            fs_imu_1 = new MPU9250MotionTracker(bus_a, 0x68, mux_a,
                                                I2cMultiplexer::kMuxChannel1);
            AddImuGyrometerMeasurable(kFsImuGyro1, fs_imu_1);
            AddImuAccelerometerMeasurable(kFsImuAccel1, fs_imu_1);
            AddImuTemperatureMeasurable(kFsImuT1, fs_imu_1);
            double initial_biases_bus_a_data[3][1];
            const Matrix initial_biases_bus_a(
                kPreFlightMagnetometerCalibrationBiasesImuBusA,
                initial_biases_bus_a_data);
            double initial_scale_factors_bus_a_data[3][3];
            const Matrix initial_scale_factors_bus_a(
                kPreFlightMagnetometerCalibrationScaleFactorsImuBusA,
                initial_scale_factors_bus_a_data);

            AddImuMagnetometerMeasurable(
                kFsImuMagno1, fs_imu_1, kImuAToBodyFrameTransform,
                initial_biases_bus_a, initial_scale_factors_bus_a,
                MagnetometerCalibration::kBufferFilenameA);
            fs_imu_1_initialised = true;
        } catch (etl::exception &e) {
            if (fs_imu_1 != NULL) delete fs_imu_1;
            DeleteMeasurable(kFsImuGyro1);
            DeleteMeasurable(kFsImuAccel1);
            DeleteMeasurable(kFsImuT1);
            DeleteMeasurable(kFsImuMagno1);
        }
    }

    if (!fs_imu_2_initialised) {
        try {
            fs_imu_2 = new MPU9250MotionTracker(bus_b, 0x68);
            AddImuGyrometerMeasurable(kFsImuGyro2, fs_imu_2);
            AddImuAccelerometerMeasurable(kFsImuAccel2, fs_imu_2);
            AddImuTemperatureMeasurable(kFsImuT2, fs_imu_2);
            double initial_biases_bus_b_data[3][1];
            const Matrix initial_biases_bus_b(
                kPreFlightMagnetometerCalibrationBiasesImuBusB,
                initial_biases_bus_b_data);
            double initial_scale_factors_bus_b_data[3][3];
            const Matrix initial_scale_factors_bus_b(
                kPreFlightMagnetometerCalibrationScaleFactorsImuBusB,
                initial_scale_factors_bus_b_data);
            AddImuMagnetometerMeasurable(
                kFsImuMagno2, fs_imu_2, kImuBToBodyFrameTransform,
                initial_biases_bus_b, initial_scale_factors_bus_b,
                MagnetometerCalibration::kBufferFilenameA);
            fs_imu_2_initialised = true;
        } catch (etl::exception &e) {
            if (fs_imu_2 != NULL) delete fs_imu_2;
            DeleteMeasurable(kFsImuGyro2);
            DeleteMeasurable(kFsImuAccel2);
            DeleteMeasurable(kFsImuT2);
            DeleteMeasurable(kFsImuMagno2);
        }
    }

    if (!(fs_adc_x_initialised && fs_adc_y_initialised &&
          fs_adc_z_initialised && fs_hb_xt_initialised &&
          fs_hb_yt_initialised && fs_hb_zt_initialised &&
          fs_imu_1_initialised && fs_imu_2_initialised))
        throw etl::exception("Flight Systems not successfully initialised",
                             __FILE__, __LINE__);

    flight_systems_initialised = true;
}

void MeasurableManager::InitUtilities(const I2cMultiplexer *mux_c) {
    Adc *util_adc_1 = NULL;
    Mcp9808 *util_temp = NULL;

    static bool util_adc_1_initialised = false;
    static bool util_temp_initialised = false;

    if (!util_adc_1_initialised) {
        try {
            util_adc_1 =
                new Adc(bus_c, 0x49, mux_c, I2cMultiplexer::kMuxChannel1);
            AddVoltage(kUtilHeatV, util_adc_1, kAdcP3NGnd, 1);
            util_adc_1_initialised = true;
        } catch (etl::exception &e) {
            if (util_adc_1 != NULL) delete util_adc_1;
            DeleteMeasurable(kUtilHeatV);
        }
    }

    if (!util_temp_initialised) {
        try {
            util_temp =
                new Mcp9808(bus_c, 0x1C, mux_c, I2cMultiplexer::kMuxChannel1);
            AddTemperature(kUtilT, util_temp);
            util_temp_initialised = true;
        } catch (etl::exception &e) {
            if (util_temp != NULL) delete util_temp;
            DeleteMeasurable(kUtilT);
        }
    }

    if (!(util_adc_1_initialised && util_temp_initialised))
        throw etl::exception("Utilities not successfully initialised", __FILE__,
                             __LINE__);

    utilities_initialised = true;
}

void MeasurableManager::InitCdh(const I2cMultiplexer *mux_a) {
    Mcp9808 *cdh_temp_1 = NULL;
    static bool cdh_temp_1_initialised = false;

    if (!cdh_temp_1_initialised) {
        try {
            Mcp9808 *cdh_temp_1 =
                new Mcp9808(bus_a, 0x1A, mux_a, I2cMultiplexer::kMuxChannel0);
            AddTemperature(kCdhT, cdh_temp_1);
            cdh_temp_1_initialised = true;
        } catch (etl::exception &e) {
            if (cdh_temp_1 != NULL) delete cdh_temp_1;
            DeleteMeasurable(kCdhT);
        }
    }

    if (!cdh_temp_1_initialised)
        throw etl::exception("CDH not successfully initialised", __FILE__,
                             __LINE__);

    cdh_initialised = true;
}

void MeasurableManager::InitXPosPanel(const I2cMultiplexer *mux_c) {
    Mcp9808 *x_pos_temp_1 = NULL;
    Mcp9808 *x_pos_temp_2 = NULL;
    Adc *x_pos_adc = NULL;

    static bool x_pos_temp_1_initialised = false;
    static bool x_pos_temp_2_initialised = false;
    static bool x_pos_adc_initialised = false;

    if (!x_pos_temp_1_initialised) {
        try {
            x_pos_temp_1 =
                new Mcp9808(bus_c, 0x19, mux_c, I2cMultiplexer::kMuxChannel4);
            AddTemperature(kXPosT1, x_pos_temp_1);
            x_pos_temp_1_initialised = true;
        } catch (etl::exception &e) {
            if (x_pos_temp_1 != NULL) delete x_pos_temp_1;
            DeleteMeasurable(kXPosT1);
        }
    }

    if (!x_pos_temp_2_initialised) {
        try {
            x_pos_temp_2 =
                new Mcp9808(bus_c, 0x1A, mux_c, I2cMultiplexer::kMuxChannel4);
            AddTemperature(kXPosT2, x_pos_temp_2);
            x_pos_temp_2_initialised = true;
        } catch (etl::exception &e) {
            if (x_pos_temp_2 != NULL) delete x_pos_temp_2;
            DeleteMeasurable(kXPosT2);
        }
    }

    if (!x_pos_adc_initialised) {
        try {
            x_pos_adc =
                new Adc(bus_c, 0x48, mux_c, I2cMultiplexer::kMuxChannel4);
            x_pos_adc->SetGainAmplifierLevel(kAdc4v096);
            AddVoltage(kXPosV, x_pos_adc, kAdcP0NGnd, 2.0);
            AddVoltage(kXPosSolarV, x_pos_adc, kAdcP2NGnd, 2.0);
            AddCurrent(kXPosI, x_pos_adc, kAdcP1NGnd, 0.285714, 0);
            AddCurrent(kXPosSolarI, x_pos_adc, kAdcP3NGnd, 0.285714, 0);
            x_pos_adc_initialised = true;
        } catch (etl::exception &e) {
            if (x_pos_adc != NULL) delete x_pos_adc;
            DeleteMeasurable(kXPosV);
            DeleteMeasurable(kXPosSolarV);
            DeleteMeasurable(kXPosI);
            DeleteMeasurable(kXPosSolarI);
        }
    }

    if (!(x_pos_temp_1_initialised && x_pos_temp_2_initialised &&
          x_pos_adc_initialised))
        throw etl::exception("X+ panel not successfully initialised", __FILE__,
                             __LINE__);

    x_pos_initialised = true;
}

void MeasurableManager::InitYPosPanel(const I2cMultiplexer *mux_c) {
    Mcp9808 *y_pos_temp_1 = NULL;
    Mcp9808 *y_pos_temp_2 = NULL;
    Adc *y_pos_adc = NULL;

    static bool y_pos_temp_1_initialised = false;
    static bool y_pos_temp_2_initialised = false;
    static bool y_pos_adc_initialised = false;

    if (!y_pos_temp_1_initialised) {
        try {
            y_pos_temp_1 =
                new Mcp9808(bus_c, 0x19, mux_c, I2cMultiplexer::kMuxChannel5);
            AddTemperature(kYPosT1, y_pos_temp_1);
            y_pos_temp_1_initialised = true;
        } catch (etl::exception &e) {
            if (y_pos_temp_1 != NULL) delete y_pos_temp_1;
            DeleteMeasurable(kYPosT1);
        }
    }

    if (!y_pos_temp_2_initialised) {
        try {
            y_pos_temp_2 =
                new Mcp9808(bus_c, 0x1A, mux_c, I2cMultiplexer::kMuxChannel5);
            AddTemperature(kYPosT2, y_pos_temp_2);
            y_pos_temp_2_initialised = true;
        } catch (etl::exception &e) {
            if (y_pos_temp_2 != NULL) delete y_pos_temp_2;
            DeleteMeasurable(kYPosT2);
        }
    }

    if (!y_pos_adc_initialised) {
        try {
            y_pos_adc =
                new Adc(bus_c, 0x48, mux_c, I2cMultiplexer::kMuxChannel5);
            y_pos_adc->SetGainAmplifierLevel(kAdc4v096);
            AddVoltage(kYPosV, y_pos_adc, kAdcP0NGnd, 2.0);
            AddVoltage(kYPosSolarV, y_pos_adc, kAdcP2NGnd, 2.0);
            AddCurrent(kYPosI, y_pos_adc, kAdcP1NGnd, 0.285714, 0);
            AddCurrent(kYPosSolarI, y_pos_adc, kAdcP3NGnd, 0.285714, 0);
            y_pos_adc_initialised = true;
        } catch (etl::exception &e) {
            if (y_pos_adc != NULL) delete y_pos_adc;
            DeleteMeasurable(kYPosV);
            DeleteMeasurable(kYPosSolarV);
            DeleteMeasurable(kYPosI);
            DeleteMeasurable(kYPosSolarI);
        }
    }

    if (!(y_pos_temp_1_initialised && y_pos_temp_2_initialised &&
          y_pos_adc_initialised))
        throw etl::exception("X+ panel not successfully initialised", __FILE__,
                             __LINE__);

    y_pos_initialised = true;
}

void MeasurableManager::InitZPosPanel(const I2cMultiplexer *mux_c) {
    Mcp9808 *z_pos_temp_1 = NULL;
    Adc *z_pos_adc = NULL;

    static bool z_pos_temp_1_initialised = false;
    static bool z_pos_adc_initialised = false;

    if (!z_pos_temp_1_initialised) {
        try {
            z_pos_temp_1 =
                new Mcp9808(bus_c, 0x19, mux_c, I2cMultiplexer::kMuxChannel2);
            AddTemperature(kZPosT, z_pos_temp_1);
            z_pos_temp_1_initialised = true;
        } catch (etl::exception &e) {
            if (z_pos_temp_1 != NULL) delete z_pos_temp_1;
            DeleteMeasurable(kZPosT);
        }
    }

    if (!z_pos_adc_initialised) {
        try {
            z_pos_adc =
                new Adc(bus_c, 0x4B, mux_c, I2cMultiplexer::kMuxChannel2);
            z_pos_adc->SetGainAmplifierLevel(kAdc4v096);
            AddVoltage(kEpsTopPanelV, z_pos_adc, kAdcP0NGnd, 3.0);
            AddVoltage(kEpsTopSolarV, z_pos_adc, kAdcP1NGnd, 3.0);
            AddCurrent(kEpsTopSolarI, z_pos_adc, kAdcP2NGnd, 5.0 / 3, 0);
            AddCurrent(kEpsTopPanelI, z_pos_adc, kAdcP3NGnd, 5.0 / 3, 0);
            z_pos_adc_initialised = true;
        } catch (etl::exception &e) {
            if (z_pos_adc != NULL) delete z_pos_adc;
            DeleteMeasurable(kEpsTopPanelV);
            DeleteMeasurable(kEpsTopSolarV);
            DeleteMeasurable(kEpsTopSolarI);
            DeleteMeasurable(kEpsTopPanelI);
        }
    }

    if (!(z_pos_temp_1_initialised && z_pos_adc_initialised))
        throw etl::exception("Z+ panel not successfully initialised", __FILE__,
                             __LINE__);

    z_pos_initialised = true;
}

void MeasurableManager::InitXNegPanel(const I2cMultiplexer *mux_c) {
    Mcp9808 *x_neg_temp_1 = NULL;
    Mcp9808 *x_neg_temp_2 = NULL;
    Adc *x_neg_adc = NULL;

    static bool x_neg_temp_1_initialised = false;
    static bool x_neg_temp_2_initialised = false;
    static bool x_neg_adc_initialised = false;

    if (!x_neg_temp_1_initialised) {
        try {
            x_neg_temp_1 =
                new Mcp9808(bus_c, 0x19, mux_c, I2cMultiplexer::kMuxChannel6);
            AddTemperature(kXNegT1, x_neg_temp_1);
            x_neg_temp_1_initialised = true;
        } catch (etl::exception &e) {
            if (x_neg_temp_1 != NULL) delete x_neg_temp_1;
            DeleteMeasurable(kXNegT1);
        }
    }

    if (!x_neg_temp_2_initialised) {
        try {
            x_neg_temp_2 =
                new Mcp9808(bus_c, 0x1A, mux_c, I2cMultiplexer::kMuxChannel6);
            AddTemperature(kXNegT2, x_neg_temp_2);
            x_neg_temp_2_initialised = true;
        } catch (etl::exception &e) {
            if (x_neg_temp_2 != NULL) delete x_neg_temp_2;
            DeleteMeasurable(kXNegT2);
        }
    }

    if (!x_neg_adc_initialised) {
        try {
            x_neg_adc =
                new Adc(bus_c, 0x48, mux_c, I2cMultiplexer::kMuxChannel6);
            x_neg_adc->SetGainAmplifierLevel(kAdc4v096);
            AddVoltage(kXNegV, x_neg_adc, kAdcP0NGnd, 2.0);
            AddVoltage(kXNegSolarV, x_neg_adc, kAdcP2NGnd, 2.0);
            AddCurrent(kXNegI, x_neg_adc, kAdcP1NGnd, 0.285714, 0);
            AddCurrent(kXNegSolarI, x_neg_adc, kAdcP3NGnd, 0.285714, 0);
            x_neg_adc_initialised = true;
        } catch (etl::exception &e) {
            if (x_neg_adc != NULL) delete x_neg_adc;
            DeleteMeasurable(kXNegV);
            DeleteMeasurable(kXNegSolarV);
            DeleteMeasurable(kXNegI);
            DeleteMeasurable(kXNegSolarI);
        }
    }

    if (!(x_neg_temp_1_initialised && x_neg_temp_2_initialised &&
          x_neg_adc_initialised))
        throw etl::exception("X+ panel not successfully initialised", __FILE__,
                             __LINE__);

    x_neg_initialised = true;
}

void MeasurableManager::InitYNegPanel(const I2cMultiplexer *mux_c) {
    Mcp9808 *y_neg_temp_1 = NULL;
    Mcp9808 *y_neg_temp_2 = NULL;
    Adc *y_neg_adc = NULL;

    static bool y_neg_temp_1_initialised = false;
    static bool y_neg_temp_2_initialised = false;
    static bool y_neg_adc_initialised = false;

    if (!y_neg_temp_1_initialised) {
        try {
            y_neg_temp_1 =
                new Mcp9808(bus_c, 0x19, mux_c, I2cMultiplexer::kMuxChannel7);
            AddTemperature(kYNegT1, y_neg_temp_1);
            y_neg_temp_1_initialised = true;
        } catch (etl::exception &e) {
            if (y_neg_temp_1 != NULL) delete y_neg_temp_1;
            DeleteMeasurable(kYNegT1);
        }
    }

    if (!y_neg_temp_2_initialised) {
        try {
            y_neg_temp_2 =
                new Mcp9808(bus_c, 0x1A, mux_c, I2cMultiplexer::kMuxChannel7);
            AddTemperature(kYNegT2, y_neg_temp_2);
            y_neg_temp_2_initialised = true;
        } catch (etl::exception &e) {
            if (y_neg_temp_2 != NULL) delete y_neg_temp_2;
            DeleteMeasurable(kYNegT2);
        }
    }

    if (!y_neg_adc_initialised) {
        try {
            y_neg_adc =
                new Adc(bus_c, 0x48, mux_c, I2cMultiplexer::kMuxChannel7);
            y_neg_adc->SetGainAmplifierLevel(kAdc4v096);
            AddVoltage(kYNegV, y_neg_adc, kAdcP0NGnd, 2.0);
            AddVoltage(kYNegSolarV, y_neg_adc, kAdcP2NGnd, 2.0);
            AddCurrent(kYNegI, y_neg_adc, kAdcP1NGnd, 0.285714, 0);
            AddCurrent(kYNegSolarI, y_neg_adc, kAdcP3NGnd, 0.285714, 0);
            y_neg_adc_initialised = true;
        } catch (etl::exception &e) {
            if (y_neg_adc != NULL) delete y_neg_adc;
            DeleteMeasurable(kYNegV);
            DeleteMeasurable(kYNegSolarV);
            DeleteMeasurable(kYNegI);
            DeleteMeasurable(kYNegSolarI);
        }
    }

    if (!(y_neg_temp_1_initialised && y_neg_temp_2_initialised &&
          y_neg_adc_initialised))
        throw etl::exception("X+ panel not successfully initialised", __FILE__,
                             __LINE__);

    y_neg_initialised = true;
}

void MeasurableManager::InitZNegPanel(const I2cMultiplexer *mux_c) {
    Mcp9808 *z_neg_temp_1 = NULL;
    Mcp9808 *z_neg_temp_2 = NULL;
    Adc *z_neg_adc = NULL;

    static bool z_neg_temp_1_initialised = false;
    static bool z_neg_temp_2_initialised = false;
    static bool z_neg_adc_initialised = false;

    if (!z_neg_temp_1_initialised) {
        try {
            z_neg_temp_1 =
                new Mcp9808(bus_c, 0x19, mux_c, I2cMultiplexer::kMuxChannel3);
            AddTemperature(kZNegT1, z_neg_temp_1);
            z_neg_temp_1_initialised = true;
        } catch (etl::exception &e) {
            if (z_neg_temp_1 != NULL) delete z_neg_temp_1;
            DeleteMeasurable(kZNegT1);
        }
    }

    if (!z_neg_temp_2_initialised) {
        try {
            z_neg_temp_2 =
                new Mcp9808(bus_c, 0x1A, mux_c, I2cMultiplexer::kMuxChannel3);
            AddTemperature(kZNegT2, z_neg_temp_2);
            z_neg_temp_2_initialised = true;
        } catch (etl::exception &e) {
            if (z_neg_temp_2 != NULL) delete z_neg_temp_2;
            DeleteMeasurable(kZNegT2);
        }
    }

    if (!z_neg_adc_initialised) {
        try {
            z_neg_adc =
                new Adc(bus_c, 0x48, mux_c, I2cMultiplexer::kMuxChannel3);
            z_neg_adc->SetGainAmplifierLevel(kAdc4v096);
            AddVoltage(kZNegV, z_neg_adc, kAdcP0NGnd, 2.0);
            AddVoltage(kZNegSolarV, z_neg_adc, kAdcP2NGnd, 2.0);
            AddCurrent(kZNegI, z_neg_adc, kAdcP1NGnd, 0.285714, 0);
            AddCurrent(kZNegSolarI, z_neg_adc, kAdcP3NGnd, 0.285714, 0);
            z_neg_adc_initialised = true;
        } catch (etl::exception &e) {
            if (z_neg_adc != NULL) delete z_neg_adc;
            DeleteMeasurable(kZNegV);
            DeleteMeasurable(kZNegSolarV);
            DeleteMeasurable(kZNegI);
            DeleteMeasurable(kZNegSolarI);
        }
    }

    if (!(z_neg_temp_1_initialised && z_neg_temp_2_initialised &&
          z_neg_adc_initialised))
        throw etl::exception("X+ panel not successfully initialised", __FILE__,
                             __LINE__);

    z_neg_initialised = true;
}

void MeasurableManager::DeleteMeasurable(MeasurableId id) {
    if (measurables[id] != NULL) {
        delete measurables[id];
        measurables[id] = NULL;
    }
}

void MeasurableManager::AddVoltage(MeasurableId id, Adc *adc, AdcMuxMode line,
                                   float scaling_factor) {
    CheckValidId(id);
    VoltageMeasurable *voltage =
        new VoltageMeasurable(adc, line, scaling_factor);
    measurables[id] = voltage;
}

void MeasurableManager::AddCurrent(MeasurableId id, Adc *adc, AdcMuxMode line,
                                   float scaling_factor,
                                   float zero_bias_point) {
    CheckValidId(id);
    CurrentMeasurable *current =
        new CurrentMeasurable(adc, line, scaling_factor, zero_bias_point);
    measurables[id] = current;
}

TemperatureMeasurable *MeasurableManager::AddTemperature(MeasurableId id,
                                                         Mcp9808 *temp_sensor) {
    CheckValidId(id);
    TemperatureMeasurable *temp = new TemperatureMeasurable(temp_sensor);
    measurables[id] = temp;
    return temp;
}

void MeasurableManager::AddBmsDieTempMeasurable(MeasurableId id, Bms *bms) {
    CheckValidId(id);
    BmsDieTemperatureMeasurable *temp = new BmsDieTemperatureMeasurable(bms);
    measurables[id] = temp;
}

void MeasurableManager::AddBmsSettingsMeasurable(MeasurableId id, Bms *bms) {
    CheckValidId(id);
    BmsSettingsMeasurable *bms_settings = new BmsSettingsMeasurable(bms);
    measurables[id] = bms_settings;
}

void MeasurableManager::AddBmsChargingInfoMeasurable(MeasurableId id,
                                                     Bms *bms) {
    CheckValidId(id);
    BmsChargingInfoMeasurable *bms_charging_info =
        new BmsChargingInfoMeasurable(bms);
    measurables[id] = bms_charging_info;
}

void MeasurableManager::AddBmsOperationValuesMeasurable(MeasurableId id,
                                                        Bms *bms) {
    CheckValidId(id);
    BmsOperationValuesMeasurable *bms_operation_values =
        new BmsOperationValuesMeasurable(bms);
    measurables[id] = bms_operation_values;
}

void MeasurableManager::AddBmsTemperatureMeasurable(MeasurableId id, Bms *bms) {
    CheckValidId(id);
    BmsTemperatureMeasurable *bms_temperatures =
        new BmsTemperatureMeasurable(bms);
    measurables[id] = bms_temperatures;
}

void MeasurableManager::AddBmsVoltagesMeasurable(MeasurableId id, Bms *bms) {
    CheckValidId(id);
    BmsVoltagesMeasurable *bms_voltages = new BmsVoltagesMeasurable(bms);
    measurables[id] = bms_voltages;
}

void MeasurableManager::AddBmsCurrentsMeasurable(MeasurableId id, Bms *bms) {
    CheckValidId(id);
    BmsCurrentsMeasurable *bms_currents = new BmsCurrentsMeasurable(bms);
    measurables[id] = bms_currents;
}

void MeasurableManager::AddImuGyrometerMeasurable(
    MeasurableId id, MPU9250MotionTracker *imu_sensor) {
    CheckValidId(id);
    ImuGyroscopeMeasurable *gyro = new ImuGyroscopeMeasurable(imu_sensor);
    measurables[id] = gyro;
}

void MeasurableManager::AddImuAccelerometerMeasurable(
    MeasurableId id, MPU9250MotionTracker *imu_sensor) {
    CheckValidId(id);
    ImuAccelerometerMeasurable *accelerometer =
        new ImuAccelerometerMeasurable(imu_sensor);
    measurables[id] = accelerometer;
}

void MeasurableManager::AddImuTemperatureMeasurable(
    MeasurableId id, MPU9250MotionTracker *imu_sensor) {
    CheckValidId(id);
    ImuTemperatureMeasurable *temp = new ImuTemperatureMeasurable(imu_sensor);
    measurables[id] = temp;
}

void MeasurableManager::AddImuMagnetometerMeasurable(
    MeasurableId id, MPU9250MotionTracker *imu_sensor,
    const Matrix &frame_mapping, const Matrix &initial_biases,
    const Matrix &initial_scale_factors,
    const char *calibration_readings_buffer_filename) {
    CheckValidId(id);
    ImuMagnetometerMeasurable *magnetometer = new ImuMagnetometerMeasurable(
        imu_sensor, frame_mapping, initial_biases, initial_scale_factors,
        calibration_readings_buffer_filename);
    measurables[id] = magnetometer;
}

BmsBatteryTemperatureMeasurable *MeasurableManager::AddBmsBatteryTempMeasurable(
    MeasurableId id, Bms *bms) {
    CheckValidId(id);
    BmsBatteryTemperatureMeasurable *temp =
        new BmsBatteryTemperatureMeasurable(bms);
    measurables[id] = temp;
    return temp;
}

void MeasurableManager::CheckValidId(MeasurableId id) {
    if (id >= kMeasurableIdEnd) {
        etl::exception e("Measurable ID outside max size", __FILE__, __LINE__);
        throw e;
    } else if (measurables.at(id) != NULL) {
        etl::exception e("ID already used", __FILE__, __LINE__);
        throw e;
    }
}
