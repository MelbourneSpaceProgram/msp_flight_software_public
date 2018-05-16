#include <external/etl/exception.h>
#include <src/database/circular_buffer_nanopb.h>
#include <src/messages/TemperatureReading.pb.h>
#include <src/messages/Time.pb.h>
#include <src/sensors/i2c_measurable_manager.h>
#include <src/sensors/measurable_id.h>
#include <src/sensors/runnable_cache_write_back.h>
#include <src/util/data_types.h>
#include <src/util/satellite_time_source.h>
#include <src/util/task_utils.h>
#include <stdio.h>

RunnableCacheWriteBack::RunnableCacheWriteBack() {}

fnptr RunnableCacheWriteBack::GetRunnablePointer() { return &WriteBack; }

void RunnableCacheWriteBack::WriteBack() {
    while (1) {
        // TODO(dingbenjamin): Implement remaining measurables

        // CDH
        WriteBackTemp(kCdhTemp1);

        TaskUtils::SleepMilli(kWriteBackPeriod);
    }
}

void RunnableCacheWriteBack::WriteBackTemp(uint16_t measurable_id) {
    byte buffer[TemperatureReading_size];
    TemperatureReading temp_reading = TemperatureReading_init_zero;
    pb_ostream_t stream = pb_ostream_from_buffer(buffer, sizeof(buffer));
    I2cMeasurableManager* manager = I2cMeasurableManager::GetInstance();

    temp_reading.temp = manager->ReadI2cMeasurable<double>(
        measurable_id, kMaxCachedAgeMilli, true);
    // TODO(dingbenjamin): Nest time message instead of unwrapping
    temp_reading.timestamp_millis_unix_epoch =
        manager->GetMeasurableTimeStamp<double>(measurable_id)
            .timestamp_millis_unix_epoch;

    if (!pb_encode(&stream, TemperatureReading_fields, &temp_reading)) {
        throw etl::exception(
            "Cache WriteBack::TemperatureReading pb_encode failed", __FILE__,
            __LINE__);
    }

    // TODO(dingbenjamin): Potentially remove in order to remove stdio include
    char filename[3];
    snprintf(filename, sizeof(filename), "%03d", measurable_id);

    CircularBufferNanopb(TemperatureReading)::Create(filename,
                                                     kNumStoredReadings);
    CircularBufferNanopb(TemperatureReading)::WriteMessage(filename,
                                                           temp_reading);
}
