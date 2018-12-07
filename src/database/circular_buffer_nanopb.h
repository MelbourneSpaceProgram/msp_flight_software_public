#ifndef SRC_DATABASE_CIRCULAR_BUFFER_NANOPB_H_
#define SRC_DATABASE_CIRCULAR_BUFFER_NANOPB_H_

#include <external/nanopb/pb_decode.h>
#include <external/nanopb/pb_encode.h>
#include <src/database/eeprom.h>
#include <src/util/data_types.h>
#include <src/util/msp_exception.h>
#include <src/util/nanopb_utils.h>

/*
  Circular Buffer abstraction backed by EEPROM.

  Each circular buffer is made to store one kind of nanopb message,
  taking care of the serialisation and deserialisation via nanopb.

  A circular buffer is just a file beginning with:

  - uint16_t buffer_length_in_messages
  - uint16_t count_messages_written
  - uint16_t write_index_bytes
  - uint16_t read_index_bytes

  where going past the end of the file wraps back to the start of the file.

  count_messages_written is here in case the size of the file can't be got from
  fsize. If it can, then count_messages_written is redundant.

  All data is Hamming encoded by the EEPROM driver.
*/

// Macro for allowing the templated class access to the NanopbMessageType_size
// and NanopbMessageType_fields data without having to explicitly name them
// every time a method is called.
#define CircularBufferNanopb(NanopbMessageType)                       \
    CircularBufferNanopb<NanopbMessageType, NanopbMessageType##_size, \
                         NanopbMessageType##_fields>

typedef struct CircularBufferHeader {
    uint16_t buffer_length_in_messages;
    uint16_t count_messages_written;
    uint16_t write_index;
    uint16_t read_index;
} CircularBufferHeader;

template <typename NanopbMessageType, uint16_t NanopbMessageType_size,
          const pb_field_t *NanopbMessageType_fields>
class CircularBufferNanopb {
   public:
    static void CreateIfNecessary(uint16_t file_offset,
                                  uint16_t buffer_length_in_messages) {
        // If file is empty, initialise metadata
        CircularBufferHeader header = ReadHeader(file_offset);
        if (!ValidHeader(file_offset, header)) {
            ForceCreate(file_offset, buffer_length_in_messages);
        }
    }

    static void ForceCreate(uint16_t file_offset,
                            uint16_t buffer_length_in_messages) {
        SetBufferLengthInMessages(file_offset, buffer_length_in_messages);
        SetCountMessagesWritten(file_offset, 0);
        SetWriteIndex(file_offset, kHeaderSize);
        SetReadIndex(file_offset, kHeaderSize);
    }

    static void WriteMessage(uint16_t file_offset,
                             const NanopbMessageType message) {
        // Serialise
        byte buffer[NanopbMessageType_size];
        NanopbEncode(NanopbMessageType)(buffer, message);

        // Write to file
        uint16_t write_index_bytes = GetWriteIndex(file_offset);
        if (!Eeprom::Write(write_index_bytes, buffer, NanopbMessageType_size)) {
            throw MspException("Failed to write to EEPROM", kEepromWriteFailure,
                               __FILE__, __LINE__);
        }
        uint16_t count_messages_written = GetCountMessagesWritten(file_offset);
        // Check for overflow
        if (count_messages_written + 1 != 0) {
            SetCountMessagesWritten(file_offset, count_messages_written + 1);
        }

        // Increment write index and wrap
        write_index_bytes += NanopbMessageType_size;
        uint16_t buffer_length_in_messages =
            GetBufferLengthInMessages(file_offset);
        if (write_index_bytes > kHeaderSize + (buffer_length_in_messages - 1) *
                                                  NanopbMessageType_size) {
            write_index_bytes = kHeaderSize;
        }
        SetWriteIndex(file_offset, write_index_bytes);
    }

    static NanopbMessageType ReadMessage(uint16_t file_offset) {
        // Read from file
        uint16_t read_index_bytes = GetReadIndex(file_offset);
        byte read_buffer[NanopbMessageType_size];
        if (!Eeprom::Read(file_offset + read_index_bytes, read_buffer,
                          NanopbMessageType_size)) {
            throw MspException("Failed to read from EEPROM or decode data",
                               kEepromReadFailure, __FILE__, __LINE__);
        }
        NanopbMessageType nanopb_message =
            NanopbDecode(NanopbMessageType)(read_buffer);

        // Increment read index and wrap
        read_index_bytes += NanopbMessageType_size;
        CircularBufferHeader header = ReadHeader(file_offset);
        if (read_index_bytes >
                kHeaderSize + (header.buffer_length_in_messages - 1) *
                                  NanopbMessageType_size ||
            read_index_bytes > kHeaderSize + header.count_messages_written *
                                                 NanopbMessageType_size) {
            read_index_bytes = kHeaderSize;
        }
        SetReadIndex(file_offset, read_index_bytes);

        return nanopb_message;
    }

