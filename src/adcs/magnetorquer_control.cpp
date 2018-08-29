#include <external/etl/exception.h>
#include <math.h>
#include <src/adcs/magnetorquer_control.h>
#include <src/board/board.h>
#include <src/config/satellite.h>
#include <src/config/unit_tests.h>
#include <src/data_dashboard/runnable_data_dashboard.h>
#include <src/messages/PwmOutputReading.pb.h>
#include <src/util/message_codes.h>
#include <ti/drivers/GPIO.h>
#include <ti/drivers/PWM.h>
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/hal/Timer.h>
#include <xdc/runtime/Log.h>

Semaphore_Handle MagnetorquerControl::degaussing_timer_semaphore;

PWM_Handle MagnetorquerControl::pwm_handle_axis_a = NULL;
PWM_Handle MagnetorquerControl::pwm_handle_axis_b = NULL;
PWM_Handle MagnetorquerControl::pwm_handle_axis_c = NULL;

void MagnetorquerControl::Initialize() { InitializePwm(); }

void MagnetorquerControl::SetMagnetorquersPowerFraction(float x, float y,
                                                        float z) {
    if (hil_available) {
        PushDebugMessage(x, y, z);
    }

    // Map from the body frame to the magnetorquer 'frame'.
    // Note: the magnetorquers may be orientated arbitrarily, and so the
    //   'a', 'b' and 'c' components may not make a right-handed coordinate
    //   system. Therefore, the transformation from the body frame to the
    //   magnetorquer 'frame' may not be a pure rotation.
    double magnetorquer_power_body_frame_data[3][1];
    Matrix magnetorquer_power_body_frame(magnetorquer_power_body_frame_data);
    magnetorquer_power_body_frame.Set(0, 0, x);
    magnetorquer_power_body_frame.Set(1, 0, y);
    magnetorquer_power_body_frame.Set(2, 0, z);
    double magnetorquer_power_magnetorquer_frame_data[3][1];
    Matrix magnetorquer_power_magnetorquer_frame(
        magnetorquer_power_magnetorquer_frame_data);
    magnetorquer_power_magnetorquer_frame.Multiply(
        kBodyToMagnetorquerFrameTransform, magnetorquer_power_body_frame);
    float a = magnetorquer_power_magnetorquer_frame.Get(0, 0);
    float b = magnetorquer_power_magnetorquer_frame.Get(1, 0);
    float c = magnetorquer_power_magnetorquer_frame.Get(2, 0);

    if (kMagnetorquerHardwareEnabled) {
        // Set A axis
        SetPolarity(kMagnetorquerAxisA, a >= 0);
        SetMagnitude(kMagnetorquerAxisA, fabsf(a));

        // Set B value
        SetPolarity(kMagnetorquerAxisB, b >= 0);
        SetMagnitude(kMagnetorquerAxisB, fabsf(b));

        // Set C value
        SetPolarity(kMagnetorquerAxisC, c >= 0);
        SetMagnitude(kMagnetorquerAxisC, fabsf(c));
    }
}

void MagnetorquerControl::InitializePwm() {
    pwm_handle_axis_a = NULL;
    pwm_handle_axis_b = NULL;
    pwm_handle_axis_c = NULL;

    // Setup the PWM parameters.
    PWM_Params params;
    PWM_Params_init(&params);

    // Output low when PWM is not running
    params.idleLevel = PWM_IDLE_LOW;
    params.periodUnits = PWM_PERIOD_US;
    // The period must be the same for all PWM instances, as per the
    // documentation. If the period is changed after opening, all other PWM
    // periods will also be set.
    params.periodValue = kMagnetorquerPWMPeriod;
    // The PWM duty is fractional and in the range [0, PWM_DUTY_FRACTION_MAX].
    params.dutyUnits = PWM_DUTY_FRACTION;
    params.dutyValue = 0;

    // Setup all PWMs
    pwm_handle_axis_a = PWM_open(kMagnetorquerPWMAxisA, &params);
    if (pwm_handle_axis_a == NULL) {
        throw etl::exception("A Axis PWM did not open", __FILE__, __LINE__);
    }

    pwm_handle_axis_b = PWM_open(kMagnetorquerPWMAxisB, &params);
    if (pwm_handle_axis_b == NULL) {
        throw etl::exception("B Axis PWM did not open", __FILE__, __LINE__);
    }

    pwm_handle_axis_c = PWM_open(kMagnetorquerPWMAxisC, &params);
    if (pwm_handle_axis_c == NULL) {
        throw etl::exception("C Axis PWM did not open", __FILE__, __LINE__);
    }

    // Start all PWMs
    PWM_start(pwm_handle_axis_a);
    PWM_start(pwm_handle_axis_b);
    PWM_start(pwm_handle_axis_c);
}

