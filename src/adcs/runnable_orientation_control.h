#ifndef SRC_ADCS_RUNNABLE_ORIENTATION_CONTROL_H_
#define SRC_ADCS_RUNNABLE_ORIENTATION_CONTROL_H_

#include <src/adcs/kalman_filter.h>
#include <src/adcs/state_estimators/b_dot_estimator.h>
#include <src/adcs/state_estimators/location_estimator.h>
#include <src/messages/LocationReading.pb.h>
#include <src/sensors/earth_sensor.h>
#include <src/tasks/runnable.h>
#include <src/util/matrix.h>
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Semaphore.h>

class RunnableOrientationControl : public Runnable {
   public:
    fnptr GetRunnablePointer();
    RunnableOrientationControl();
    static void SetupControlLoopTimer();
    static void OrientationControlTimerISR(
        UArg orientation_control_timer_semaphore);

   private:
    static void ControlOrientation();
    static bool Detumble(Matrix& signed_pwm_output);
    static bool PointToNadir(Matrix& gyro_reading, Matrix& signed_pwm_output);

    static KalmanFilter kalman_filter;
    static BDotEstimator b_dot_estimator;
    static FirstOrderIirLowpass angular_velocity_smoother;
    static EarthSensor earth_sensor;
    static LocationEstimator location_estimator;

    static Semaphore_Handle control_loop_timer_semaphore;
};

#endif  // SRC_ADCS_RUNNABLE_ORIENTATION_CONTROL_H_
