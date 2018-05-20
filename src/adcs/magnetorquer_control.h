#ifndef SRC_MAGNETORQUER_CONTROL_H_
#define SRC_MAGNETORQUER_CONTROL_H_

#include <external/etl/exception.h>
#include <ti/drivers/GPIO.h>
#include <ti/drivers/PWM.h>
#include <ti/sysbios/hal/Timer.h>
#include <ti/sysbios/knl/Semaphore.h>
#include "../board/board.h"

//
// Controls magnetorquer power output and polarity via on-chip PWM and GPIO
// control.
//
// PWM driver reference:
// http://dev.ti.com/tirex/content/simplelink_msp432p4_sdk_1_60_00_12/docs/tidrivers/doxygen/html/_p_w_m_8h.html
//
// GPIO driver reference:
// http://dev.ti.com/tirex/content/simplelink_msp432p4_sdk_1_60_00_12/docs/tidrivers/doxygen/html/_g_p_i_o_m_s_p432_8h.html
//

class MagnetorquerControl {
   public:
    static void Initialize();
    static void SetMagnetorquersPowerFraction(float x, float y, float z);

    static const float kMagnetorquerPowerMax = 1.0f;
    static const float kMagnetorquerPowerMin = -1.0f;
    static void Degauss();
    static void SetupDegaussingPolaritySwitchTimer();
    static void DegaussingTimerISR(UArg degaussing_timer_semaphore);

    static const uint32_t kDegaussingSwitchPeriodMicros = 100000;

   private:
    typedef enum MagnetorquerAxis {
        kMagnetorquerAxisX = 0x00,
        kMagnetorquerAxisY = 0x01,
        kMagnetorquerAxisZ = 0x02
    } MagnetorquerAxis;

    static void InitializePwm();
    static void SetPolarity(MagnetorquerControl::MagnetorquerAxis axis,
                            bool positive);
    static void SetMagnitude(MagnetorquerControl::MagnetorquerAxis axis,
                             float magnitude);
    static void PushDebugMessage(float x, float y, float z);

    static PWM_Handle pwm_handle_axis_x;
    static PWM_Handle pwm_handle_axis_y;
    static PWM_Handle pwm_handle_axis_z;

    // PWM Period in microseconds.
    //
    // The PWM clock runs off TIMER_A_CLOCKSOURCE_SMCLK, which is a 12MHz clock.
    // This gives a resulution of 1/12 uS = 0.083uS.
    //
    // With a period of 100uS, we have a duty resolution of 1/1200 of a period.
    static const uint16_t kMagnetorquerPWMPeriod = 100;

    static const uint8_t kMagnetorquerPWMAxisX = Mag_STR_CDH_FS_Out1;
    static const uint8_t kMagnetorquerPWMAxisY = Mag_STR_CDH_FS_Out2;
    static const uint8_t kMagnetorquerPWMAxisZ = Mag_STR_CDH_FS_Out3;

    static const uint32_t kMagnetorquerPolarityGpioAxisX = X_Mag_DIR_CDH_FS;
    static const uint32_t kMagnetorquerPolarityGpioAxisY = Y_Mag_DIR_CDH_FS;
    static const uint32_t kMagnetorquerPolarityGpioAxisZ = Z_Mag_DIR_CDH_FS;

    //
    // TODO(crozone):
    //
    // Make this reference an external pwm_and_gpio_enabled constant
    //
    static const bool kMagnetorquerHardwareEnabled = true;

    static Semaphore_Handle degaussing_timer_semaphore;
    static const uint16_t kDegaussingTimeConstantMillis = 300;

    // Compute the exponential decay multiplier from the time constant
    // and sample period:
    //     exp(-(2 * (double)kDegaussingSwitchPeriodMicros * 1e-6) /
    //         ((double)kDegaussingTimeConstantMillis * 1e-3));
    static const float kDegaussingDecayMultiplier = 0.513417119;

    // 3 time constants worth of exp decay:
    //     round((3 * (double)kDegaussingTimeConstantMillis * 1e-3) /
    //       ((double)kDegaussingSwitchPeriodMicros * 1e-6));
    static const uint16_t kNDegaussPulses = 9;
};

#endif  // SRC_MAGNETORQUER_CONTROL_H_
