#include <src/tasks/tasks.hpp>
#include <xdc/std.h>
#include <ti/sysbios/knl/Task.h>
#include <src/uart/uart_configuration.hpp>
#include <src/uart/uart.hpp>
#include <src/CDH/util/DebugStream.h>
#include <src/messages/SerialisedMessage.h>
#include <src/messages/TestMessage.h>
#include <src/CDH/util/SerialisedMessageBuilder.h>
#include "Board.h"
#define UARTA0 MSP_EXP432P401R_UARTA0
#define UARTA2 MSP_EXP432P401R_UARTA2

Task_Struct debugStream_task_struct;
Char debugStream_stack[2048];

void sendTestMessage(DebugStream debug_stream, char data) {
    TestMessage msg(data);
    SerialisedMessage serial_msg = msg.Serialise();
    debug_stream.SendMessage(serial_msg);
}

void *debugStream(){
    DebugStream debug_stream;

    while(1){
        uint8_t code = debug_stream.ReceiveCode();
        switch(code) {
            case SerialisedMessageBuilder::kTemperatureSensor: {
                TemperatureMessage temp_msg(220.0, 44, 50);
                SerialisedMessage serial_msg = temp_msg.Serialise();
                debug_stream.SendMessage(serial_msg);
            } break;

            case SerialisedMessageBuilder::kRadiationSensor: {
                // TODO
            } break;

            case SerialisedMessageBuilder::kTestSensor: {
                sendTestMessage(debug_stream, 0xF0);
            } break;

            default: {
                sendTestMessage(debug_stream, 0xFF); // TODO: Replace with invalid request message
            } break;
        }
        Task_sleep(500);
    }
}

void InitTasks() {

    Task_Params taskParams;
    Task_Params_init(&taskParams);
    taskParams.stackSize = 2048; // TODO
    taskParams.instance->name = "debugStream";
    taskParams.stack = &debugStream_stack;
    taskParams.priority = 6;

    Task_construct(&debugStream_task_struct, (Task_FuncPtr) debugStream, &taskParams, NULL);
    Task_Handle task = Task_handle(&debugStream_task_struct);

    //InitMCP9808ReadTask();
}
