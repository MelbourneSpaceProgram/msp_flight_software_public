#include <external/etl/exception.h>
#include <math.h>
#include <src/adcs/magnetorquer_control.h>
#include <src/board/board.h>
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

PWM_Handle MagnetorquerControl::pwm_handle_axis_x = NULL;
PWM_Handle MagnetorquerControl::pwm_handle_axis_y = NULL;
PWM_Handle MagnetorquerControl::pwm_handle_axis_z = NULL;

void MagnetorquerControl::Initialize() { InitializePwm(); }

void MagnetorquerControl::SetMagnetorquersPowerFraction(float x, float y,
                                                        float z) {
    if (hil_available) {
        PushDebugMessage(x, y, z);
    }

    if (kMagnetorquerHardwareEnabled) {
        // Set X axis
        SetPolarity(kMagnetorquerAxisX, x >= 0);
        SetMagnitude(kMagnetorquerAxisX, fabsf(x));

        // Set Y value
        SetPolarity(kMagnetorquerAxisY, y >= 0);
        SetMagnitude(kMagnetorquerAxisY, fabsf(y));

        // Set Z value
        SetPolarity(kMagnetorquerAxisZ, z >= 0);
        SetMagnitude(kMagnetorquerAxisZ, fabsf(z));
    }
}

void MagnetorquerControl::InitializePwm() {
    pwm_handle_axis_x = NULL;
    pwm_handle_axis_y = NULL;
    pwm_handle_axis_z = NULL;

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
    pwm_handle_axis_x = PWM_open(kMagnetorquerPWMAxisX, &params);
    if (pwm_handle_axis_x == NULL) {
        throw etl::exception("X Axis PWM did not open", __FILE__, __LINE__);
    }

    pwm_handle_axis_y = PWM_open(kMagnetorquerPWMAxisY, &params);
    if (pwm_handle_axis_y == NULL) {
        throw etl::exception("Y Axis PWM did not open", __FILE__, __LINE__);
    }

    pwm_handle_axis_z = PWM_open(kMagnetorquerPWMAxisZ, &params);
    if (pwm_handle_axis_z == NULL) {
        throw etl::exception("Z Axis PWM did not open", __FILE__, __LINE__);
    }

    // Start all PWMs
    PWM_start(pwm_handle_axis_x);
    PWM_start(pwm_handle_axis_y);
    PWM_start(pwm_handle_axis_z);
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

    if (axis == kMagnetorquerAxisX) {
        GPIO_write(kMagnetorquerPolarityGpioAxisX, polarity);
    } else if (axis == kMagnetorquerAxisY) {
        GPIO_write(kMagnetorquerPolarityGpioAxisY, polarity);
    } else if (axis == kMagnetorquerAxisZ) {
        GPIO_write(kMagnetorquerPolarityGpioAxisZ, polarity);
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
    if (axis == kMagnetorquerAxisX) {
        pwm_handle = pwm_handle_axis_x;
    } else if (axis == kMagnetorquerAxisY) {
        pwm_handle = pwm_handle_axis_y;
    } else if (axis == kMagnetorquerAxisZ) {
        pwm_handle = pwm_handle_axis_z;
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
        // Positive power
        SetMagnetorquersPowerFraction(power, power, power);
        // Wait for timer
        // TODO(rskew): It is possible for this to be null due to how the
        // semaphore is initialised. I (akremor) have temporarily modded it so
        // the code doesn't break but we need to revisit the creation of the
        // semaphore. Perhaps dependency injection. There is a race condition
        // because this check can be called before the semaphore has been
        // initialised.
        if (degaussing_timer_semaphore) {
            Semaphore_pend(degaussing_timer_semaphore, BIOS_WAIT_FOREVER);
            // Negative power
            SetMagnetorquersPowerFraction(-power, -power, -power);
            // Update power and wait for timer
            power = power * kDegaussingDecayMultiplier;
            Semaphore_pend(degaussing_timer_semaphore, BIOS_WAIT_FOREVER);
        } else {
            SetMagnetorquersPowerFraction(-power, -power, -power);
            power = power * kDegaussingDecayMultiplier;
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
