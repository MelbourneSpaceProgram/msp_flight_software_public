#include <src/telecomms/lithium.h>
#include <src/telecomms/lithium_commands/no_op_command.h>
#include <src/telecomms/runnable_lithium.h>
#include <ti/sysbios/knl/Task.h>

RunnableLithium::RunnableLithium() {}

fnptr RunnableLithium::GetRunnablePointer() { return &LithiumStream; }

void RunnableLithium::LithiumStream() {
    Lithium *lithium = Lithium::GetInstance();
    while (true) {
        NoOpCommand test_command;
        lithium->SendCommand(&test_command);
        Task_sleep(500);
    }
}
