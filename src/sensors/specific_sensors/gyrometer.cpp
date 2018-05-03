#include <external/nanopb/pb_decode.h>
#include <external/nanopb/pb_encode.h>
#include <src/adcs/controllers/b_dot_controller.h>
#include <src/adcs/state_estimators/b_dot_estimator.h>
#include <src/config/unit_tests.h>
#include <src/data_dashboard/runnable_data_dashboard.h>
#include <src/debug_interface/debug_stream.h>
#include <src/sensors/generic_measurable.h>
#include <src/sensors/specific_sensors/gyrometer.h>
#include <src/util/message_codes.h>
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Semaphore.h>

bool Gyrometer::TakeReading() {
    if (hil_enabled) {
        return TakeReadingHil();
    }
    // TODO(rskew): Implement the I2C read
    return false;
}

bool Gyrometer::TakeReadingHil() {
    DebugStream *debug_stream = DebugStream::GetInstance();
    uint8_t buffer[GyrometerReading_size];

    bool success = debug_stream->RequestMessageFromSimulator(kGyrometerReadingRequestCode,
                                              buffer, GyrometerReading_size);
    if (success){

        pb_istream_t stream =
            pb_istream_from_buffer(buffer, GyrometerReading_size);

        bool status = pb_decode(&stream, GyrometerReading_fields, &reading);
        if (!status) {
            etl::exception e("pb_decode failed",
                             __FILE__, __LINE__);
            throw e;
        }

        NotifyObservers();
    }
    return success;
}