    static CircularBufferHeader ReadHeader(uint16_t file_offset) {
        uint16_t buffer_length_in_messages =
            GetBufferLengthInMessages(file_offset);
        uint16_t count_messages_written = GetCountMessagesWritten(file_offset);
        uint16_t write_index = GetWriteIndex(file_offset);
        uint16_t read_index = GetReadIndex(file_offset);
        CircularBufferHeader header = {buffer_length_in_messages,
                                       count_messages_written, write_index,
                                       read_index};
        return header;
    }

    static NanopbMessageType Search(uint16_t file_offset,
                                    uint64_t timestamp_ms) {
        uint16_t saved_read_index = GetReadIndex(file_offset);
        uint16_t messages_written = GetCountMessagesWritten(file_offset);
        if (messages_written == 0) {
            throw MspException(
                "No messages in the circular buffer being searched",
                kCircularBufferEmptySearchFail, __FILE__, __LINE__);
        }

        SetReadIndex(file_offset, kHeaderSize);
        NanopbMessageType reading = ReadMessage(file_offset);

        while (GetReadIndex(file_offset) < messages_written &&
               reading.timestamp_ms < timestamp_ms) {
            reading = ReadMessage(file_offset);
        }
        // Will return the first reading after passing the requested time
        // or the last reading, if none exists
        SetReadIndex(file_offset, saved_read_index);
        return reading;
    }

   private:
    static uint16_t GetBufferLengthInMessages(uint16_t file_offset) {
        // buffer_length_in_messages is the first encoded uint16_t in the file,
        // starting byte 0
        return ReadUInt16_t(file_offset + 0);
    }

    static void SetBufferLengthInMessages(uint16_t file_offset,
                                          uint16_t buffer_length_in_messages) {
        // buffer_length_in_messages is the first encoded uint16_t in the
        // file, starting byte 0
        WriteUInt16_t(file_offset + 0, buffer_length_in_messages);
    }

    static uint16_t GetCountMessagesWritten(uint16_t file_offset) {
        // count_messages_written is the second encoded uint16_t in the
        // file, starting byte 8
        return ReadUInt16_t(file_offset + 1 * sizeof(uint16_t));
    }

    static void SetCountMessagesWritten(uint16_t file_offset,
                                        uint16_t count_messages_written) {
        // count_messages_written is the second encoded uint16_t in
        // the file, starting byte 8
        WriteUInt16_t(file_offset + 1 * sizeof(uint16_t),
                      count_messages_written);
    }

    static uint16_t GetWriteIndex(uint16_t file_offset) {
        // write_index is the third encoded uint16_t in the
        // file, starting byte 16
        return ReadUInt16_t(file_offset + 2 * sizeof(uint16_t));
    }

    static void SetWriteIndex(uint16_t file_offset,
                              uint16_t write_index_bytes) {
        // write_index is the third encoded uint16_t in the
        // file, starting byte 16
        WriteUInt16_t(file_offset + 2 * sizeof(uint16_t), write_index_bytes);
    }

    static uint16_t GetReadIndex(uint16_t file_offset) {
        // read_index is the fourth encoded uint16_t in
        // the file, starting byte 24
        return ReadUInt16_t(file_offset + 3 * sizeof(uint16_t));
    }

    static void SetReadIndex(uint16_t file_offset, uint16_t read_index_bytes) {
        // read_index is the fourth encoded uint16_t
        // in the file, starting byte 24
        WriteUInt16_t(file_offset + 3 * sizeof(uint16_t), read_index_bytes);
    }

    static uint16_t ReadUInt16_t(uint16_t read_index) {
        byte read_buffer[sizeof(uint16_t)];
        if (!Eeprom::Read(read_index, read_buffer, sizeof(uint16_t))) {
            throw MspException("Failed to read from EEPROM or decode reading",
                               kEepromReadFailure, __FILE__, __LINE__);
        }
        return read_buffer[0] << 8 + read_buffer[1];
    }

    static void WriteUInt16_t(uint16_t write_index, uint16_t value_to_write) {
        byte write_buffer[sizeof(uint16_t)];
        write_buffer[0] = (byte)(value_to_write);
        write_buffer[1] = (byte)(value_to_write >> 8);
        if (!Eeprom::Write(write_index, write_buffer, sizeof(uint16_t))) {
            throw MspException("Failed to write to EEPROM", kEepromWriteFailure,
                               __FILE__, __LINE__);
        }
    }

    static bool ValidHeader(uint16_t file_offset,
                            CircularBufferHeader &header) {
        return header.write_index >= file_offset &&
               header.write_index < file_offset + kHeaderSize +
                                        NanopbMessageType_size *
                                            header.buffer_length_in_messages &&
               header.read_index >= file_offset &&
               header.read_index < file_offset + kHeaderSize +
                                       NanopbMessageType_size *
                                           header.buffer_length_in_messages &&
               header.count_messages_written < header.buffer_length_in_messages;
    }

    static constexpr uint8_t kHeaderSize = 4 * sizeof(uint16_t);
};

#endif  // SRC_DATABASE_CIRCULAR_BUFFER_NANOPB_H_
