#include <CppUTest/TestHarness.h>
#include <src/messages/ClearSectionsUplinkPayload.pb.h>
#include <src/messages/ExecuteSectionsUplinkPayload.pb.h>
#include <src/messages/QuerySectionsUplinkPayload.pb.h>
#include <src/payload_processor/payload_processor.h>
#include <src/payload_processor/payload_section_manager.h>
#include <src/payload_processor/runnable_payload_processor.h>
#include <src/payload_processor/tests/mock_uplink_builder.h>
#include <src/payload_processor/uplinks/test_uplink.h>

void CopySectionFromPayload(uint16_t from_index, uint16_t to_index,
                            byte* section, byte* payload) {
    for (uint16_t i = from_index; i <= to_index; i++) {
        section[i - from_index] = payload[i];
    }
}

bool TestTwoSections(uint8_t start_index_a, uint8_t end_index_a,
                     bool expected_a, uint8_t start_index_b,
                     uint8_t end_index_b, bool expected_b) {
    byte test_uplink_payload[Lithium::kMaxReceivedUplinkSize] = {0};
    MockUplinkBuilder test_uplink_builder(test_uplink_payload,
                                          Lithium::kMaxReceivedUplinkSize);

    byte section_payload_1[Lithium::kMaxReceivedUplinkSize] = {0};
    MockUplinkBuilder section_builder_1(section_payload_1,
                                        Lithium::kMaxReceivedUplinkSize);

    byte section_payload_2[Lithium::kMaxReceivedUplinkSize] = {0};
    MockUplinkBuilder section_builder_2(section_payload_2,
                                        Lithium::kMaxReceivedUplinkSize);

    byte execute_section_payload[Lithium::kMaxReceivedUplinkSize] = {0};
    MockUplinkBuilder execute_section_builder(execute_section_payload,
                                              Lithium::kMaxReceivedUplinkSize);

    ExecuteSectionsUplinkPayload execute_sections;
    execute_sections.post_buffer = true;

    PayloadProcessor payload_processor;

    test_uplink_builder.AddUplinkCode(kTestUplink)
        .AddData<byte>(TestUplink::kTestValue1)
        .AddData<byte>(TestUplink::kTestValue2)
        .Build();

    section_builder_1.AddUplinkCode(kSectionUplink)
        .AddUplinkSection(&test_uplink_builder, start_index_a, end_index_a);
    section_builder_2.AddUplinkCode(kSectionUplink)
        .AddUplinkSection(&test_uplink_builder, start_index_b, end_index_b);
    execute_section_builder.AddUplinkCode(kExecuteSectionsUplink)
        .AddNanopbMacro(ExecuteSectionsUplinkPayload)(execute_sections);

    CHECK_EQUAL(expected_a, payload_processor.ParseAndExecuteUplinks(
                                section_builder_1.Build()));
    CHECK_EQUAL(expected_b, payload_processor.ParseAndExecuteUplinks(
                                section_builder_2.Build()));
    return payload_processor.ParseAndExecuteUplinks(
        execute_section_builder.Build());
}

TEST_GROUP(PayloadSectionManager){};

TEST(PayloadSectionManager, TestAddSectionToBuffer) {
    PayloadSectionManager manager;
    uint16_t command_code = 1;

    uint16_t payload_size = 150;
    byte payload[payload_size];

    // Fill payload with data
    for (uint8_t i = 0; i < payload_size; i++) {
        payload[i] = i;
    }

    // Test case where whole section needs to be written to buffer
    uint16_t section_1_start_index = 30;
    uint16_t section_1_end_index = 90;
    uint16_t section_1_size = section_1_end_index - section_1_start_index + 1;
    byte section_1[section_1_size];
    CopySectionFromPayload(section_1_start_index, section_1_end_index,
                           section_1, payload);

    // Test case where case where there is some overlap at the end
    uint16_t section_2_start_index = 20;
    uint16_t section_2_end_index = 50;
    uint16_t section_2_size = section_2_end_index - section_2_start_index + 1;
    byte section_2[section_2_size];
    CopySectionFromPayload(section_2_start_index, section_2_end_index,
                           section_2, payload);

    // Test case where case where there is some overlap at the start
    uint16_t section_3_start_index = 50;
    uint16_t section_3_end_index = 130;
    uint16_t section_3_size = section_3_end_index - section_3_start_index + 1;
    byte section_3[section_3_size];
    CopySectionFromPayload(section_3_start_index, section_3_end_index,
                           section_3, payload);

    // Test case where case where there is some overlap at the start and end
    uint16_t section_4_start_index = 0;
    uint16_t section_4_end_index = 5;
    uint16_t section_4_size = section_4_end_index - section_4_start_index + 1;
    byte section_4[section_4_size];
    CopySectionFromPayload(section_4_start_index, section_4_end_index,
                           section_4, payload);

    uint16_t section_5_start_index = 0;
    uint16_t section_5_end_index = 25;
    uint16_t section_5_size = section_5_end_index - section_5_start_index + 1;
    byte section_5[section_5_size];
    CopySectionFromPayload(section_5_start_index, section_5_end_index,
                           section_5, payload);

    // Test case where case where the buffer should not be written to
    uint16_t section_6_start_index = 50;
    uint16_t section_6_end_index = 60;
    uint16_t section_6_size = section_6_end_index - section_6_start_index + 1;
    byte section_6[section_6_size];
    CopySectionFromPayload(section_6_start_index, section_6_end_index,
                           section_6, payload);

    // Fill the buffer
    uint16_t section_7_start_index = 129;
    uint16_t section_7_end_index = payload_size - 1;
    uint16_t section_7_size = section_7_end_index - section_7_start_index + 1;
    byte section_7[section_7_size];
    CopySectionFromPayload(section_7_start_index, section_7_end_index,
                           section_7, payload);

    CHECK(manager.AddSectionToBuffer(command_code, section_1_start_index,
                                     section_1_end_index, section_1));
    CHECK(manager.AddSectionToBuffer(command_code, section_2_start_index,
                                     section_2_end_index, section_2));
    CHECK(manager.AddSectionToBuffer(command_code, section_3_start_index,
                                     section_3_end_index, section_3));
    CHECK(manager.AddSectionToBuffer(command_code, section_4_start_index,
                                     section_4_end_index, section_4));
    CHECK(manager.AddSectionToBuffer(command_code, section_5_start_index,
                                     section_5_end_index, section_5));
    CHECK(manager.AddSectionToBuffer(command_code, section_6_start_index,
                                     section_6_end_index, section_6));
    CHECK(manager.AddSectionToBuffer(command_code, section_7_start_index,
                                     section_7_end_index, section_7));

    byte* section_manager_buffer = manager.GetBuffer();

    // Check that section buffer and payload are the same
    for (uint8_t i = 0; i < payload_size; i++) {
        CHECK_EQUAL(payload[i], section_manager_buffer[i]);
    }
}

