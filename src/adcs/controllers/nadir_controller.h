#ifndef CONTROLLERS_NADIR_CONTROLLER_H_
#define CONTROLLERS_NADIR_CONTROLLER_H_

class Matrix;

class NadirController {
   public:
    static void Control(const Matrix &nadir_error_quart, Matrix &omega,
                        Matrix &dipole_ouput);

   private:
    static const double max_achievable_dipole[3];
    static const double kp, kd, ki;
};

#endif /* CONTROLLERS_NADIR_CONTROLLER_H_ */
