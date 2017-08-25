/* Contains pwm.cpp related defines and declarations */

#ifndef PWM_HPP
#define PWM_HPP

#define PERIOD 1000   /* period set to 1000 microseconds by default
                       * (at our discretion) */
#define NUM_INPUTS_1 3
#define NUM_INPUTS_2 2
#define SLEEP_PERIOD 1000 /* default period at which input is retrieved
                            (milliseconds) */
#define NUM_PWM 3       /* the number of pwm instances being run simultaneously */

void MSP_pwm_init(void);
void MSP_PWM_set_duty(int index, double duty_cycle);
uint16_t duty_in_usecs(double duty_percent);

#endif
