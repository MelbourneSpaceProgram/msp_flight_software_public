#ifndef SRC_MESSAGES_UPLINKS_ECHO_UPLINK_H_
#define SRC_MESSAGES_UPLINKS_ECHO_UPLINK_H_

#include <src/payload_processor/uplinks/uplink.h>
#include <src/telecomms/msp_payloads/echo_payload.h>

class EchoUplink : public Uplink {
   public:
    explicit EchoUplink(byte* payload);
    bool ExecuteUplink();

   private:
    EchoPayload response;
};

#endif /* SRC_MESSAGES_UPLINKS_ECHO_UPLINK_H_ */
