#ifndef SRC_DATABASE_CIRCULAR_BUFFER_NANOPB_H_
#define SRC_DATABASE_CIRCULAR_BUFFER_NANOPB_H_

#include <external/etl/exception.h>
#include <external/nanopb/pb_decode.h>
#include <external/nanopb/pb_encode.h>
#include <src/database/hamming_coder.h>
#include <src/database/sd_raid.h>
#include <src/messages/pb.h>
#include <src/util/data_types.h>
#include <src/util/nanopb_utils.h>

/*
  Circular Buffer abstraction backed by sd card files.

  Each circular buffer is made to store one kind of nanopb message,
  taking care of the serialisation and deserialisation via nanopb and
  Hamming encoding/decoding for single bit-flip redundancy.

  A circular buffer is just a file beginning with:

  - uint32_t buffer_length_in_messages
  - uint32_t count_messages_written
  - uint32_t write_index
  - uint32_t read_index_bytes

  count_messages_written is here in case the size of the file can't be got from
  fsize. If it can, then count_messages_written is redundant.

  All data is Hamming encoded for single bit error correction, which doubles the
  stored size.
*/

// Macro for allowing the templated class access to the NanopbMessageType_size
// and NanopbMessageType_fields data without having to explicitly name them
// every time a method is called.
#define CircularBufferNanopb(NanopbMessageType)                       \
    CircularBufferNanopb<NanopbMessageType, NanopbMessageType##_size, \
                         NanopbMessageType##_fields>

typedef struct CircularBufferHeader {
    uint32_t buffer_length_in_messages;
    uint32_t count_messages_written;
    uint32_t write_index;
    uint32_t read_index;
} CircularBufferHeader;

template <typename NanopbMessageType, uint16_t NanopbMessageType_size,
          const pb_field_t *NanopbMessageType_fields>
class CircularBufferNanopb {
   public:
    static void Create(const char *file_name,
                       uint32_t buffer_length_in_messages) {
        if (SdRaid::GetInstance()->FileCreate(file_name)) {
            // TODO(akremor): Need to develop a fix for the below
            // It is API version dependent which means it breaks some builds
            // uint32_t file_size_in_bytes = file_name.fsize;
            uint32_t file_size_in_bytes = 0;
            // If file is empty, initialise metadata
            if (file_size_in_bytes < kEncodedHeaderSize) {
                SetBufferLengthInMessages(file_name, buffer_length_in_messages);
                SetCountMessagesWritten(file_name, 0);
                SetWriteIndex(file_name, kEncodedHeaderSize);
                SetReadIndex(file_name, kEncodedHeaderSize);
            }
        }
    }

    static void WriteMessage(const char *file_name,
                             const NanopbMessageType message) {
        // Serialise
        byte buffer[NanopbMessageType_size];
        NanopbEncode(NanopbMessageType)(buffer, message);

        // Write to file
        uint32_t write_index = GetWriteIndex(file_name);
        for (uint16_t i = 0; i < NanopbMessageType_size; i++) {
            WriteByte(file_name, buffer[i], write_index + i * 2);
        }
        uint32_t count_messages_written = GetCountMessagesWritten(file_name);
        // Check for overflow
        if (count_messages_written + 1 != 0) {
            SetCountMessagesWritten(file_name, count_messages_written + 1);
        }

        // Increment write index and wrap
        write_index += 2 * NanopbMessageType_size;
        uint32_t buffer_length_in_messages =
            GetBufferLengthInMessages(file_name);
        if (write_index >
            2 * (buffer_length_in_messages - 1) * NanopbMessageType_size +
                kEncodedHeaderSize) {
            write_index = kEncodedHeaderSize;
        }
        SetWriteIndex(file_name, write_index);
    }

