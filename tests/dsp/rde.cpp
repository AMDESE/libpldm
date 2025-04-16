#include <libpldm/pldm_types.h>
#include <libpldm/rde.h>

#include <array>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

static const uint8_t FIXED_INSTANCE_ID = 15;

/* data is a pointer to pldm message response header */

static void checkHeader(const pldm_msg* msg, uint8_t command,
                        MessageType request)
{
    if (msg == nullptr)
    {
        std::cout << "Message is NULL\n";
        return;
    }

    EXPECT_EQ(msg->hdr.request, request);
    EXPECT_EQ(msg->hdr.type, PLDM_RDE);
    EXPECT_EQ(msg->hdr.command, command);
    EXPECT_EQ(msg->hdr.instance_id, FIXED_INSTANCE_ID);
}

TEST(NegotiateRedfishParametersTest, EncodeDecodeRequestSuccess)
{
    uint8_t mcConcurrencySupport = 13;
    bitfield16_t mcFeatureSupport = {.value = 0x7389};

    std::array<uint8_t, sizeof(struct pldm_msg_hdr) +
                            PLDM_RDE_NEGOTIATE_REDFISH_PARAMETERS_REQ_BYTES>
        requestMsg{};
    pldm_msg* request = (pldm_msg*)requestMsg.data();

    EXPECT_EQ(encode_rde_negotiate_redfish_parameters_req(
                  FIXED_INSTANCE_ID, mcConcurrencySupport, &mcFeatureSupport,
                  PLDM_RDE_NEGOTIATE_REDFISH_PARAMETERS_REQ_BYTES, request),
              PLDM_SUCCESS);

    checkHeader(request, PLDM_NEGOTIATE_REDFISH_PARAMETERS, PLDM_REQUEST);

    uint8_t decodedMcConcurrencySupport;
    bitfield16_t decodedMcFeatureSupport;

    EXPECT_EQ(decode_rde_negotiate_redfish_parameters_req(
                  request, PLDM_RDE_NEGOTIATE_REDFISH_PARAMETERS_REQ_BYTES,
                  &decodedMcConcurrencySupport, &decodedMcFeatureSupport),
              PLDM_SUCCESS);

    EXPECT_EQ(decodedMcConcurrencySupport, mcConcurrencySupport);
    EXPECT_EQ(decodedMcFeatureSupport.value, mcFeatureSupport.value);
}

TEST(NegotiateRedfishParametersTest, EncodeDecodeResponseSuccess)
{
    uint8_t completionCode = 0;

    uint8_t deviceConcurrencySupport = 1;
    bitfield8_t deviceCapabilitiesFlags = {.byte = 0x3F};
    bitfield16_t deviceFeatureSupport = {.value = 0x7389};
    uint32_t deviceConfigurationSignature = 0xDEADBEEF;
    constexpr const char* device = "Test Device";

    constexpr size_t payloadLength =
        PLDM_RDE_NEGOTIATE_REDFISH_PARAMETERS_RESP_MIN_SIZE + 11;

    // Already has the space for the null character in sizeof(struct
    // pldm_rde_negotiate_redfish_parameters_resp).
    std::array<uint8_t, sizeof(struct pldm_msg_hdr) + payloadLength>
        responseMsg{};
    pldm_msg* response = (pldm_msg*)responseMsg.data();

    EXPECT_EQ(encode_rde_negotiate_redfish_parameters_resp(
                  FIXED_INSTANCE_ID, completionCode, deviceConcurrencySupport,
                  &deviceCapabilitiesFlags, &deviceFeatureSupport,
                  deviceConfigurationSignature, device,
                  PLDM_RDE_VARSTRING_ASCII, payloadLength, response),
              PLDM_SUCCESS);

    checkHeader(response, PLDM_NEGOTIATE_REDFISH_PARAMETERS, PLDM_RESPONSE);

    // verify payload.
    uint8_t decodedCompletionCode;
    uint8_t decodedDeviceConcurrencySupport;
    bitfield8_t decodedDeviceCapabilitiesFlags;
    bitfield16_t decodedDeviceFeatureSupport;
    uint32_t decodedDeviceConfigurationSignature;
    struct pldm_rde_varstring decodedProviderName;

    EXPECT_EQ(decode_rde_negotiate_redfish_parameters_resp(
                  response, payloadLength, &decodedCompletionCode,
                  &decodedDeviceConcurrencySupport,
                  &decodedDeviceCapabilitiesFlags, &decodedDeviceFeatureSupport,
                  &decodedDeviceConfigurationSignature, &decodedProviderName),
              PLDM_SUCCESS);

    EXPECT_EQ(decodedCompletionCode, completionCode);
    EXPECT_EQ(decodedDeviceConcurrencySupport, deviceConcurrencySupport);
    EXPECT_EQ(decodedDeviceCapabilitiesFlags.byte,
              deviceCapabilitiesFlags.byte);
    EXPECT_EQ(decodedDeviceFeatureSupport.value, deviceFeatureSupport.value);
    EXPECT_EQ(decodedDeviceConfigurationSignature,
              deviceConfigurationSignature);
    EXPECT_EQ(decodedProviderName.string_format, PLDM_RDE_VARSTRING_ASCII);
    EXPECT_EQ(decodedProviderName.string_length_bytes, strlen(device) + 1);
    EXPECT_EQ(strncmp(device, decodedProviderName.string_data, strlen(device)),
              0);
}
