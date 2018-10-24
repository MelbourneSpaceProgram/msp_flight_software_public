#ifndef RUNNABLE_CONSOLE_LOGGER_H_
#define RUNNABLE_CONSOLE_LOGGER_H_

#include <src/tasks/runnable.h>
#include <ti/sysbios/gates/GateMutexPri.h>

class Uart;

class RunnableConsoleLogger : public Runnable {
   public:
    RunnableConsoleLogger(Uart* debug_uart);
    fnptr GetRunnablePointer();
    static void WriteToDataLogger(uint8_t measurable_id, byte encoded_message[],
                                  uint8_t message_size);
    static constexpr uint8_t kMeasurableLoggerSyncChar1 = 0xCA;
    static constexpr uint8_t kMeasurableLoggerSyncChar2 = 0xFE;
	static constexpr uint16_t kMaxEncodedMessageLength = 100;
	static constexpr uint16_t kWaitTimeMs = 100;

   private:
    static void LogToConsole();
    static Uart* debug_uart;
    static bool initialised;
	static GateMutexPri_Params console_uart_mutex_params;
	static GateMutexPri_Handle console_uart_mutex;
};

extern "C" {
void UartPutch(char);
void UartFlush();
}

#endif  // RUNNABLE_CONSOLE_LOGGER_H_
