#include <src/payload_processor/payload_processor.h>
#include <src/payload_processor/runnable_payload_processor.h>
#include <src/tasks/runnable.h>
#include <src/telecomms/lithium.h>
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Mailbox.h>

RunnablePayloadProcessor::RunnablePayloadProcessor() {}

fnptr RunnablePayloadProcessor::GetRunnablePointer() {
    return &RunnablePayloadProcessor::ExecuteCommandsInLithiumPayload;
}

void RunnablePayloadProcessor::ExecuteCommandsInLithiumPayload() {
    PayloadProcessor payload_processor;
    byte lithium_payload[Lithium::kMaxReceivedUplinkSize];
    Mailbox_Handle payload_mailbox_handle =
        Lithium::GetInstance()->GetUplinkMailbox();

    while (1) {
        Mailbox_pend(payload_mailbox_handle, &lithium_payload,
                     BIOS_WAIT_FOREVER);

        // TODO(dingbenjamin): Parse 4 byte MSP header
        // TODO(dingbenjamin): Check sequence number/security

        // TODO(): Determine why the 24 is required to index out of the header
        // Suspect the Lithium is forwarding the AX.25 header along as we are
        // seeing the callsign come in
        byte *msp_payload =
            lithium_payload + kAx25Bytes + kSignatureBytes + kLengthBytes + kSequenceNumberBytes;
        payload_processor.ParseAndExecuteCommands(msp_payload);
    }
}
