#ifndef SRC_DATABASE_CIRCULAR_BUFFER_NANOPB_H_
#define SRC_DATABASE_CIRCULAR_BUFFER_NANOPB_H_

#include <external/etl/exception.h>
#include <external/nanopb/pb_decode.h>
#include <external/nanopb/pb_encode.h>
#include <src/database/hamming_coder.h>
#include <src/database/sd_card.h>
#include <src/messages/pb.h>
#include <src/util/data_types.h>

/*
  Circular Buffer abstraction backed by sd card files.

  Each circular buffer is made to store one kind of nanopb message,
  taking care of the serialisation and deserialisation via nanopb and
  Hamming encoding/decoding for single bit-flip redundancy.

  A circular buffer is just a file beginning with:

  - uint32_t buffer_length_in_messages
  - uint32_t count_messages_written
  - uint32_t write_index_bytes
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

template <typename NanopbMessageType, uint16_t NanopbMessageType_size,
          const pb_field_t *NanopbMessageType_fields>
class CircularBufferNanopb {
   public:
    static void Create(const char *file_name,
                       uint32_t buffer_length_in_messages) {
        File file_handle = SdCard::FileOpen(
            file_name, SdCard::kFileOpenAlwaysMode | SdCard::kFileWriteMode |
                           SdCard::kFileReadMode);
        uint32_t file_size_in_bytes = file_handle.fsize;
        // If file is empty, initialise metadata
        // TODO (rskew) check that metadata is valid
        if (file_size_in_bytes < kEncodedHeaderSize) {
            SetBufferLengthInMessages(file_handle, buffer_length_in_messages);
            SetCountMessagesWritten(file_handle, 0);
            SetWriteIndex(file_handle, kEncodedHeaderSize);
            SetReadIndex(file_handle, kEncodedHeaderSize);
        }

        // If the file already exists, do nothing

        SdCard::FileClose(file_handle);
    }

    static void WriteMessage(const char *file_name,
                             const NanopbMessageType message) {
        File file_handle = SdCard::FileOpen(
            file_name, SdCard::kFileOpenExistingMode | SdCard::kFileWriteMode |
                           SdCard::kFileReadMode);

        // Serialise
        byte buffer[NanopbMessageType_size];
        bool status;
        pb_ostream_t stream = pb_ostream_from_buffer(buffer, sizeof(buffer));
        status = pb_encode(&stream, NanopbMessageType_fields, &message);
        if (!status) {
            etl::exception e("pb_encode failed", __FILE__, __LINE__);
            throw e;
        }

        // Write to file
        uint32_t write_index_bytes = GetWriteIndex(file_handle);
        SdCard::FileSeek(file_handle, write_index_bytes);
        for (uint16_t i = 0; i < NanopbMessageType_size; i++) {
            WriteByte(file_handle, buffer[i]);
        }
        uint32_t count_messages_written = GetCountMessagesWritten(file_handle);
        // Check for overflow
        if (count_messages_written + 1 != 0) {
            SetCountMessagesWritten(file_handle, count_messages_written + 1);
        }

        // Increment write index and wrap
        write_index_bytes += 2 * NanopbMessageType_size;
        uint32_t buffer_length_in_messages =
            GetBufferLengthInMessages(file_handle);
        if (write_index_bytes >
            2 * (buffer_length_in_messages - 1) * NanopbMessageType_size +
                kEncodedHeaderSize) {
            write_index_bytes = kEncodedHeaderSize;
        }
        SetWriteIndex(file_handle, write_index_bytes);

        SdCard::FileClose(file_handle);
    }

    static NanopbMessageType ReadMessage(const char *file_name) {
        File file_handle = SdCard::FileOpen(
            file_name, SdCard::kFileOpenExistingMode | SdCard::kFileWriteMode |
                           SdCard::kFileReadMode);
        // TODO (rskew) check for successful file open

        // Read from file
        uint32_t read_index_bytes = GetReadIndex(file_handle);
        SdCard::FileSeek(file_handle, read_index_bytes);
        byte hamming_encoded_buffer[2 * NanopbMessageType_size];
        SdCard::FileRead(file_handle, hamming_encoded_buffer,
                         2 * NanopbMessageType_size);

        // Increment write index and wrap
        read_index_bytes += 2 * NanopbMessageType_size;
        uint32_t buffer_length_in_messages =
            GetBufferLengthInMessages(file_handle);
        uint32_t count_messages_written = GetCountMessagesWritten(file_handle);
        if (read_index_bytes >
                2 * (buffer_length_in_messages - 1) * NanopbMessageType_size +
                    kEncodedHeaderSize ||
            read_index_bytes >
                2 * count_messages_written * NanopbMessageType_size +
                    kEncodedHeaderSize) {
            read_index_bytes = kEncodedHeaderSize;
        }
        SetReadIndex(file_handle, read_index_bytes);

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
        pb_istream_t stream = pb_istream_from_buffer(hamming_decoded_buffer,
                                                     NanopbMessageType_size);
        NanopbMessageType message_struct;
        bool status =
            pb_decode(&stream, NanopbMessageType_fields, &message_struct);
        if (!status) {
            etl::exception e("pb_decode failed", __FILE__, __LINE__);
            throw e;
        }

        SdCard::FileClose(file_handle);
        return message_struct;
    }

    // TODO (rskew) return header struct
    static void ReadHeader(const char *file_name) {
        File file_handle = SdCard::FileOpen(
            file_name, SdCard::kFileOpenExistingMode | SdCard::kFileWriteMode |
                           SdCard::kFileReadMode);
        uint32_t buffer_length_in_messages =
            GetBufferLengthInMessages(file_handle);
        uint32_t count_messages_written = GetCountMessagesWritten(file_handle);
        uint32_t write_index = GetWriteIndex(file_handle);
        uint32_t read_index = GetReadIndex(file_handle);
        SdCard::FileClose(file_handle);
    }

   private:
    static void WriteByte(File &file_handle, byte unencoded_byte) {
        HammingEncodedByte hamming_encoded_byte;
        hamming_encoded_byte = HammingCoder::Encode(unencoded_byte);
        SdCard::FileWrite(file_handle, hamming_encoded_byte.codewords, 2);
    }

    static uint32_t GetBufferLengthInMessages(File &file_handle) {
        // buffer_length_in_messages is the first encoded uint32_t in the file,
        // starting byte 0
        return ReadUint32_t(file_handle, 0);
    }

    static void SetBufferLengthInMessages(File &file_handle,
                                          uint32_t buffer_length_in_messages) {
        // buffer_length_in_messages is the first encoded uint32_t in the
        // file, starting byte 0
        WriteUint32_t(file_handle, 0, buffer_length_in_messages);
    }

    static uint32_t GetCountMessagesWritten(File &file_handle) {
        // count_messages_written is the second encoded uint32_t in the
        // file, starting byte 8
        return ReadUint32_t(file_handle, 2 * kUint32_tSize);
    }

    static void SetCountMessagesWritten(File &file_handle,
                                        uint32_t count_messages_written) {
        // count_messages_written is the second encoded uint32_t in
        // the file, starting byte 8
        WriteUint32_t(file_handle, 2 * 1 * kUint32_tSize,
                      count_messages_written);
    }

    static uint32_t GetWriteIndex(File &file_handle) {
        // write_index is the third encoded uint32_t in the
        // file, starting byte 16
        return ReadUint32_t(file_handle, 2 * 2 * kUint32_tSize);
    }

    static void SetWriteIndex(File &file_handle, uint32_t write_index_bytes) {
        // write_index is the third encoded uint32_t in the
        // file, starting byte 16
        WriteUint32_t(file_handle, 2 * 2 * kUint32_tSize, write_index_bytes);
    }

    static uint32_t GetReadIndex(File &file_handle) {
        // read_index is the fourth encoded uint32_t in
        // the file, starting byte 24
        return ReadUint32_t(file_handle, 2 * 3 * kUint32_tSize);
    }

    static void SetReadIndex(File &file_handle, uint32_t read_index_bytes) {
        // read_index is the fourth encoded uint32_t
        // in the file, starting byte 24
        WriteUint32_t(file_handle, 2 * 3 * kUint32_tSize, read_index_bytes);
    }

    static uint32_t ReadUint32_t(File &file_handle, uint32_t index_bytes) {
        byte byte_array[kUint32_tSize];
        bool valid_decodings[kUint32_tSize];
        byte hamming_encoded_byte_array[2 * kUint32_tSize];
        // write_index is the third encoded uint32_t in
        // the file, starting byte 16
        SdCard::FileSeek(file_handle, index_bytes);
        SdCard::FileRead(file_handle, hamming_encoded_byte_array,
                         2 * kUint32_tSize);
        HammingCoder::DecodeByteArray(
            byte_array, kUint32_tSize, valid_decodings,
            hamming_encoded_byte_array, 2 * kUint32_tSize);
        for (uint16_t i = 0; i < kUint32_tSize; i++) {
            if (valid_decodings[i] == false) {
                etl::exception e("Failed to decode hamming encoded message",
                                 __FILE__, __LINE__);
                throw e;
            }
        }
        return ByteArrayToUint32_t(byte_array);
    }

    static void WriteUint32_t(File &file_handle, uint32_t index_bytes,
                              uint32_t value_to_write) {
        byte write_buffer[4];
        Uint32_tToByteArray(value_to_write, write_buffer);
        SdCard::FileSeek(file_handle, index_bytes);
        for (uint8_t i = 0; i < kUint32_tSize; i++) {
            WriteByte(file_handle, write_buffer[i]);
        }
        SdCard::FileFlush(file_handle);
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

    static const uint8_t kEncodedHeaderSize = 2 * 4 * kUint32_tSize;
};

#endif  // SRC_DATABASE_CIRCULAR_BUFFER_NANOPB_H_