    static NanopbMessageType ReadMessage(const char *file_name) {
        // Read from file
        uint32_t read_index = GetReadIndex(file_name);
        byte hamming_encoded_buffer[2 * NanopbMessageType_size];
        SdRaid::GetInstance()->FileRead(file_name, hamming_encoded_buffer,
                                        read_index, 2 * NanopbMessageType_size);

        // Increment write index and wrap
        read_index += 2 * NanopbMessageType_size;
        uint32_t buffer_length_in_messages =
            GetBufferLengthInMessages(file_name);
        uint32_t count_messages_written = GetCountMessagesWritten(file_name);
        if (read_index >
                2 * (buffer_length_in_messages - 1) * NanopbMessageType_size +
                    kEncodedHeaderSize ||
            read_index > 2 * count_messages_written * NanopbMessageType_size +
                             kEncodedHeaderSize) {
            read_index = kEncodedHeaderSize;
        }
        SetReadIndex(file_name, read_index);

        // Hamming decode
        byte hamming_decoded_buffer[NanopbMessageType_size];
        bool valid_decodings[NanopbMessageType_size];
        HammingCoder::DecodeByteArray(
            hamming_decoded_buffer, NanopbMessageType_size, valid_decodings,
            hamming_encoded_buffer, 2 * NanopbMessageType_size);
        for (uint16_t i = 0; i < NanopbMessageType_size; i++) {
            if (valid_decodings[i] == false) {
                etl::exception e("Failed to decode hamming encoded message",
                                 __FILE__, __LINE__);
                throw e;
            }
        }

        // Deserialise
        NanopbMessageType message_struct =
            NanopbDecode(NanopbMessageType)(hamming_decoded_buffer);

        return message_struct;
    }

    static uint32_t ReadCountMessagesWritten(const char *file_name) {
        uint32_t count_messages_written = GetCountMessagesWritten(file_name);
        return count_messages_written;
    }

    static CircularBufferHeader ReadHeader(const char *file_name) {
        uint32_t buffer_length_in_messages =
            GetBufferLengthInMessages(file_name);
        uint32_t count_messages_written = GetCountMessagesWritten(file_name);
        uint32_t write_index = GetWriteIndex(file_name);
        uint32_t read_index = GetReadIndex(file_name);
        CircularBufferHeader header = {buffer_length_in_messages,
                                       count_messages_written, write_index,
                                       read_index};
        return header;
    }

    static NanopbMessageType Search(const char *file_name,
                                    uint64_t timestamp_ms) {
        uint32_t saved_read_index = GetReadIndex(file_name);
        uint32_t messages_written = GetCountMessagesWritten(file_name);
        if (messages_written == 0) {
            throw etl::exception(
                "No messages in the circular buffer being searched", __FILE__,
                __LINE__);
        }

        SetReadIndex(file_name, kEncodedHeaderSize);
        NanopbMessageType reading =
            CircularBufferNanopb(NanopbMessageType)::ReadMessage(file_name);

        while (GetReadIndex(file_name) < messages_written &&
               reading.timestamp_ms < timestamp_ms) {
            reading =
                CircularBufferNanopb(NanopbMessageType)::ReadMessage(file_name);
        }
        // Will return the first reading after passing the requested time
        // or the last reading, if none exists
        SetReadIndex(file_name, saved_read_index);
        return reading;
    }

   private:
    static void WriteByte(const char *file_name, byte unencoded_byte,
                          uint32_t index) {
        HammingEncodedByte hamming_encoded_byte;
        hamming_encoded_byte = HammingCoder::Encode(unencoded_byte);
        SdRaid::GetInstance()->FileWrite(
            file_name, hamming_encoded_byte.codewords, index, 2);
    }

    static uint32_t GetBufferLengthInMessages(const char *file_name) {
        // buffer_length_in_messages is the first encoded uint32_t in the file,
        // starting byte 0
        return ReadUint32_t(file_name, 0);
    }

