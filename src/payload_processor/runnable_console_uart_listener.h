#ifndef SRC_PAYLOAD_PROCESSOR_RUNNABLE_CONSOLE_UART_LISTENER_H_
#define SRC_PAYLOAD_PROCESSOR_RUNNABLE_CONSOLE_UART_LISTENER_H_

#include <src/tasks/runnable.h>
#include <src/util/data_types.h>

// TODO(dingbenjamin): Make generic so can receive different messages other than
// just those destined for the payload processor, can combine with the data
// dashbaord as well
class RunnableConsoleUartListener : public Runnable {
   public:
    RunnableConsoleUartListener();
    fnptr GetRunnablePointer();

   private:
    static void Listen();
    static bool ReadUart(byte* read_buffer, uint8_t size);
};

#endif  // SRC_PAYLOAD_PROCESSOR_RUNNABLE_CONSOLE_UART_LISTENER_H_
