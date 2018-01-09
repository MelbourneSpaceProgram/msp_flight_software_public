/* Contains pwm.cpp related defines and declarations */

#ifndef SRC_PWM_PWM_HPP_
#define SRC_PWM_PWM_HPP_

#define PERIOD 1000  // measured in microseconds
#define NUM_INPUTS_1 3
#define NUM_INPUTS_2 2
#define SLEEP_PERIOD 1000  // measured in microseconds
#define NUM_PWM 3  // the number of pwm instances being run simultaneously

void MSP_pwm_init(void);
void MSP_PWM_set_duty(int index, double duty_cycle);
uint16_t duty_in_usecs(double duty_percent);

#endif  // SRC_PWM_PWM_HPP_
