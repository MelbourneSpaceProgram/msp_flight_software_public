#include <src/tasks/runnable.h>
#include <src/telecomms/lithium.h>
#include <ti/sysbios/knl/Mailbox.h>
#include <ti/sysbios/BIOS.h>
#include <src/payload_processor/runnable_payload_processor.h>
#include <src/payload_processor/payload_processor.h>

RunnablePayloadProcessor::RunnablePayloadProcessor() {}

fnptr RunnablePayloadProcessor::GetRunnablePointer() {
    return &RunnablePayloadProcessor::ExecuteCommandsInLithiumPayload;
}

void RunnablePayloadProcessor::ExecuteCommandsInLithiumPayload() {
    PayloadProcessor payload_processor;
    byte payload[Lithium::kMaxReceivedSize];
    Mailbox_Handle payload_mailbox_handle =
        Lithium::GetInstance()->GetMessageMailbox();

    while (1) {
        Mailbox_pend(payload_mailbox_handle, &payload, BIOS_WAIT_FOREVER);
        payload_processor.ParseAndExecuteCommands(payload);
    }
}
