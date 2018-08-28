#include <CppUTest/TestHarness.h>
#include <src/config/satellite.h>
#include <src/sensors/magnetometer_calibration.h>

const double test_calibration_values_data[40][3] = {
    {-36.74, -32.77, -117.56}, {-36.74, -32.77, -117.56},
    {-36.56, -33.29, -117.73}, {-35.52, -32.94, -117.39},
    {-35.87, -32.60, -117.73}, {-36.56, -31.55, -118.40},
    {-37.26, -31.55, -118.40}, {-36.91, -32.94, -117.39},
    {-37.78, -32.07, -117.22}, {-35.17, -35.39, -115.37},
    {-35.17, -35.39, -115.37}, {-36.21, -39.23, -113.36},
    {-35.35, -44.99, -109.49}, {-36.74, -48.48, -106.80},
    {-37.43, -49.53, -106.13}, {-36.21, -51.45, -103.61},
    {-37.26, -57.03, -99.91},  {-37.26, -61.57, -93.19},
    {-35.87, -64.01, -87.13},  {-35.87, -64.01, -87.13},
    {-37.61, -71.34, -75.03},  {-37.95, -73.44, -66.29},
    {-38.82, -74.66, -59.73},  {-42.65, -75.36, -52.00},
    {-43.52, -74.83, -46.79},  {-43.69, -73.61, -41.24},
    {-43.87, -71.69, -36.03},  {-43.87, -71.69, -36.03},
    {-46.83, -70.47, -34.52},  {-47.52, -71.52, -37.21},
    {-54.13, -70.47, -41.92},  {-59.70, -70.12, -49.65},
    {-65.96, -68.38, -56.37},  {-74.48, -62.97, -59.23},
    {-83.70, -56.86, -60.74},  {-92.92, -46.91, -65.62},
    {-101.97, -28.76, -73.35}, {-101.97, -28.76, -73.35},
    {-102.32, -14.09, -83.10}, {-99.19, -5.02, -92.18}};

const double biases_expected_data[3][1] = {
    {-47.7545590950}, {-30.0125021696}, {-72.0746933014}};
const double scale_factors_expected_data[3][3] = {
    {1113.2012457487, 116.4796625013, -84.2727071949},
    {116.4796625013, 1431.0120177096, 250.0608796889},
    {-84.2727071949, 250.0608796889, 1202.6204518099}};

const double aggregated_readings_expected_data[10][10] = {
    {686753805.032894, 283283607.234315, 743847495.015528, 704096683.962669,
     1246029696.435966, 643749082.370985, -16768448.088310, -10304061.384014,
     -17872010.450386, 117965.448500},
    {283283607.234315, 482542964.958296, 636472047.571461, 927553206.515871,
     715822752.047222, 684281600.109327, -10977204.352814, -14679835.627210,
     -15997798.403232, 117214.154900},
    {743847495.015528, 636472047.571461, 3380513846.544021, 2545397743.757553,
     2695048219.141963, 1123684969.587659, -27944984.625066, -26450833.965132,
     -63673369.811902, 313838.250100},
    {704096683.962669, 927553206.515871, 2545397743.757553, 2545888190.285846,
     2247369939.175317, 1431645504.094443, -27358392.093596, -31995596.806464,
     -52901667.930264, 304240.877600},
    {1246029696.435966, 715822752.047222, 2695048219.141963, 2247369939.175317,
     2975389980.062112, 1408193367.925337, -35744020.900772, -27358392.093596,
     -55889969.250132, 314856.893400},
    {643749082.370985, 684281600.109327, 1123684969.587659, 1431645504.094443,
     1408193367.925337, 1133134428.937261, -20608122.768028, -21954408.705628,
     -27358392.093596, 191796.656200},
    {-16768448.088310, -10977204.352814, -27944984.625066, -27358392.093596,
     -35744020.900772, -20608122.768028, 471861.794000, 383593.312400,
     629713.786800, -3982.300000},
    {-10304061.384014, -14679835.627210, -26450833.965132, -31995596.806464,
     -27358392.093596, -21954408.705628, 383593.312400, 468856.619600,
     608481.755200, -4049.020000},
    {-17872010.450386, -15997798.403232, -63673369.811902, -52901667.930264,
     -55889969.250132, -27358392.093596, 629713.786800, 608481.755200,
     1255353.000400, -6674.500000},
    {117965.448500, 117214.154900, 313838.250100, 304240.877600, 314856.893400,
     191796.656200, -3982.300000, -4049.020000, -6674.500000, 40.000000}};

