/*
 * nadir_controller.h
 *
 *  Created on: 16Apr.,2018
 *      Author: jackm_000
 */

#ifndef CONTROLLERS_NADIR_CONTROLLER_H_
#define CONTROLLERS_NADIR_CONTROLLER_H_
#include <src/utils/matrix.h>


class NadirController {
    public:
    static void Control(const Matrix &b, const Matrix &nadir_est_body,
                        Matrix &torque_output);

    private:
    static const double max_achievable_dipole[3];
    static const double kp , kd, ki;

};

#endif /* CONTROLLERS_NADIR_CONTROLLER_H_ */
