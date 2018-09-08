#ifndef SRC_SYSTEM_RUNNABLE_SYSTEM_HEALTH_CHECK_H_
#define SRC_SYSTEM_RUNNABLE_SYSTEM_HEALTH_CHECK_H_

#include <external/etl/exception.h>
#include <src/board/uart/uart.h>
#include <src/config/satellite.h>
#include <src/database/circular_buffer_nanopb.h>
#include <src/messages/Time.pb.h>
#include <src/sensors/i2c_measurable_manager.h>
#include <src/tasks/runnable.h>
#include <src/util/nanopb_utils.h>
#include <stdio.h>
#include <xdc/runtime/Log.h>

#define LogMeasurableMacro(RawType, NanopbMessageType)                   \
    RunnableSystemHealthCheck::LogMeasurable<RawType, NanopbMessageType, \
                                             NanopbMessageType##_size,   \
                                             NanopbMessageType##_fields>

class RunnableSystemHealthCheck : public Runnable {
   public:
    fnptr GetRunnablePointer();
    RunnableSystemHealthCheck();
    static void WriteToDataLogger(uint8_t measurable_id, byte encoded_message[],
                                  uint8_t message_size);
    static bool IsEnabled();
    static void EnableDatalogger(bool enable_logger);

   private:
    static bool datalogger_enabled;
    static Uart debug_uart;
    static void SystemHealthCheck();
    static const uint32_t kHealthCheckPeriodMillis = 30000;
    static const uint32_t kCircularBufferMessageLength = 10000;

    template <typename T, typename NanopbMessageType,
              uint16_t NanopbMessageType_size,
              const pb_field_t* NanopbMessageType_fields>
    static void LogMeasurable(uint16_t id,
                              NanopbMessageType (*raw_to_nanopb)(T, Time)) {
        I2cMeasurableManager* manager = I2cMeasurableManager::GetInstance();

        // TODO(dingbenjamin): Fix case where another task interrupts and
        // generates a new timestamp between the reading of the  measurable and
        // the getting of the timestamp
        T raw = manager->ReadI2cMeasurable<T>(id, 0);
        Time time =
            I2cMeasurableManager::GetInstance()->GetMeasurableTimeStamp<T>(id);

        // Convert the reading from the measurable manager to a nanopb
        // using the conversion function
        NanopbMessageType pb_reading = raw_to_nanopb(raw, time);

        size_t size;
        pb_get_encoded_size(&size, NanopbMessageType_fields, &pb_reading);

        if (kLogToSd) {
            char file_name[4];
            snprintf(file_name, sizeof(file_name), "%03d", id);
            // TODO(dingbenjamin): Exception handle here
            CircularBufferNanopb<
                NanopbMessageType, NanopbMessageType_size,
                NanopbMessageType_fields>::Create(file_name,
                                                  kCircularBufferMessageLength);
            CircularBufferNanopb<
                NanopbMessageType, NanopbMessageType_size,
                NanopbMessageType_fields>::WriteMessage(file_name, pb_reading);
        };

        if (kLogToUart) {
            assert(NanopbMessageType_size <= 255);
            byte buffer[255];
            try {
                NanopbEncode(NanopbMessageType)(buffer, pb_reading);
            } catch (etl::exception& e) {
                Log_error1("Nanopb encode failed for measurable id %d", id);
            }
            WriteToDataLogger(id, buffer, size);
        }
    }
    static const uint8_t kMeasurableLoggerSyncChar1 = 0xCA;
    static const uint8_t kMeasurableLoggerSyncChar2 = 0xFE;
};

#endif  //  SRC_SYSTEM_RUNNABLE_SYSTEM_HEALTH_CHECK_H_
