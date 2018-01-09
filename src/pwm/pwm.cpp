/* Contains pwm related functions.
 *
 * Note: The array in GPIO.h (within msp... .c) is
 * an enumeration which is unused by the pwm functions. A seperate
 * enum can be found for the (above) pwm pins (by clicking into
 * Board_PWM0).
 */

// TODO(akremor): Get rid of redundant headers after code is working
#include <stdio.h>
#include <ti/drivers/GPIO.h>
#include <ti/drivers/PWM.h>
#include <src/pwm/pwm.hpp>
#include "Board.h"

PWM_Handle pwm[NUM_PWM];
PWM_Params params[NUM_PWM];
// TODO(akremor): Abstract once it is working
void MSP_pwm_init(void) {
    int pin_pwm[NUM_PWM], i; /* pwm pin , polarity pin, polarity */
    uint16_t period = PERIOD;
    uint16_t duty[NUM_PWM];
    double duty_percent[NUM_PWM];

    Board_initGPIO();

    /* getting pin and port input and initialising each pwm */
    for (i = 0; i < NUM_PWM; i++) {
        pin_pwm[i] = i;
        duty_percent[i] = 0;
        duty[i] = duty_in_usecs(duty_percent[i]);

        /* Sleep time in microseconds */
        pwm[i] = NULL;

        PWM_Params_init(&params[i]);
        params[i].dutyUnits = PWM_DUTY_US;
        params[i].dutyValue = duty[i];
        params[i].periodUnits = PWM_PERIOD_US;
        params[i].periodValue = period;

        pwm[i] = PWM_open(pin_pwm[i], &params[i]);
        /* refer to Note 1 */
        if (pwm[i] == NULL) {
            /* pin1 didn't open */
            // TODO(akremor): Add error handling and log output
            while (1) {
            }
        }

        PWM_start(pwm[i]);
    }
}

void MSP_PWM_set_duty(int index, double duty_cycle) {
    PWM_setDuty(pwm[index], duty_in_usecs(duty_cycle));
}

// returns the duty in microseconds given the duty in percentage
uint16_t duty_in_usecs(double duty_percent) { return PERIOD * duty_percent; }
