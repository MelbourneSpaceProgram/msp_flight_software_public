#include <src/config/unit_tests.h>
#include <src/telecomms/lithium_configuration.h>
#include <src/telecomms/lithium_function_config.h>
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

void TestFunctionConfigDeserialise() {
    LithiumConfiguration config;
    LithiumFunctionConfig *function_config = config.GetFunctionConfig();
    TEST_ASSERT_EQUAL_UINT16(LithiumFunctionConfig::kExternalEventOffLogicLow,
                             function_config->GetExternalEventFunction());
    TEST_ASSERT_EQUAL_UINT16(LithiumFunctionConfig::kPin13OffLogicLow,
                             function_config->GetConfig2Function());
    TEST_ASSERT_EQUAL_UINT16(LithiumFunctionConfig::kPin14OffLogicLow,
                             function_config->GetConfig1Function());
    TEST_ASSERT_EQUAL_UINT16(LithiumFunctionConfig::kTelemetryPacketLoggingRate1Hz,
                             function_config->GetTelemetryLoggingRate());
    TEST_ASSERT(!function_config->IsRxCrcFunctionEnable());
    TEST_ASSERT(!function_config->IsTxCrcFunctionEnable());
    TEST_ASSERT(!function_config->IsOaFunctionEnable());
    TEST_ASSERT(!function_config->IsOaFunctionEnable());
    TEST_ASSERT(!function_config->IsCodeUploadFunctionEnable());
    TEST_ASSERT(!function_config->IsRadioResetFunctionEnable());
    TEST_ASSERT(!function_config->IsFactorySettingsFlag());
    TEST_ASSERT(!function_config->IsAutoFrequencyControlEnable());

    config.GetFunctionConfig()->SetExternalEventFunction(
        LithiumFunctionConfig::k2500MsToggle);
    config.GetFunctionConfig()->SetConfig2Function(LithiumFunctionConfig::kTxRxSwitch);
    config.GetFunctionConfig()->SetConfig1Function(
        LithiumFunctionConfig::kDioOaPatternA);
    config.GetFunctionConfig()->SetTelemetryLoggingRate(
        LithiumFunctionConfig::kTelemetryPacketLoggingRate4Hz);
    config.GetFunctionConfig()->SetRxCrcFunctionEnable(true);
    config.GetFunctionConfig()->SetTxCrcFunctionEnable(true);
    config.GetFunctionConfig()->SetOaFunctionEnable(true);
    config.GetFunctionConfig()->SetCodeUploadFunctionEnable(true);
    config.GetFunctionConfig()->SetRadioResetFunctionEnable(true);
    config.GetFunctionConfig()->SetAutoFrequencyControlEnable(true);

    SerialisedMessage serialised_config = config.Serialise();
    byte *config_bytes = serialised_config.GetBuffer();
    LithiumConfiguration deserialised_config(config_bytes);

    LithiumFunctionConfig *deserialised_function_config =
        deserialised_config.GetFunctionConfig();
    TEST_ASSERT_EQUAL_UINT16(
        LithiumFunctionConfig::k2500MsToggle,
        deserialised_function_config->GetExternalEventFunction());
    TEST_ASSERT_EQUAL_UINT16(
        LithiumFunctionConfig::kTxRxSwitch,
        deserialised_function_config->GetConfig2Function());
    TEST_ASSERT_EQUAL_UINT16(
        LithiumFunctionConfig::kDioOaPatternA,
        deserialised_function_config->GetConfig1Function());
    TEST_ASSERT_EQUAL_UINT16(
        LithiumFunctionConfig::kTelemetryPacketLoggingRate4Hz,
        deserialised_function_config->GetTelemetryLoggingRate());
    TEST_ASSERT(deserialised_function_config->IsRxCrcFunctionEnable());
    TEST_ASSERT(deserialised_function_config->IsTxCrcFunctionEnable());
    TEST_ASSERT(deserialised_function_config->IsOaFunctionEnable());
    TEST_ASSERT(deserialised_function_config->IsOaFunctionEnable());
    TEST_ASSERT(deserialised_function_config->IsCodeUploadFunctionEnable());
    TEST_ASSERT(deserialised_function_config->IsRadioResetFunctionEnable());
    TEST_ASSERT(!deserialised_function_config->IsFactorySettingsFlag());
    TEST_ASSERT(deserialised_function_config->IsAutoFrequencyControlEnable());
}