// Perfect upload
TEST(PayloadSectionManager, TestSectionUplinkPerfect) {
    CHECK(TestTwoSections(0, 2, true, 3, 5, true));
}
// Upload the same section twice
TEST(PayloadSectionManager, TestSectionUplinkSameFail) {
    CHECK_FALSE(TestTwoSections(0, 2, true, 0, 2, true));
}

// Upload overlapping sections
TEST(PayloadSectionManager, TestSectionUplinkOverlap) {
    CHECK(TestTwoSections(0, 4, true, 1, 5, true));
}

// Upload two sections with a gap in the middle
TEST(PayloadSectionManager, TestSectionUplinkGapFail) {
    CHECK_FALSE(TestTwoSections(0, 2, true, 4, 5, true));
}

// Upload invalid section
TEST(PayloadSectionManager, TestSectionInvalidFail) {
    CHECK_FALSE(TestTwoSections(0, 2, true, 4, 3, false));
}

// Full upload in one go
TEST(PayloadSectionManager, TestSectionUplinkFull) {
    CHECK(TestTwoSections(0, 5, true, 0, 0, true));
}

TEST(PayloadSectionManager, TestSectionQueryClearExecute) {
    byte test_uplink_payload[Lithium::kMaxReceivedUplinkSize] = {0};
    MockUplinkBuilder test_uplink_builder(test_uplink_payload,
                                          Lithium::kMaxReceivedUplinkSize);

    byte payload_success[Lithium::kMaxReceivedUplinkSize] = {0};
    MockUplinkBuilder builder_success(payload_success,
                                      Lithium::kMaxReceivedUplinkSize);

    byte payload_fail[Lithium::kMaxReceivedUplinkSize] = {0};
    MockUplinkBuilder builder_fail(payload_fail,
                                   Lithium::kMaxReceivedUplinkSize);

    ExecuteSectionsUplinkPayload execute_sections;
    execute_sections.post_buffer = true;

    ClearSectionsUplinkPayload clear_sections;
    clear_sections.clear_buffer = true;

    QuerySectionsUplinkPayload query_sections;
    query_sections.queried = true;

    test_uplink_builder.AddUplinkCode(kTestUplink)
        .AddData<byte>(TestUplink::kTestValue1)
        .AddData<byte>(TestUplink::kTestValue2)
        .Build();

    PayloadProcessor payload_processor;

    builder_success.AddUplinkCode(kSectionUplink)
        .AddUplinkSection(&test_uplink_builder, 0, 5)
        .AddUplinkCode(kQuerySectionsUplink)
        .AddNanopbMacro(QuerySectionsUplinkPayload)(query_sections);

    CHECK(payload_processor.ParseAndExecuteUplinks(builder_success.Build()));

    builder_fail.AddUplinkCode(kClearSectionsUplink)
        .AddNanopbMacro(ClearSectionsUplinkPayload)(clear_sections)
        .AddUplinkCode(kExecuteSectionsUplink)
        .AddNanopbMacro(ExecuteSectionsUplinkPayload)(execute_sections);

    CHECK_FALSE(payload_processor.ParseAndExecuteUplinks(builder_fail.Build()));
}
