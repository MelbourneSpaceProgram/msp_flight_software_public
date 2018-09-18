#ifndef SRC_PAYLOAD_PROCESSOR_RUNNABLE_CONSOLE_UART_LISTENER_H_
#define SRC_PAYLOAD_PROCESSOR_RUNNABLE_CONSOLE_UART_LISTENER_H_

#include <src/tasks/runnable.h>
#include <src/util/data_types.h>

class Uart;

class RunnableConsoleUartListener : public Runnable {
   public:
    RunnableConsoleUartListener(Uart* debug_uart);
    fnptr GetRunnablePointer();

   private:
    static void Listen();
    static bool ReadUart(byte* read_buffer, uint8_t size);
    static Uart* debug_uart;

    static const uint8_t kConsoleUartHeaderSize = 5;
};

#endif  // SRC_PAYLOAD_PROCESSOR_RUNNABLE_CONSOLE_UART_LISTENER_H_
