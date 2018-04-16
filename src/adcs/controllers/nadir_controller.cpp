#include <src/adcs/controllers/nadir_controller.h>
#include <srx/util/physical_constans.h>

const double NadirController::max_achievable_dipole[] = {0.35, 0.35, 0.05};
const double NadirController::kd = 1;
const double NadirController::kp = 1;

void NadirController::Control(const Matrix &b, const Matrix &nadir_error_quart,
                              const Matrix &omega , Matrix &dipole_ouput )

    {
        if (false)
        {
            //some useful exception thrown
        }
        double error_vector_data[3];

        for(int i = 0; i<3;i++)
        {
            error_vector_data[i] = nadir_error_quart.Get(i+1,0);
        }
        Matrix error_vector(error_vector_data);
        

    }




