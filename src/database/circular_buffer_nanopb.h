#ifndef SRC_DATABASE_CIRCULAR_BUFFER_NANOPB_H_
#define SRC_DATABASE_CIRCULAR_BUFFER_NANOPB_H_

#include <external/etl/exception.h>
#include <external/nanopb/pb_decode.h>
#include <external/nanopb/pb_encode.h>
#include <src/database/HammingCoder.h>
#include <src/database/sd_card.h>
#include <src/messages/pb.h>
#include <src/util/data_types.h>

<<<<<<< HEAD
template <typename NanopbMessageType, uint16_t nanopb_message_size,
          const pb_field_t* nanopb_message_fields>
class CircularBufferNanopb {
   public:
    CircularBufferNanopb(uint32_t buffer_length_in_messages,
                         const char* file_name)
        : buffer_length_in_messages(buffer_length_in_messages),
          file_name(file_name),
          write_index_in_messages(0),
          read_index_in_messages(0) {
        // Open the file twice, once for writing and once for reading.
        // This gives a 'writing head' and a 'reading head' like a tape player.
        // Create the file if it doesn't exist.
        file_handle_writing = SdCard::FileOpen(
            file_name, SdCard::kFileOpenAlwaysMode | SdCard::kFileWriteMode);
        // File may already have data in it. Check the size and set the indices.
        uint32_t file_size_in_bytes = file_handle_writing.fsize;
        SdCard::FileSeek(file_handle_writing, file_size_in_bytes);
        uint32_t file_size_in_messages =
            file_size_in_bytes / nanopb_message_size;
        write_index_in_messages = file_size_in_messages;
        if (write_index_in_messages > buffer_length_in_messages) {
            write_index_in_messages = 0;
            SdCard::FileSeek(file_handle_writing, 0);
        }
=======
template <typename T, uint16_t nanopb_message_size,
          const pb_field_t* nanopb_message_fields>
class CircularBufferNanopb {
   public:
    CircularBufferNanopb(uint32_t buffer_length_in_messages, const char* file_name)
        : buffer_length_in_messages(buffer_length_in_messages),
          file_name(file_name),
          write_index_in_messages(0),
          read_index_in_messages(0),
          count_messages_written(0) {
        // Open the file twice, once for writing and once for reading.
        // This gives a 'writing head' and a 'reading head' like a tape player.
        file_handle_writing =
          SdCard::FileOpen(file_name, SdCard::kFileOpenAlwaysMode | SdCard::kFileWriteMode);
        // File may already have data in it. Check the size and set the indices.
        uint32_t file_size_in_bytes = file_handle_writing.fsize;
        SdCard::FileSeek(file_handle_writing, file_size_in_bytes);
        uint32_t file_size_in_messages = file_size_in_bytes / nanopb_message_size;
        write_index_in_messages = file_size_in_messages;
        if (write_index_in_messages > buffer_length_in_messages) {
          write_index_in_messages = 0;
          SdCard::FileSeek(file_handle_writing, 0);
        }
        count_messages_written = file_size_in_messages;
>>>>>>> Detumbling branch to get detumbling working until it works
        file_handle_reading =
            SdCard::FileOpen(file_name, SdCard::kFileReadMode);
    };

    ~CircularBufferNanopb() {
        SdCard::FileClose(file_handle_writing);
        SdCard::FileClose(file_handle_reading);
        SdCard::FileDelete(file_name);
    };

<<<<<<< HEAD
    void WriteMessage(NanopbMessageType message) {
=======
    void WriteMessage(T message) {
>>>>>>> Detumbling branch to get detumbling working until it works
        // Serialise
        byte buffer[nanopb_message_size];
        bool status;
        pb_ostream_t stream = pb_ostream_from_buffer(buffer, sizeof(buffer));
        status = pb_encode(&stream, nanopb_message_fields, &message);
        if (!status) {
            etl::exception e("pb_encode failed", __FILE__, __LINE__);
            throw e;
        }

        // Hamming encode bytes and write to file
        HammingEncodedByte hamming_encoded_byte;
<<<<<<< HEAD
        for (uint16_t i = 0; i < nanopb_message_size; i++) {
=======
        for (uint8_t i = 0; i < nanopb_message_size; i++) {
>>>>>>> Detumbling branch to get detumbling working until it works
            hamming_encoded_byte = HammingCoder::Encode(buffer[i]);
            SdCard::FileWrite(file_handle_writing,
                              hamming_encoded_byte.codewords, 2);
        }
        SdCard::FileFlush(file_handle_writing);

        // Increment write index and wrap
        write_index_in_messages += 1;
<<<<<<< HEAD
        if (write_index_in_messages >= buffer_length_in_messages) {
=======
        if (write_index_in_messages > buffer_length_in_messages) {
>>>>>>> Detumbling branch to get detumbling working until it works
            write_index_in_messages = 0;
            SdCard::FileSeek(file_handle_writing, 0);
        }
    }

<<<<<<< HEAD
    NanopbMessageType ReadMessage() {
        // Check there is data to read
        uint32_t file_size_in_messages =
            file_handle_reading.fsize / nanopb_message_size;
        if (read_index_in_messages >= file_size_in_messages) {
=======
    T ReadMessage() {
        // Check there is data to read
        if (read_index_in_messages >= count_messages_written) {
>>>>>>> Detumbling branch to get detumbling working until it works
            etl::exception e("Tried to read nonexistent data from file",
                             __FILE__, __LINE__);
            throw e;
        }

        // Read from file
        byte hamming_encoded_buffer[2 * nanopb_message_size];
        SdCard::FileRead(file_handle_reading, hamming_encoded_buffer,
                         2 * nanopb_message_size);

        // Increment read index and wrap
        read_index_in_messages += 1;
        if (read_index_in_messages > buffer_length_in_messages) {
            read_index_in_messages = 0;
            SdCard::FileSeek(file_handle_reading, 0);
        }

        // Hamming decode
        byte hamming_decoded_buffer[nanopb_message_size];
        for (uint8_t i = 0; i < nanopb_message_size; i++) {
            HammingEncodedByte hamming_encoded_byte;
            HammingDecodedByte hamming_decoded_byte;
            hamming_encoded_byte.codewords[0] = hamming_encoded_buffer[i * 2];
            hamming_encoded_byte.codewords[1] =
                hamming_encoded_buffer[i * 2 + 1];
            hamming_decoded_byte = HammingCoder::Decode(hamming_encoded_byte);
            if (hamming_decoded_byte.valid == false) {
                etl::exception e("Failed to decode hamming encoded message",
                                 __FILE__, __LINE__);
                throw e;
            }
            hamming_decoded_buffer[i] = hamming_decoded_byte.data;
        }

        // Deserialise
        pb_istream_t stream =
            pb_istream_from_buffer(hamming_decoded_buffer, nanopb_message_size);
<<<<<<< HEAD
        NanopbMessageType message_struct;
=======
        T message_struct;
>>>>>>> Detumbling branch to get detumbling working until it works
        bool status =
            pb_decode(&stream, nanopb_message_fields, &message_struct);
        if (!status) {
            etl::exception e("pb_decode failed", __FILE__, __LINE__);
            throw e;
        }

        return message_struct;
    }

   private:
    uint32_t buffer_length_in_messages;
    const char* file_name;
    uint32_t write_index_in_messages;
    uint32_t read_index_in_messages;
<<<<<<< HEAD
=======
    uint32_t count_messages_written;
>>>>>>> Detumbling branch to get detumbling working until it works
    File file_handle_writing;
    File file_handle_reading;
};

#endif  // SRC_DATABASE_CIRCULAR_BUFFER_NANOPB_H_
