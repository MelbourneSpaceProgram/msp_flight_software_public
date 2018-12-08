#ifndef SRC_ADCS_MAGNETORQUER_CONTROL_H_
#define SRC_ADCS_MAGNETORQUER_CONTROL_H_

#include <src/board/board.h>
#include <ti/drivers/PWM.h>
#include <ti/sysbios/knl/Semaphore.h>

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

    static constexpr float kMagnetorquerPowerMax = 1.0f;
    static constexpr float kMagnetorquerPowerMin = -1.0f;
    static void Degauss();
    static void SetupDegaussingPolaritySwitchTimer();
    static void DegaussingTimerISR(UArg degaussing_timer_semaphore);

    static constexpr uint32_t kDegaussingSwitchPeriodMicros = 8000;

   private:
    typedef enum MagnetorquerAxis {
        kMagnetorquerAxisA = 0x00,
        kMagnetorquerAxisB = 0x01,
        kMagnetorquerAxisC = 0x02
    } MagnetorquerAxis;

    static void InitializePwm();
    static void SetPolarity(MagnetorquerControl::MagnetorquerAxis axis,
                            bool positive);
    static void SetMagnitude(MagnetorquerControl::MagnetorquerAxis axis,
                             float magnitude);
    static void PushDebugMessage(float x, float y, float z);

    static PWM_Handle pwm_handle_axis_a;
    static PWM_Handle pwm_handle_axis_b;
    static PWM_Handle pwm_handle_axis_c;

    // PWM Period in microseconds.
    //
    // The PWM clock runs off TIMER_A_CLOCKSOURCE_SMCLK, which is a 12MHz clock.
    // This gives a resulution of 1/12 uS = 0.083uS.
    //
    // With a period of 100uS, we have a duty resolution of 1/1200 of a period.
    static constexpr uint16_t kMagnetorquerPWMPeriod = 5;

    static constexpr uint8_t kMagnetorquerPWMAxisA = Mag_STR_CDH_FS_Out1;
    static constexpr uint8_t kMagnetorquerPWMAxisB = Mag_STR_CDH_FS_Out2;
    static constexpr uint8_t kMagnetorquerPWMAxisC = Mag_STR_CDH_FS_Out3;

    static constexpr uint32_t kMagnetorquerPolarityGpioAxisA = FS_A_DIR;
    static constexpr uint32_t kMagnetorquerPolarityGpioAxisB = FS_B_DIR;
    static constexpr uint32_t kMagnetorquerPolarityGpioAxisC = FS_C_DIR;

    static Semaphore_Handle degaussing_timer_semaphore;
    static const float kDegaussingDecayMultiplier;
    static const uint16_t kNDegaussPulses;
    static const uint16_t kNPulsesFudge;
    static const uint16_t kDegaussingTimeConstantMillis = 25;
    static const uint16_t kDegaussingPeriodMillis = 15;
};

#endif  // SRC_ADCS_MAGNETORQUER_CONTROL_H_
