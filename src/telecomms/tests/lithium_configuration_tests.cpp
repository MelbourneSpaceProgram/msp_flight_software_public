#include <src/config/unit_tests.h>
#include <src/telecomms/lithium_configuration.h>
#include <src/telecomms/lithium_utils.h>
#include <test_runners/unity.h>

void TestLithiumConfigurationSerialise() {
    LithiumConfiguration config;
    SerialisedMessage serialised_config = config.Serialise();
    TEST_ASSERT_EQUAL_UINT16(serialised_config.GetSize(),
                             config.GetSerialisedSize());
    // TODO(dingbenjamin): Anything else worth testing that isn't covered by
    // deserialise?
}

void TestLithiumConfigurationDeserialise() {
    LithiumConfiguration config;
    config.SetFunctionConfig(0b1111111111111111);
    config.SetFunctionConfig2(0b1111111111110000);
    config.SetInterfaceBaudRate(LithiumConfiguration::kBaud38400);
    config.SetRxFreq(1000);
    config.SetTxFreq(200000);
    config.SetRxRfBaudRate(LithiumConfiguration::kRfBaud19200);
    config.SetTxRfBaudRate(LithiumConfiguration::kRfBaud9600);
    config.SetTxModulation(LithiumConfiguration::kAfsk);
    config.SetRxModulation(LithiumConfiguration::kBpsk);
    config.SetTxPostamble(0x01);
    config.SetTxPreamble(0x01);
    config.SetTxPowerAmpLevel(190);
    SerialisedMessage serialised_config = config.Serialise();
    byte *config_bytes = serialised_config.GetBuffer();

    LithiumConfiguration deserialised_config(config_bytes);
    TEST_ASSERT_EQUAL_UINT16(0b1111111111111111,
                             deserialised_config.GetFunctionConfig());
    TEST_ASSERT_EQUAL_UINT16(0b1111111111110000,
                             deserialised_config.GetFunctionConfig2());
    TEST_ASSERT_EQUAL_UINT8(LithiumConfiguration::kBaud38400,
                            deserialised_config.GetInterfaceBaudRate());
    TEST_ASSERT_EQUAL_UINT32(1000, deserialised_config.GetRxFreq());
    TEST_ASSERT_EQUAL_UINT32(200000, deserialised_config.GetTxFreq());
    TEST_ASSERT_EQUAL_UINT8(LithiumConfiguration::kRfBaud19200,
                            deserialised_config.GetRxRfBaudRate());
    TEST_ASSERT_EQUAL_UINT8(LithiumConfiguration::kRfBaud9600,
                            deserialised_config.GetTxRfBaudRate());
    TEST_ASSERT_EQUAL_UINT8(LithiumConfiguration::kAfsk,
                            deserialised_config.GetTxModulation());
    TEST_ASSERT_EQUAL_UINT8(LithiumConfiguration::kBpsk,
                            deserialised_config.GetRxModulation());
    TEST_ASSERT_EQUAL_UINT8(0x01, deserialised_config.GetTxPostamble());
    TEST_ASSERT_EQUAL_UINT8(0x01, deserialised_config.GetTxPreamble());
    TEST_ASSERT_EQUAL_UINT8(190, deserialised_config.GetTxPowerAmpLevel());
}
