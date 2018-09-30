#ifndef SRC_BOARD_DEBUG_INTERFACE_DEBUG_STREAM_H_
#define SRC_BOARD_DEBUG_INTERFACE_DEBUG_STREAM_H_

#include <src/board/uart/uart.h>
#include <src/util/data_types.h>
#include <src/util/nanopb_utils.h>
#include <ti/sysbios/knl/Semaphore.h>

#define PostNanopbToSimMacro(NanopbMessageType, message_code, nanopb_message) \
    DebugStream::GetInstance()                                                \
        ->PostNanopb<NanopbMessageType, NanopbMessageType##_size,             \
                     NanopbMessageType##_fields>(                             \
            message_code, NanopbMessageType##_size, nanopb_message)

#define RequestNanopbFromSimMacro(NanopbMessageType, message_code)   \
    DebugStream::GetInstance()                                       \
        ->RequestNanopb<NanopbMessageType, NanopbMessageType##_size, \
                        NanopbMessageType##_fields>(message_code,    \
                                                    NanopbMessageType##_size)

class DebugStream {
   public:
    static DebugStream *GetInstance();  // Initial call is not thread safe
    bool RequestMessageFromSimulator(byte message_code, byte *response_buffer,
                                     uint8_t encoded_response_size);
    void PostMessageToDebugClient(byte message_code, uint8_t payload_size,
                                  byte *payload);

    template <class NanopbMessageType, uint8_t NanopbMessageType_size,
              const pb_field_t *NanopbMessageType_fields>
    void PostNanopb(const byte message_code, const uint8_t encoded_message_size,
                    const NanopbMessageType &nanopb_message) {
        byte buffer[encoded_message_size];
        NanopbEncode(NanopbMessageType)(buffer, nanopb_message);
        PostMessageToDebugClient(message_code, encoded_message_size, buffer);
    }

    template <class NanopbMessageType, uint8_t NanopbMessageType_size,
              const pb_field_t *NanopbMessageType_fields>
    NanopbMessageType RequestNanopb(const byte message_code,
                                    const uint8_t encoded_response_size) {
        byte response_buffer[encoded_response_size];
        bool success = RequestMessageFromSimulator(
            message_code, response_buffer, encoded_response_size);
        if (success != 1) {
            etl::exception e("Failed to request message from simulator",
                             __FILE__, __LINE__);
            throw e;
        }
        return NanopbDecode(NanopbMessageType)(response_buffer);
    }

   private:
    Semaphore_Params bus_available_params;
    Semaphore_Handle bus_available;
    Uart debug_uart;
    DebugStream();
    static DebugStream *instance;

    static constexpr uint32_t kTimeoutMillis = 1000;

    static constexpr byte kSyncByte1 = 0xAF;
    static constexpr byte kSyncByte2 = 0xFA;
};

#endif  // SRC_BOARD_DEBUG_INTERFACE_DEBUG_STREAM_H_
