#ifndef SRC_SYSTEM_RUNNABLE_SYSTEM_HEALTH_CHECK_H_
#define SRC_SYSTEM_RUNNABLE_SYSTEM_HEALTH_CHECK_H_

#include <external/etl/exception.h>
#include <src/board/uart/uart.h>
#include <src/config/satellite.h>
#include <src/database/circular_buffer_nanopb.h>
#include <src/messages/Time.pb.h>
#include <src/sensors/measurable_manager.h>
#include <src/tasks/runnable.h>
#include <src/util/etl_utils.h>
#include <src/util/nanopb_utils.h>
#include <stdio.h>
#include <xdc/runtime/Log.h>

#define LogMeasurableMacro(NanopbMessageType)                          \
    RunnableSystemHealthCheck::LogMeasurable<NanopbMessageType,        \
                                             NanopbMessageType##_size, \
                                             NanopbMessageType##_fields>

class Uart;

class RunnableSystemHealthCheck : public Runnable {
    friend class RunnableConsoleUartListener;

   public:
    fnptr GetRunnablePointer();
    RunnableSystemHealthCheck(Uart* debug_uart);
    static void WriteToDataLogger(uint8_t measurable_id, byte encoded_message[],
                                  uint8_t message_size);
    static bool IsEnabled();
    static void EnableDatalogger(bool enable_logger);
    static void Init();

    static const uint8_t kMeasurableLoggerSyncChar1 = 0xCA;
    static const uint8_t kMeasurableLoggerSyncChar2 = 0xFE;
    static const uint32_t kHealthCheckPeriodMillis = 1000;

   private:
    static bool datalogger_enabled;
    static Uart* debug_uart;
    static void SystemHealthCheck();
    static const uint32_t kCircularBufferMessageLength = 10000;

    template <typename NanopbMessageType, uint16_t NanopbMessageType_size,
              const pb_field_t* NanopbMessageType_fields>
    static void LogMeasurable(uint16_t id) {
        MeasurableManager* manager = MeasurableManager::GetInstance();
        NanopbMessageType pb_reading =
            manager->ReadNanopbMeasurable<NanopbMessageType>(id, 0);
        size_t size;
        pb_get_encoded_size(&size, NanopbMessageType_fields, &pb_reading);

        if (kLogToSd && kSdCardAvailable) {
            char file_name[4];
            snprintf(file_name, sizeof(file_name), "%03d", id);
            try {
                CircularBufferNanopb<NanopbMessageType, NanopbMessageType_size,
                                     NanopbMessageType_fields>::
                    Create(file_name, kCircularBufferMessageLength);
                CircularBufferNanopb<
                    NanopbMessageType, NanopbMessageType_size,
                    NanopbMessageType_fields>::WriteMessage(file_name,
                                                            pb_reading);
            } catch (etl::exception& e) {
                EtlUtils::LogException(e);
                Log_error1("CircularBuffer failure for measurable id %d", id);
            }
        };

        if (kLogToUart) {
            assert(NanopbMessageType_size <= 255);
            byte buffer[255];
            try {
                NanopbEncode(NanopbMessageType)(buffer, pb_reading);
            } catch (etl::exception& e) {
                EtlUtils::LogException(e);
                Log_error1("Nanopb encode failed for measurable id %d", id);
            }
            WriteToDataLogger(id, buffer, size);
        }
    }
};

extern "C" {
void UartPutch(Char);
void UartFlush();
}

#endif  //  SRC_SYSTEM_RUNNABLE_SYSTEM_HEALTH_CHECK_H_
