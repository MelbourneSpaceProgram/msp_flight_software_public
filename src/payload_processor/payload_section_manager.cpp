#include <src/payload_processor/payload_section_manager.h>
#include <src/util/message_codes.h>
#include <string.h>
#include <ti/sysbios/BIOS.h>
#include <xdc/runtime/Log.h>

PayloadSectionManager::PayloadSectionManager()
    : current_command_code(kNoUplink),
      uplink_mailbox_handle(Lithium::GetInstance()->GetUplinkMailbox()) {
    empty_locations.push_back(
        std::make_pair(0, Lithium::kMaxReceivedUplinkSize - 1));
    memset(section_buffer, 0, Lithium::kMaxReceivedUplinkSize);
}

bool PayloadSectionManager::AddSectionToBuffer(uint16_t command_code,
                                               uint16_t start_index,
                                               uint16_t end_index,
                                               byte* section) {
    if (start_index > end_index) {
        Log_error0(
            "Section not added to payload section buffer. Start index is "
            "greater than the end index.");
        return false;
    }

    if (start_index > Lithium::kMaxReceivedUplinkSize ||
        end_index > Lithium::kMaxReceivedUplinkSize) {
        Log_error0(
            "Section not added to payload section buffer. Start index or "
            "end index is out of bounds.");
        return false;
    }

    // Confirm that the manager should accept the received section
    if (!(current_command_code == kNoUplink ||
          command_code == current_command_code)) {
        Log_info0(
            "Section not added to payload section buffer. Current command code "
            "is not kNoCommand or received command code and current command "
            "code do not match.");
        return false;
    }

    // Confirm that the buffer is not full
    if (empty_locations[0].first == Lithium::kMaxReceivedUplinkSize - 1) {
        Log_info0(
            "Section not added to payload section buffer. Section buffer "
            "is full.");
        return false;
    }

    int16_t empty_start_index;
    int16_t empty_end_index;

    // Cycle through the empty locations in the buffer and see if the
    // section received can fill the buffer
    for (etl::vector<std::pair<uint16_t, uint16_t>,
                     Lithium::kMaxReceivedUplinkSize>::iterator it =
             empty_locations.begin();
         it != empty_locations.end(); ++it) {
        empty_start_index = it->first;
        empty_end_index = it->second;

        // Determine if the received section is in the range of the current
        // empty location represented by the pair at the current iterator.
        if (start_index > empty_end_index || end_index < empty_start_index) {
            continue;
        }

        // Start writing from the maximum of start_index and empty_start_index
        int16_t write_start_index =
            (start_index > empty_start_index) ? start_index : empty_start_index;

        // Finish writing to the minimum of end_index and empty_end_index
        int16_t write_end_index =
            (end_index > empty_end_index) ? empty_end_index : end_index;

        CopyToBuffer(write_start_index, write_end_index, section, start_index);

        // Update the empty locations
        if (write_start_index == empty_start_index &&
            write_end_index == empty_end_index) {
            // The empty location represented by the pair at the current
            // iterator has been written to entirely
            it = empty_locations.erase(it);
            if (!empty_locations.empty()) --it;
            continue;
        } else if (write_start_index > empty_start_index &&
                   write_end_index < empty_end_index) {
            // The write was in the middle of the empty location
            it->second = write_start_index - 1;
            empty_locations.insert(
                it, std::make_pair(write_end_index + 1, empty_end_index));
        } else if (write_start_index > empty_start_index) {
            // Only the end of the empty location was written to
            it->second = write_start_index - 1;
        } else if (write_end_index < empty_end_index) {
            // Only the start of the empty location was written to
            it->first = write_end_index + 1;
        } else {
            Log_error0(
                "Payload section manager could not determine empty location "
                "pairs.");
            return false;
        }

        if (it->first > it->second) {
            it = empty_locations.erase(it);
            if (!empty_locations.empty()) --it;
        }
    }
    return true;
}

void PayloadSectionManager::CopyToBuffer(uint16_t from_index, uint16_t to_index,
                                         byte* section,
                                         uint16_t section_offset) {
    for (uint16_t i = from_index; i <= to_index; i++) {
        section_buffer[i] = section[i - section_offset];
    }
}

byte* PayloadSectionManager::GetBuffer() { return section_buffer; }

void PayloadSectionManager::ClearBuffer() {
    empty_locations.clear();
    empty_locations.push_back(
        std::make_pair(0, Lithium::kMaxReceivedUplinkSize - 1));
    memset(section_buffer, 0, Lithium::kMaxReceivedUplinkSize);
    current_command_code = kNoUplink;
}

bool PayloadSectionManager::PostPayloadToUplinkMailbox() {
    return Mailbox_post(uplink_mailbox_handle, section_buffer,
                        BIOS_WAIT_FOREVER);
}

uint16_t PayloadSectionManager::GetCurrentCommandCode() {
    return current_command_code;
}

etl::vector<std::pair<uint16_t, uint16_t>, Lithium::kMaxReceivedUplinkSize>
PayloadSectionManager::GetBufferStatus() {
    return empty_locations;
}
