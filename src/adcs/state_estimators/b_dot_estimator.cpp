#include <external/etl/exception.h>
#include <src/adcs/state_estimators/b_dot_estimator.h>
#include <src/util/physical_constants.h>

BDotEstimator::BDotEstimator(uint16_t sample_period_millis,
                             uint16_t time_constant_millis)
    : smoother_x(time_constant_millis, sample_period_millis),
      smoother_y(time_constant_millis, sample_period_millis),
      smoother_z(time_constant_millis, sample_period_millis),
      differentiator_x(sample_period_millis),
      differentiator_y(sample_period_millis),
      differentiator_z(sample_period_millis){};

void BDotEstimator::Estimate(const Matrix &magnetometer_reading,
                             Matrix &estimate_output) {
    if (!estimate_output.SameSize(magnetometer_reading) ||
        estimate_output.GetNRows() != geomagnetic_field_vector_nrows ||
        estimate_output.GetNColumns() != geomagnetic_field_vector_ncolumns) {
        etl::exception e("BDotEstimator::Estimate arguments not 3x1",
                         "__FILE__", __LINE__);
        throw e;
    }
    estimate_output.Set(0, 0,
                        differentiator_x.ProcessSample(smoother_x.ProcessSample(
                            magnetometer_reading.Get(0, 0))));
    estimate_output.Set(1, 0,
                        differentiator_y.ProcessSample(smoother_y.ProcessSample(
                            magnetometer_reading.Get(1, 0))));
    estimate_output.Set(2, 0,
                        differentiator_z.ProcessSample(smoother_z.ProcessSample(
                            magnetometer_reading.Get(2, 0))));
}
