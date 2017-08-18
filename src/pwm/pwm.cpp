/* Contains pwm related functions.
 *
 * Note: The array in GPIO.h (within msp... .c) is
 * an enumeration which is unused by the pwm functions. A seperate
 * enum can be found for the (above) pwm pins (by clicking into
 * Board_PWM0).
 */

// TODO: Get rid of redundant headers after code is working
#include <ti/sysbios/knl/Task.h>
#include <ti/drivers/PWM.h>
#include "Board.h"
#include <ti/drivers/GPIO.h>
#include <stdio.h>

#include "pwm.hpp"


// this is the core pwm function
// TODO: Abstract once it is working
void pwm_execute(void) {

    int         pin_pwm[NUM_PWM], pin_led[NUM_PWM], pol[NUM_PWM], i; /* pwm pin , polarity pin, polarity */
    uint16_t    period, duty[NUM_PWM];
    uint32_t    sleep_period = SLEEP_PERIOD;
    double      duty_percent[NUM_PWM];

    PWM_Handle pwm[NUM_PWM];
    PWM_Params params[NUM_PWM];

    period = PERIOD;
    Board_initGPIO();

    /* getting pin and port input and initialising each pwm */
    printf("Usage: pin_pwm    pin_led    initial_duty_percent\n");
    for (i = 0; i < NUM_PWM; i++) {
        printf("Enter information for pwm %d: ", i);
        if (scanf("%d %d %lf", &pin_pwm[i], &pin_led[i], &duty_percent[i])
                != NUM_INPUTS_1) {
            printf("Incorrect number of inputs\n");
            return;
        }
        if (duty_percent[i] < 0 || duty_percent[i] > 1) {
            printf("Enter a valid duty percentage between 0 and 1\n");
            return;
        }

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
            printf("Error\n");
            while (1);
        }

        /* initialising pin2 (high) */
        pol[i] = 1;

        PWM_start(pwm[i]);
    }



    /* Loop forever while getting a new duty at a particular frequency */
    while (1) {
        for (i = 0; i < NUM_PWM; i++) {
            PWM_setDuty(pwm[i], duty_in_usecs(duty_percent[i]));
            GPIO_write(pin_led[i], pol[i]);
        }
            //Task_sleep(sleep_period);
            /* (determines how often duty_percent and polarity are redefined) */
        printf("Usage: duty_percent    polarity\n");
        for (i = 0; i < NUM_PWM; i++) {
            printf("pwm %i: ", i);
            if (scanf("%lf %d", &duty_percent[i], &pol[i]) != NUM_INPUTS_2) {
                printf("Incorrect input\n");
                return;
            }
        }
    }

}


// returns the duty in microseconds given the duty in percentage
uint16_t duty_in_usecs(double duty_percent) {
    return PERIOD * duty_percent;
}
