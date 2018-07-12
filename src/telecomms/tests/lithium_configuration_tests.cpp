#include <CppUTest/TestHarness.h>
#include <src/config/unit_tests.h>
#include <src/telecomms/lithium_configuration.h>
#include <src/telecomms/lithium_function_config.h>
#include <src/telecomms/lithium_utils.h>

TEST_GROUP(LithiumConfiguration){};

TEST(LithiumConfiguration, TestLithiumConfigurationSerialise) {
    LithiumConfiguration config;
    SerialisedMessage serialised_config = config.Serialise();
    CHECK_EQUAL(serialised_config.GetSize(), config.GetSerialisedSize());
    // TODO(dingbenjamin): Anything else worth testing that isn't covered by
    // deserialise?
}

TEST(LithiumConfiguration, TestLithiumConfigurationDeserialise) {
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
    CHECK_EQUAL(LithiumConfiguration::kBaud38400,
                deserialised_config.GetInterfaceBaudRate());
    CHECK_EQUAL(1000, deserialised_config.GetRxFreq());
    CHECK_EQUAL(200000, deserialised_config.GetTxFreq());
    CHECK_EQUAL(LithiumConfiguration::kRfBaud19200,
                deserialised_config.GetRxRfBaudRate());
    CHECK_EQUAL(LithiumConfiguration::kRfBaud9600,
                deserialised_config.GetTxRfBaudRate());
    CHECK_EQUAL(LithiumConfiguration::kAfsk,
                deserialised_config.GetTxModulation());
    CHECK_EQUAL(LithiumConfiguration::kBpsk,
                deserialised_config.GetRxModulation());
    CHECK_EQUAL(0x01, deserialised_config.GetTxPostamble());
    CHECK_EQUAL(0x01, deserialised_config.GetTxPreamble());
    CHECK_EQUAL(190, deserialised_config.GetTxPowerAmpLevel());
}

TEST(LithiumConfiguration, TestFunctionConfigDeserialise) {
    LithiumConfiguration config;
    LithiumFunctionConfig *function_config = config.GetFunctionConfig();
    CHECK_EQUAL(LithiumFunctionConfig::kExternalEventOffLogicLow,
                function_config->GetExternalEventFunction());
    CHECK_EQUAL(LithiumFunctionConfig::kPin13OffLogicLow,
                function_config->GetConfig2Function());
    CHECK_EQUAL(LithiumFunctionConfig::kPin14OffLogicLow,
                function_config->GetConfig1Function());
    CHECK_EQUAL(LithiumFunctionConfig::kTelemetryPacketLoggingRate1Hz,
                function_config->GetTelemetryLoggingRate());
    CHECK(!function_config->IsRxCrcFunctionEnable());
    CHECK(!function_config->IsTxCrcFunctionEnable());
    CHECK(!function_config->IsOaFunctionEnable());
    CHECK(!function_config->IsOaFunctionEnable());
    CHECK(!function_config->IsCodeUploadFunctionEnable());
    CHECK(!function_config->IsRadioResetFunctionEnable());
    CHECK(!function_config->IsFactorySettingsFlag());
    CHECK(!function_config->IsAutoFrequencyControlEnable());

    config.GetFunctionConfig()->SetExternalEventFunction(
        LithiumFunctionConfig::k2500MsToggle);
    config.GetFunctionConfig()->SetConfig2Function(
        LithiumFunctionConfig::kTxRxSwitch);
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
    CHECK_EQUAL(LithiumFunctionConfig::k2500MsToggle,
                deserialised_function_config->GetExternalEventFunction());
    CHECK_EQUAL(LithiumFunctionConfig::kTxRxSwitch,
                deserialised_function_config->GetConfig2Function());
    CHECK_EQUAL(LithiumFunctionConfig::kDioOaPatternA,
                deserialised_function_config->GetConfig1Function());
    CHECK_EQUAL(LithiumFunctionConfig::kTelemetryPacketLoggingRate4Hz,
                deserialised_function_config->GetTelemetryLoggingRate());
    CHECK(deserialised_function_config->IsRxCrcFunctionEnable());
    CHECK(deserialised_function_config->IsTxCrcFunctionEnable());
    CHECK(deserialised_function_config->IsOaFunctionEnable());
    CHECK(deserialised_function_config->IsOaFunctionEnable());
    CHECK(deserialised_function_config->IsCodeUploadFunctionEnable());
    CHECK(deserialised_function_config->IsRadioResetFunctionEnable());
    CHECK(!deserialised_function_config->IsFactorySettingsFlag());
    CHECK(deserialised_function_config->IsAutoFrequencyControlEnable());
}