    static void SetBufferLengthInMessages(const char *file_name,
                                          uint32_t buffer_length_in_messages) {
        // buffer_length_in_messages is the first encoded uint32_t in the
        // file, starting byte 0
        WriteUint32_t(file_name, 0, buffer_length_in_messages);
    }

    static uint32_t GetCountMessagesWritten(const char *file_name) {
        // count_messages_written is the second encoded uint32_t in the
        // file, starting byte 8
        return ReadUint32_t(file_name, 2 * sizeof(uint32_t));
    }

    static void SetCountMessagesWritten(const char *file_name,
                                        uint32_t count_messages_written) {
        // count_messages_written is the second encoded uint32_t in
        // the file, starting byte 8
        WriteUint32_t(file_name, 2 * 1 * sizeof(uint32_t),
                      count_messages_written);
    }

    static uint32_t GetWriteIndex(const char *file_name) {
        // write_index is the third encoded uint32_t in the
        // file, starting byte 16
        return ReadUint32_t(file_name, 2 * 2 * sizeof(uint32_t));
    }

    static void SetWriteIndex(const char *file_name, uint32_t write_index) {
        // write_index is the third encoded uint32_t in the
        // file, starting byte 16
        WriteUint32_t(file_name, 2 * 2 * sizeof(uint32_t), write_index);
    }

    static uint32_t GetReadIndex(const char *file_name) {
        // read_index is the fourth encoded uint32_t in
        // the file, starting byte 24
        return ReadUint32_t(file_name, 2 * 3 * sizeof(uint32_t));
    }

    static void SetReadIndex(const char *file_name, uint32_t read_index) {
        // read_index is the fourth encoded uint32_t
        // in the file, starting byte 24
        WriteUint32_t(file_name, 2 * 3 * sizeof(uint32_t), read_index);
    }

    static uint32_t ReadUint32_t(const char *file_name, uint32_t index_bytes) {
        byte byte_array[sizeof(uint32_t)];
        bool valid_decodings[sizeof(uint32_t)];
        byte hamming_encoded_byte_array[2 * sizeof(uint32_t)];
        // write_index is the third encoded uint32_t in
        // the file, starting byte 16
        SdRaid::GetInstance()->FileRead(file_name, hamming_encoded_byte_array,
                                        index_bytes, 2 * sizeof(uint32_t));
        HammingCoder::DecodeByteArray(
            byte_array, sizeof(uint32_t), valid_decodings,
            hamming_encoded_byte_array, 2 * sizeof(uint32_t));
        for (uint16_t i = 0; i < sizeof(uint32_t); i++) {
            if (valid_decodings[i] == false) {
                etl::exception e("Failed to decode hamming encoded message",
                                 __FILE__, __LINE__);
                throw e;
            }
        }
        return ByteArrayToUint32_t(byte_array);
    }

    static void WriteUint32_t(const char *file_name, uint32_t index,
                              uint32_t value_to_write) {
        byte write_buffer[4];
        Uint32_tToByteArray(value_to_write, write_buffer);
        for (uint8_t i = 0; i < sizeof(uint32_t); i++) {
            WriteByte(file_name, write_buffer[i], index + 2 * i);
        }
    }

    static void Uint32_tToByteArray(uint32_t uint, byte *byte_array) {
        byte_array[0] = 0xff & (uint >> (8 * 3));
        byte_array[1] = 0xff & (uint >> (8 * 2));
        byte_array[2] = 0xff & (uint >> (8 * 1));
        byte_array[3] = 0xff & (uint);
    }

    static uint32_t ByteArrayToUint32_t(byte *byte_array) {
        return (byte_array[0] << (8 * 3)) + (byte_array[1] << (8 * 2)) +
               (byte_array[2] << (8 * 1)) + (byte_array[3]);
    }

    static constexpr uint8_t kEncodedHeaderSize = 2 * 4 * sizeof(uint32_t);
};

#endif  // SRC_DATABASE_CIRCULAR_BUFFER_NANOPB_H_