TEST_GROUP(MagnetometerCalibration){};

TEST(MagnetometerCalibration, TestComputeAggregatedReadings) {
    double test_calibration_values_dummy_data[40][3];
    const Matrix test_calibration_values(test_calibration_values_data,
                                         test_calibration_values_dummy_data);

    double aggregated_readings_expected_dummy_data[10][10];
    const Matrix aggregated_readings_expected(
        aggregated_readings_expected_data,
        aggregated_readings_expected_dummy_data);

    double initial_biases_bus_a_dummy_data[3][1];
    const Matrix initial_biases_bus_a(
        kPreFlightMagnetometerCalibrationBiasesImuBusA,
        initial_biases_bus_a_dummy_data);
    double initial_scale_factors_bus_a_dummy_data[3][3];
    const Matrix initial_scale_factors_bus_a(
        kPreFlightMagnetometerCalibrationScaleFactorsImuBusA,
        initial_scale_factors_bus_a_dummy_data);
    MagnetometerCalibration magnetometer_calibration(
        initial_biases_bus_a, initial_scale_factors_bus_a);
    magnetometer_calibration.AggregateReadings(test_calibration_values);
    for (uint8_t i = 0; i < 10; i++) {
        for (uint8_t j = 0; j < 10; j++) {
            DOUBLES_EQUAL(
                1,
                magnetometer_calibration.GetAggregatedReadings().Get(i, j) /
                    aggregated_readings_expected.Get(i, j),
                0.001);
        }
    }
}

TEST(MagnetometerCalibration, TestMagnetometerCalibration) {
    double test_calibration_values_dummy_data[40][3];
    const Matrix test_calibration_values(test_calibration_values_data,
                                         test_calibration_values_dummy_data);

    double biases_expected_dummy_data[3][1];
    const Matrix biases_expected(biases_expected_data,
                                 biases_expected_dummy_data);

    double scale_factors_expected_dummy_data[3][3];
    const Matrix scale_factors_expected(scale_factors_expected_data,
                                        scale_factors_expected_dummy_data);

    double initial_biases_bus_a_data[3][1];
    const Matrix initial_biases_bus_a(
        kPreFlightMagnetometerCalibrationBiasesImuBusA,
        initial_biases_bus_a_data);
    double initial_scale_factors_bus_a_data[3][3];
    const Matrix initial_scale_factors_bus_a(
        kPreFlightMagnetometerCalibrationScaleFactorsImuBusA,
        initial_scale_factors_bus_a_data);
    MagnetometerCalibration magnetometer_calibration(
        initial_biases_bus_a, initial_scale_factors_bus_a);

    uint8_t number_of_test_samples = 40;
    for (uint8_t i = 0; i < number_of_test_samples;
         i += MagnetometerCalibration::kBatchSizeInReadings) {
        magnetometer_calibration.AggregateReadings(test_calibration_values);
    }
    magnetometer_calibration.ComputeCalibrationParameters();

    for (uint8_t i = 0; i < 3; i++) {
        DOUBLES_EQUAL(biases_expected.Get(i, 0),
                      magnetometer_calibration.GetBiases().Get(i, 0), 0.001);
    }
    for (uint8_t i = 0; i < 3; i++) {
        for (uint8_t j = 0; j < 3; j++) {
            if (kUsePreFlightMagnetometerCalibrationScaleFactors) {
                DOUBLES_EQUAL(
                    kPreFlightMagnetometerCalibrationScaleFactorsImuBusA[i][j] /
                        1000,
                    magnetometer_calibration.GetScaleFactors().Get(i, j) / 1000,
                    0.001);
            } else {
                DOUBLES_EQUAL(
                    scale_factors_expected.Get(i, j) / 1000,
                    magnetometer_calibration.GetScaleFactors().Get(i, j) / 1000,
                    0.001);
            }
        }
    }
}