void MagnetorquerControl::PushDebugMessage(float x, float y, float z) {
    // Send torque output to simulation.
    PwmOutputReading pwm_output_reading = PwmOutputReading_init_zero;

    pwm_output_reading.x = x;
    pwm_output_reading.y = y;
    pwm_output_reading.z = z;

    RunnableDataDashboard::TransmitMessage(
        kPwmOutputReadingCode, PwmOutputReading_size, PwmOutputReading_fields,
        &pwm_output_reading);
}

void MagnetorquerControl::SetPolarity(MagnetorquerAxis axis, bool positive) {
    uint8_t polarity = (positive) ? 1 : 0;

    if (axis == kMagnetorquerAxisA) {
        GPIO_write(kMagnetorquerPolarityGpioAxisA, polarity);
    } else if (axis == kMagnetorquerAxisB) {
        GPIO_write(kMagnetorquerPolarityGpioAxisB, polarity);
    } else if (axis == kMagnetorquerAxisC) {
        GPIO_write(kMagnetorquerPolarityGpioAxisC, polarity);
    } else {
        throw etl::exception("Invalid axis", __FILE__, __LINE__);
    }
}

void MagnetorquerControl::SetMagnitude(MagnetorquerAxis axis, float magnitude) {
    // Clamp absolute power between [0, kMagnetorquerPowerMax]
    if (magnitude < 0) {
        magnitude = 0;
    } else if (magnitude > kMagnetorquerPowerMax) {
        magnitude = kMagnetorquerPowerMax;
    }

    // Get the PWM handle for the axis specified
    PWM_Handle pwm_handle;
    if (axis == kMagnetorquerAxisA) {
        pwm_handle = pwm_handle_axis_a;
    } else if (axis == kMagnetorquerAxisB) {
        pwm_handle = pwm_handle_axis_b;
    } else if (axis == kMagnetorquerAxisC) {
        pwm_handle = pwm_handle_axis_c;
    } else {
        throw etl::exception("Invalid axis", __FILE__, __LINE__);
    }

    // Set PWM duty.
    // Duty value is fractional. Range is [0, PWM_DUTY_FRACTION_MAX].
    uint32_t duty = static_cast<uint32_t>(
        round(PWM_DUTY_FRACTION_MAX *
              static_cast<double>(magnitude / kMagnetorquerPowerMax)));
    PWM_setDuty(pwm_handle, duty);
}

void MagnetorquerControl::Degauss() {
    float power = 1;
    for (uint8_t i = 0; i < kNDegaussPulses; i++) {
        if (degaussing_timer_semaphore) {
            // Wait for timer to tick
            Semaphore_pend(degaussing_timer_semaphore, BIOS_WAIT_FOREVER);
            // Positive power
            SetMagnetorquersPowerFraction(power, power, power);
            // Wait for timer to tick
            Semaphore_pend(degaussing_timer_semaphore, BIOS_WAIT_FOREVER);
            // Negative power
            SetMagnetorquersPowerFraction(-power, -power, -power);
            // Update power
            power = power * kDegaussingDecayMultiplier;
        } else {
            Log_warning("Trying to degauss before the timer semaphore has been
                         initialized");
        }
    }
}

void MagnetorquerControl::DegaussingTimerISR(UArg degaussing_timer_semaphore) {
    Semaphore_post((Semaphore_Handle)degaussing_timer_semaphore);
}

void MagnetorquerControl::SetupDegaussingPolaritySwitchTimer() {
    Timer_Handle degaussing_timer;
    // Potential issues with this object going out of scope:
    //   If the timer has parameters changed at runtime and
    //   this object is referred to internally, crashes might happen.
    Timer_Params degaussing_timer_params;
    Semaphore_Params degaussing_timer_semaphore_params;
    Semaphore_Params_init(&degaussing_timer_semaphore_params);
    degaussing_timer_semaphore_params.mode = Semaphore_Mode_BINARY;
    MagnetorquerControl::degaussing_timer_semaphore =
        Semaphore_create(0, &degaussing_timer_semaphore_params, NULL);
    Timer_Params_init(&degaussing_timer_params);
    degaussing_timer_params.period =
        MagnetorquerControl::kDegaussingSwitchPeriodMicros;
    degaussing_timer_params.arg = (UArg)degaussing_timer_semaphore;
    degaussing_timer =
        Timer_create(Board_TIMER3, MagnetorquerControl::DegaussingTimerISR,
                     &degaussing_timer_params, NULL);
    if (degaussing_timer == NULL) {
        etl::exception e("Timer create failed", __FILE__, __LINE__);
        throw e;
    }
}
