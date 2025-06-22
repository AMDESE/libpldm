#include <libpldm/pldm_types.h>
#include <libpldm/rde.h>

#include <array>
#include <cstring>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

// NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)

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

    EXPECT_EQ(encode_negotiate_redfish_parameters_req(
                  FIXED_INSTANCE_ID, mcConcurrencySupport, &mcFeatureSupport,
                  request),
              PLDM_SUCCESS);

    checkHeader(request, PLDM_NEGOTIATE_REDFISH_PARAMETERS, PLDM_REQUEST);

    uint8_t decodedMcConcurrencySupport;
    bitfield16_t decodedMcFeatureSupport;

    EXPECT_EQ(
        decode_negotiate_redfish_parameters_req(
            request, &decodedMcConcurrencySupport, &decodedMcFeatureSupport),
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

    EXPECT_EQ(encode_negotiate_redfish_parameters_resp(
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

    EXPECT_EQ(decode_negotiate_redfish_parameters_resp(
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

TEST(NegotiateMediumParametersTest, EncodeDecodeRequestSuccess)
{
    uint32_t mcMaximumTransferSize = 0xDEADBEEF;

    std::array<uint8_t, sizeof(struct pldm_msg_hdr) +
                            PLDM_RDE_NEGOTIATE_MEDIUM_PARAMETERS_REQ_BYTES>
        requestMsg{};
    pldm_msg* request = (pldm_msg*)requestMsg.data();

    EXPECT_EQ(encode_negotiate_medium_parameters_req(
                  FIXED_INSTANCE_ID, mcMaximumTransferSize, request),
              PLDM_SUCCESS);

    checkHeader(request, PLDM_NEGOTIATE_MEDIUM_PARAMETERS, PLDM_REQUEST);

    uint32_t decodedMcMaximumTransferSize;

    EXPECT_EQ(decode_negotiate_medium_parameters_req(
                  request, &decodedMcMaximumTransferSize),
              PLDM_SUCCESS);

    EXPECT_EQ(decodedMcMaximumTransferSize, mcMaximumTransferSize);
}

TEST(NegotiateMediumParametersTest, EncodeDecodeResponseSuccess)
{
    uint8_t completionCode = PLDM_SUCCESS;
    uint32_t deviceMaximumTransferSize = 0xDEADBEEF;

    std::array<uint8_t, sizeof(struct pldm_msg_hdr) +
                            PLDM_RDE_NEGOTIATE_MEDIUM_PARAMETERS_RESP_BYTES>
        responseMsg{};
    pldm_msg* response = (pldm_msg*)responseMsg.data();

    EXPECT_EQ(encode_negotiate_medium_parameters_resp(
                  FIXED_INSTANCE_ID, completionCode, deviceMaximumTransferSize,
                  PLDM_RDE_NEGOTIATE_MEDIUM_PARAMETERS_RESP_BYTES, response),
              PLDM_SUCCESS);

    checkHeader(response, PLDM_NEGOTIATE_MEDIUM_PARAMETERS, PLDM_RESPONSE);

    uint8_t decodedCompletionCode;
    uint32_t decodedDeviceMaximumTransferSize;

    EXPECT_EQ(decode_negotiate_medium_parameters_resp(
                  response, PLDM_RDE_NEGOTIATE_MEDIUM_PARAMETERS_RESP_BYTES,
                  &decodedCompletionCode, &decodedDeviceMaximumTransferSize),
              PLDM_SUCCESS);

    EXPECT_EQ(decodedCompletionCode, completionCode);
    EXPECT_EQ(decodedDeviceMaximumTransferSize, deviceMaximumTransferSize);
}

TEST(GetSchemaDictionaryTest, EncodeDecodeRequestSuccess)
{
    uint32_t resorceID = 0xDEADBEEF;
    uint8_t requestedSchemaClass = PLDM_RDE_SCHEMA_MAJOR;

    std::array<uint8_t, sizeof(struct pldm_msg_hdr) +
                            PLDM_RDE_SCHEMA_DICTIONARY_REQ_BYTES>
        requestMsg{};
    pldm_msg* request = (pldm_msg*)requestMsg.data();

    EXPECT_EQ(encode_get_schema_dictionary_req(
                  FIXED_INSTANCE_ID, resorceID, requestedSchemaClass,
                  PLDM_RDE_SCHEMA_DICTIONARY_REQ_BYTES, request),
              PLDM_SUCCESS);

    checkHeader(request, PLDM_GET_SCHEMA_DICTIONARY, PLDM_REQUEST);

    uint32_t decodeResorceID;
    uint8_t decodeRequestedSchemaClass;

    EXPECT_EQ(decode_get_schema_dictionary_req(
                  request, PLDM_RDE_SCHEMA_DICTIONARY_REQ_BYTES,
                  &decodeResorceID, &decodeRequestedSchemaClass),
              PLDM_SUCCESS);

    EXPECT_EQ(decodeResorceID, resorceID);
    EXPECT_EQ(requestedSchemaClass, requestedSchemaClass);
}

TEST(GetSchemaDictionaryTest, EncodeDecodeResponseSuccess)
{
    uint8_t completionCode = PLDM_SUCCESS;
    uint8_t dictionaryFormat = PLDM_RDE_SCHEMA_MAJOR;
    uint32_t transferHandle = 0xDEADBEEF;

    std::array<uint8_t, sizeof(struct pldm_msg_hdr) +
                            PLDM_RDE_SCHEMA_DICTIONARY_RESP_BYTES>
        responseMsg{};
    pldm_msg* response = (pldm_msg*)responseMsg.data();

    EXPECT_EQ(encode_get_schema_dictionary_resp(
                  FIXED_INSTANCE_ID, completionCode, dictionaryFormat,
                  transferHandle, PLDM_RDE_SCHEMA_DICTIONARY_RESP_BYTES,
                  response),
              PLDM_SUCCESS);

    checkHeader(response, PLDM_GET_SCHEMA_DICTIONARY, PLDM_RESPONSE);

    uint8_t decodeCompletionCode;
    uint8_t decodeDictionaryFormat;
    uint32_t decodeTransferHandle;

    EXPECT_EQ(decode_get_schema_dictionary_resp(
                  response, PLDM_RDE_SCHEMA_DICTIONARY_RESP_BYTES,
                  &decodeCompletionCode, &decodeDictionaryFormat,
                  &decodeTransferHandle),
              PLDM_SUCCESS);

    EXPECT_EQ(decodeCompletionCode, completionCode);
    EXPECT_EQ(decodeDictionaryFormat, dictionaryFormat);
    EXPECT_EQ(decodeTransferHandle, transferHandle);
}

TEST(GetSchemaURITest, EncodeDecodeRequestSuccess)
{
    uint32_t resourceID = 0xDEADBEEF;
    uint8_t requestedSchemaClass = PLDM_RDE_SCHEMA_MAJOR;

    std::array<uint8_t,
               sizeof(struct pldm_msg_hdr) + PLDM_RDE_SCHEMA_URI_REQ_BYTES>
        requestMsg{};
    pldm_msg* request = (pldm_msg*)requestMsg.data();

    EXPECT_EQ(encode_get_schema_uri_req(FIXED_INSTANCE_ID, resourceID,
                                        requestedSchemaClass, 0, request),
              PLDM_SUCCESS);

    uint32_t decodeResourceID;
    uint8_t decodeRequestedSchemaClass;
    uint8_t decodeOemExtensionNumber;

    EXPECT_EQ(decode_get_schema_uri_req(request, &decodeResourceID,
                                        &decodeRequestedSchemaClass,
                                        &decodeOemExtensionNumber),
              PLDM_SUCCESS);

    checkHeader(request, PLDM_GET_SCHEMA_URI, PLDM_REQUEST);

    EXPECT_EQ(decodeResourceID, resourceID);
    EXPECT_EQ(decodeRequestedSchemaClass, requestedSchemaClass);
    EXPECT_EQ(decodeOemExtensionNumber, 0);
}

TEST(GetSchemaURITest, EncodeDecodeResponseSuccess)
{
    uint8_t completionCode = PLDM_SUCCESS;
    constexpr uint8_t stringFragmentCount = 3;

    std::array<std::string, stringFragmentCount> schemaURI = {
        "fragment1", "fragment2", "fragment3"};

    std::vector<pldm_rde_varstring> varstrings(stringFragmentCount);
    size_t payloadLength = PLDM_RDE_SCHEMA_URI_RESP_FIXED_BYTES;

    for (int i = 0; i < stringFragmentCount; ++i)
    {
        varstrings[i].string_format = 0x00; // Example format
        varstrings[i].string_length_bytes =
            schemaURI[i].size() + 1; // Include null terminator
        varstrings[i].string_data = const_cast<char*>(schemaURI[i].c_str());
        payloadLength +=
            PLDM_RDE_VARSTRING_HEADER_SIZE + varstrings[i].string_length_bytes;
    }

    std::vector<uint8_t> response(sizeof(struct pldm_msg_hdr) + payloadLength);
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
    pldm_msg* responsePtr = reinterpret_cast<pldm_msg*>(&response[0]);

    EXPECT_EQ(encode_get_schema_uri_resp(FIXED_INSTANCE_ID, completionCode,
                                         stringFragmentCount, varstrings.data(),
                                         responsePtr),
              PLDM_SUCCESS);
    checkHeader(responsePtr, PLDM_GET_SCHEMA_URI, PLDM_RESPONSE);

    uint8_t decodeCompletionCode;
    uint8_t decodeStringFragmentCount;
    size_t actual_uri_len = 0;
    uint8_t buffer[sizeof(struct pldm_rde_varstring) +
                   PLDM_RDE_SCHEMA_URI_RESP_MAX_VAR_BYTES];
    pldm_rde_varstring* decodeSchemaURI = (struct pldm_rde_varstring*)buffer;

    EXPECT_EQ(decode_get_schema_uri_resp(
                  responsePtr, &decodeCompletionCode,
                  &decodeStringFragmentCount, decodeSchemaURI,
                  PLDM_RDE_SCHEMA_URI_RESP_MAX_VAR_BYTES, &actual_uri_len),
              PLDM_SUCCESS);

    checkHeader(responsePtr, PLDM_GET_SCHEMA_URI, PLDM_RESPONSE);
    EXPECT_EQ(decodeCompletionCode, completionCode);
    EXPECT_EQ(decodeStringFragmentCount, stringFragmentCount);

    for (uint8_t i = 0; i < stringFragmentCount; ++i)
    {
        EXPECT_EQ(decodeSchemaURI[i].string_data, schemaURI[i]);
    }
}

TEST(GetResourceEtagTest, EncodeDecodeRequestSuccess)
{
    uint32_t resorceID = 0xDEADBEEF;

    std::array<uint8_t, sizeof(struct pldm_msg_hdr) +
                            PLDM_RDE_GET_RESOURCE_ETAG_REQ_BYTES>
        requestMsg{};
    pldm_msg* request = (pldm_msg*)requestMsg.data();

    EXPECT_EQ(
        encode_get_resource_etag_req(FIXED_INSTANCE_ID, resorceID, request),
        PLDM_SUCCESS);

    checkHeader(request, PLDM_GET_RESOURCE_ETAG, PLDM_REQUEST);

    uint32_t decodeResorceID;

    EXPECT_EQ(decode_get_resource_etag_req(request, &decodeResorceID),
              PLDM_SUCCESS);

    EXPECT_EQ(decodeResorceID, resorceID);
}

TEST(GetResourceEtagTest, EncodeDecodeResponseSuccess)
{
    uint8_t completionCode = 0;
    constexpr const char* etag_string_data = "TestData";

    constexpr size_t payloadLength =
        PLDM_RDE_GET_RESOURCE_ETAG_RESP_FIXED_BYTES + 8;

    // Already has the space for the null character in sizeof(struct
    // pldm_rde_negotiate_redfish_parameters_resp).
    std::array<uint8_t, sizeof(struct pldm_msg_hdr) + payloadLength>
        responseMsg{};
    pldm_msg* response = (pldm_msg*)responseMsg.data();

    EXPECT_EQ(encode_get_resource_etag_resp(FIXED_INSTANCE_ID, completionCode,
                                            etag_string_data, response),
              PLDM_SUCCESS);

    checkHeader(response, PLDM_GET_RESOURCE_ETAG, PLDM_RESPONSE);

    // verify payload.
    uint8_t decodedCompletionCode;
    struct pldm_rde_varstring decodedEtag;

    EXPECT_EQ(decode_get_resource_etag_resp(response, payloadLength,
                                            &decodedCompletionCode,
                                            &decodedEtag),
              PLDM_SUCCESS);

    EXPECT_EQ(decodedCompletionCode, completionCode);
    EXPECT_EQ(decodedEtag.string_length_bytes, strlen(etag_string_data) + 1);
    EXPECT_EQ(strncmp(etag_string_data, decodedEtag.string_data,
                      strlen(etag_string_data)),
              0);
}

TEST(RDEMultipartSendTest, EncodeDecodeRequestSuccess)
{
    uint32_t dataTransferHandle = 1;
    uint16_t operationId = 1;
    uint8_t transferFlag = PLDM_RDE_MIDDLE;
    uint32_t nextDataTransferHandle = 1;
    const uint32_t dataLengthBytes = 8;
    uint32_t dataIntegrityChecksum = 0x10;
    const uint32_t payloadLen =
        PLDM_RDE_MULTIPART_SEND_REQ_FIXED_BYTES + dataLengthBytes;
    std::array<uint8_t, dataLengthBytes> data = {0x01, 0x02, 0x03, 0x04,
                                                 0x05, 0x06, 0x07, 0x08};

    std::array<uint8_t, sizeof(struct pldm_msg_hdr) + payloadLen> requestMsg{};
    pldm_msg* request = (pldm_msg*)requestMsg.data();

    EXPECT_EQ(encode_rde_multipart_send_req(
                  FIXED_INSTANCE_ID, dataTransferHandle, operationId,
                  transferFlag, nextDataTransferHandle, dataLengthBytes,
                  data.data(), dataIntegrityChecksum, request),
              PLDM_SUCCESS);

    checkHeader(request, PLDM_RDE_MULTIPART_SEND, PLDM_REQUEST);

    uint32_t decodeDataTransferHandle;
    uint16_t decodeOperationId;
    uint8_t decodeTransferFlag;
    uint32_t decodeNextDataTransferHandle;
    uint32_t decodeDataLengthBytes;
    uint32_t decodeDataIntegrityChecksum = 0;

    std::array<uint8_t, dataLengthBytes> decodeData = {0};

    EXPECT_EQ(decode_rde_multipart_send_req(
                  request, payloadLen, &decodeDataTransferHandle,
                  &decodeOperationId, &decodeTransferFlag,
                  &decodeNextDataTransferHandle, &decodeDataLengthBytes,
                  decodeData.data(), &decodeDataIntegrityChecksum),
              PLDM_SUCCESS);

    EXPECT_EQ(decodeDataTransferHandle, dataTransferHandle);
    EXPECT_EQ(decodeOperationId, operationId);
    EXPECT_EQ(decodeTransferFlag, transferFlag);
    EXPECT_EQ(decodeNextDataTransferHandle, nextDataTransferHandle);
    EXPECT_EQ(decodeDataLengthBytes, dataLengthBytes);
    EXPECT_EQ(decodeData, data);
}

TEST(RDEMultipartSendTest, EncodeDecodeRequestWithChecksumSuccess)
{
    uint32_t dataTransferHandle = 1;
    rde_op_id operationId = 1;
    uint8_t transferFlag = PLDM_RDE_END;
    uint32_t nextDataTransferHandle = 1;
    constexpr uint32_t dataOnlyLength = 8;
    std::array<uint8_t, dataOnlyLength> data = {0x01, 0x02, 0x03, 0x04,
                                                0x05, 0x06, 0x07, 0x08};
    uint32_t dataIntegrityChecksum = 0x10;
    const uint32_t dataLengthBytes =
        dataOnlyLength + sizeof(dataIntegrityChecksum);
    const uint32_t payloadLen =
        PLDM_RDE_MULTIPART_SEND_REQ_FIXED_BYTES + dataLengthBytes;

    std::array<uint8_t, sizeof(pldm_msg_hdr) + payloadLen> requestMsg{};
    pldm_msg* request = (pldm_msg*)requestMsg.data();

    EXPECT_EQ(encode_rde_multipart_send_req(
                  FIXED_INSTANCE_ID, dataTransferHandle, operationId,
                  transferFlag, nextDataTransferHandle, dataLengthBytes,
                  data.data(), dataIntegrityChecksum, request),
              PLDM_SUCCESS);

    checkHeader(request, PLDM_RDE_MULTIPART_SEND, PLDM_REQUEST);

    uint32_t decodeDataTransferHandle;
    rde_op_id decodeOperationId;
    uint8_t decodeTransferFlag;
    uint32_t decodeNextDataTransferHandle;
    uint32_t decodeDataLengthBytes;
    uint32_t decodeDataIntegrityChecksum = 0;
    std::array<uint8_t, dataOnlyLength> decodeData = {0};

    EXPECT_EQ(decode_rde_multipart_send_req(
                  request, payloadLen, &decodeDataTransferHandle,
                  &decodeOperationId, &decodeTransferFlag,
                  &decodeNextDataTransferHandle, &decodeDataLengthBytes,
                  decodeData.data(), &decodeDataIntegrityChecksum),
              PLDM_SUCCESS);

    // Validate decoded values
    EXPECT_EQ(decodeDataTransferHandle, dataTransferHandle);
    EXPECT_EQ(decodeOperationId, operationId);
    EXPECT_EQ(decodeTransferFlag, transferFlag);
    EXPECT_EQ(decodeNextDataTransferHandle, nextDataTransferHandle);
    EXPECT_EQ(decodeDataLengthBytes, dataLengthBytes);
    EXPECT_EQ(decodeData, data);
    EXPECT_EQ(decodeDataIntegrityChecksum, dataIntegrityChecksum);
}

TEST(RDEMultipartSendTest, EncodeDecodeResponseSuccess)
{
    uint8_t completionCode = 0;
    uint8_t transferOperation = 1;

    constexpr size_t payloadLength = PLDM_RDE_MULTIPART_SEND_RESP_BYTES;

    std::array<uint8_t, sizeof(struct pldm_msg_hdr) + payloadLength>
        responseMsg{};
    pldm_msg* response = (pldm_msg*)responseMsg.data();

    EXPECT_EQ(encode_rde_multipart_send_resp(FIXED_INSTANCE_ID, completionCode,
                                             transferOperation, payloadLength,
                                             response),
              PLDM_SUCCESS);

    checkHeader(response, PLDM_RDE_MULTIPART_SEND, PLDM_RESPONSE);

    // verify payload.
    uint8_t decodedCompletionCode;
    uint8_t decodedtransferOperation;

    EXPECT_EQ(decode_rde_multipart_send_resp(response, payloadLength,
                                             &decodedCompletionCode,
                                             &decodedtransferOperation),
              PLDM_SUCCESS);

    EXPECT_EQ(decodedCompletionCode, completionCode);
    EXPECT_EQ(decodedtransferOperation, transferOperation);
}

TEST(RDEMultipartReceiveTest, EncodeDecodeRequestSuccess)
{
    uint32_t dataTransferHandle = 1;
    rde_op_id operationID = 1;
    uint8_t transferOperation = 1;

    constexpr size_t payloadLength = PLDM_RDE_MULTIPART_RECEIVE_REQ_BYTES;

    std::array<uint8_t, sizeof(struct pldm_msg_hdr) + payloadLength>
        responseMsg{};
    pldm_msg* response = (pldm_msg*)responseMsg.data();

    EXPECT_EQ(encode_rde_multipart_receive_req(FIXED_INSTANCE_ID,
                                               dataTransferHandle, operationID,
                                               transferOperation, response),
              PLDM_SUCCESS);

    checkHeader(response, PLDM_RDE_MULTIPART_RECEIVE, PLDM_REQUEST);

    // verify payload.
    uint32_t decodeDataTransferHandle = 1;
    rde_op_id decodeOperationID = 1;
    uint8_t decodeTransferOperation = 1;

    EXPECT_EQ(decode_rde_multipart_receive_req(
                  response, payloadLength, &decodeDataTransferHandle,
                  &decodeOperationID, &decodeTransferOperation),
              PLDM_SUCCESS);

    EXPECT_EQ(decodeDataTransferHandle, dataTransferHandle);
    EXPECT_EQ(decodeOperationID, operationID);
    EXPECT_EQ(decodeTransferOperation, transferOperation);
}

TEST(RDEMultipartReceiveTest, EncodeDecodeResoponseSuccess)
{
    uint8_t completionCode = PLDM_SUCCESS;
    uint8_t transferFlag = PLDM_RDE_MIDDLE;
    uint32_t nextDataTransferHandle = 1;
    const uint32_t dataLengthBytes = 8;
    uint32_t dataIntegrityChecksum = 0x10;
    const uint32_t payloadLen =
        PLDM_RDE_MULTIPART_RECEIVE_RESP_FIXED_BYTES + dataLengthBytes;
    std::array<uint8_t, dataLengthBytes> data = {0x01, 0x02, 0x03, 0x04,
                                                 0x05, 0x06, 0x07, 0x08};

    std::array<uint8_t, sizeof(struct pldm_msg_hdr) + payloadLen> requestMsg{};
    pldm_msg* request = (pldm_msg*)requestMsg.data();

    EXPECT_EQ(encode_rde_multipart_receive_resp(
                  FIXED_INSTANCE_ID, completionCode, transferFlag,
                  nextDataTransferHandle, dataLengthBytes, data.data(),
                  dataIntegrityChecksum, request),
              PLDM_SUCCESS);

    checkHeader(request, PLDM_RDE_MULTIPART_RECEIVE, PLDM_RESPONSE);

    uint8_t decodeCompletionCode;
    uint8_t decodeTransferFlag;
    uint32_t decodeNextDataTransferHandle;
    uint32_t decodeDataLengthBytes;
    uint32_t decodeDataIntegrityChecksum = 0;

    std::array<uint8_t, dataLengthBytes> decodeData = {0};

    EXPECT_EQ(decode_rde_multipart_receive_resp(
                  request, payloadLen, &decodeCompletionCode,
                  &decodeTransferFlag, &decodeNextDataTransferHandle,
                  &decodeDataLengthBytes, decodeData.data(),
                  &decodeDataIntegrityChecksum),
              PLDM_SUCCESS);

    EXPECT_EQ(decodeCompletionCode, completionCode);
    EXPECT_EQ(decodeTransferFlag, transferFlag);
    EXPECT_EQ(decodeNextDataTransferHandle, nextDataTransferHandle);
    EXPECT_EQ(decodeDataLengthBytes, dataLengthBytes);
    EXPECT_EQ(decodeData, data);
}

TEST(RDEMultipartReceiveTest, EncodeDecodeResoponseWithChecksumSuccess)
{
    uint8_t completionCode = PLDM_SUCCESS;
    uint8_t transferFlag = PLDM_RDE_END;
    uint32_t nextDataTransferHandle = 1;
    const uint32_t dataLengthBytes = 8;
    uint32_t dataIntegrityChecksum = 0x10;
    const uint32_t payloadLen = PLDM_RDE_MULTIPART_RECEIVE_RESP_FIXED_BYTES +
                                dataLengthBytes + sizeof(dataIntegrityChecksum);
    std::array<uint8_t, dataLengthBytes> data = {0x01, 0x02, 0x03, 0x04,
                                                 0x05, 0x06, 0x07, 0x08};

    std::array<uint8_t, sizeof(struct pldm_msg_hdr) + payloadLen> requestMsg{};
    pldm_msg* request = (pldm_msg*)requestMsg.data();
    uint32_t dataBytes = dataLengthBytes + sizeof(dataIntegrityChecksum);

    EXPECT_EQ(encode_rde_multipart_receive_resp(
                  FIXED_INSTANCE_ID, completionCode, transferFlag,
                  nextDataTransferHandle, dataBytes, data.data(),
                  dataIntegrityChecksum, request),
              PLDM_SUCCESS);

    checkHeader(request, PLDM_RDE_MULTIPART_RECEIVE, PLDM_RESPONSE);

    uint8_t decodeCompletionCode;
    uint8_t decodeTransferFlag;
    uint32_t decodeNextDataTransferHandle;
    uint32_t decodeDataBytes;
    uint32_t decodeDataIntegrityChecksum = 0;

    std::array<uint8_t, dataLengthBytes> decodeData = {0};

    EXPECT_EQ(decode_rde_multipart_receive_resp(
                  request, payloadLen, &decodeCompletionCode,
                  &decodeTransferFlag, &decodeNextDataTransferHandle,
                  &decodeDataBytes, decodeData.data(),
                  &decodeDataIntegrityChecksum),
              PLDM_SUCCESS);

    EXPECT_EQ(decodeCompletionCode, completionCode);
    EXPECT_EQ(decodeTransferFlag, transferFlag);
    EXPECT_EQ(decodeNextDataTransferHandle, nextDataTransferHandle);
    EXPECT_EQ(decodeDataBytes, dataBytes);
    EXPECT_EQ(decodeData, data);
    EXPECT_EQ(decodeDataIntegrityChecksum, dataIntegrityChecksum);
}

TEST(RDEOperationCompleteTest, EncodeDecodeRequestSuccess)
{
    uint32_t resourceId = 0x12345678;
    rde_op_id operationId = 0x42;
    const uint32_t payloadLen = PLDM_RDE_OPERATION_COMPLETE_REQ_BYTES;
    std::array<uint8_t, sizeof(pldm_msg_hdr) + payloadLen> requestMsg{};
    pldm_msg* request = (pldm_msg*)requestMsg.data();

    EXPECT_EQ(encode_rde_operation_complete_req(FIXED_INSTANCE_ID, resourceId,
                                                operationId, request),
              PLDM_SUCCESS);

    checkHeader(request, PLDM_RDE_OPERATION_COMPLETE, PLDM_REQUEST);

    // Decode
    uint32_t decodedResourceId = 0;
    rde_op_id decodedOperationId = 0;
    EXPECT_EQ(decode_rde_operation_complete_req(
                  request, payloadLen, &decodedResourceId, &decodedOperationId),
              PLDM_SUCCESS);
    // Validate decoded values
    EXPECT_EQ(decodedResourceId, resourceId);
    EXPECT_EQ(decodedOperationId, operationId);
}

TEST(RDEOperationCompleteTest, EncodeDecodeResoponseSuccess)
{
    uint8_t completionCode = PLDM_SUCCESS;

    std::array<uint8_t, sizeof(struct pldm_msg_hdr) + sizeof(completionCode)>
        requestMsg{};
    pldm_msg* response = (pldm_msg*)requestMsg.data();

    EXPECT_EQ(encode_rde_operation_complete_resp(FIXED_INSTANCE_ID,
                                                 completionCode, response),
              PLDM_SUCCESS);

    checkHeader(response, PLDM_RDE_OPERATION_COMPLETE, PLDM_RESPONSE);

    uint8_t decodedCompletionCode;

    EXPECT_EQ(decode_rde_operation_complete_resp(
                  response, sizeof(completionCode), &decodedCompletionCode),
              PLDM_SUCCESS);

    EXPECT_EQ(decodedCompletionCode, completionCode);
}

TEST(RDEOperationStatusTest, EncodeDecodeRequestSuccess)
{
    uint32_t resourceId = 0x1234;
    rde_op_id operationId = 0x42;
    const uint32_t payloadLen = PLDM_RDE_OPERATION_STATUS_REQ_BYTES;
    std::array<uint8_t, sizeof(pldm_msg_hdr) + payloadLen> requestMsg{};
    pldm_msg* request = (pldm_msg*)requestMsg.data();

    EXPECT_EQ(encode_rde_operation_status_req(FIXED_INSTANCE_ID, resourceId,
                                              operationId, request),
              PLDM_SUCCESS);

    checkHeader(request, PLDM_RDE_OPERATION_STATUS, PLDM_REQUEST);

    // Decode
    uint32_t decodedResourceId = 0;
    rde_op_id decodedOperationId = 0;
    EXPECT_EQ(decode_rde_operation_status_req(
                  request, payloadLen, &decodedResourceId, &decodedOperationId),
              PLDM_SUCCESS);
    // Validate decoded values
    EXPECT_EQ(decodedResourceId, resourceId);
    EXPECT_EQ(decodedOperationId, operationId);
}

TEST(RDEOperationStatusTest, EncodeDecodeResoponseSuccess)
{
    uint8_t completionCode = PLDM_SUCCESS;
    uint8_t operationStatus = PLDM_RDE_OPERATION_HAVE_RESULTS;
    uint8_t completionPercentage = 50;
    uint32_t completionTimeSeconds = 0x7F;
    bitfield8_t operationExecutionFlags = {.byte = 0x07};
    uint32_t resultTransferHandle = 1;
    bitfield8_t permissionFlags = {.byte = 0x03};
    const uint32_t responsePayloadLength = 8;
    const char* etag = "SampleData";
    std::array<uint8_t, responsePayloadLength> responsePayload = {1, 2, 3, 4,
                                                                  5, 6, 7, 8};
    // size of etag("SampleData") is 10 + 1 for null terminator
    const uint32_t payloadLen = PLDM_RDE_OPERATION_STATUS_RESP_FIXED_BYTES +
                                PLDM_RDE_VARSTRING_HEADER_SIZE + 11 +
                                responsePayloadLength;

    std::array<uint8_t, sizeof(struct pldm_msg_hdr) + payloadLen> requestMsg{};
    pldm_msg* response = (pldm_msg*)requestMsg.data();

    EXPECT_EQ(encode_rde_operation_status_resp(
                  FIXED_INSTANCE_ID, completionCode, operationStatus,
                  completionPercentage, completionTimeSeconds,
                  &operationExecutionFlags, resultTransferHandle,
                  &permissionFlags, responsePayloadLength, etag,
                  responsePayload.data(), response),
              PLDM_SUCCESS);

    checkHeader(response, PLDM_RDE_OPERATION_STATUS, PLDM_RESPONSE);

    uint8_t decodedCompletionCode;
    uint8_t decodedOperationStatus;
    uint8_t decodedCompletionPercentage;
    uint32_t decodedCompletionTimeSeconds;
    bitfield8_t decodedOperationExecutionFlags;
    uint32_t decodedResultTransferHandle;
    bitfield8_t decodedPermissionFlags;
    uint32_t decodedResponsePayloadLength;
    struct pldm_rde_varstring decodedEtag;
    std::array<uint8_t, responsePayloadLength> decodedResponsePayload = {0};

    EXPECT_EQ(decode_rde_operation_status_resp(
                  response, payloadLen, &decodedCompletionCode,
                  &decodedOperationStatus, &decodedCompletionPercentage,
                  &decodedCompletionTimeSeconds,
                  &decodedOperationExecutionFlags, &decodedResultTransferHandle,
                  &decodedPermissionFlags, &decodedResponsePayloadLength,
                  &decodedEtag, decodedResponsePayload.data()),
              PLDM_SUCCESS);

    EXPECT_EQ(decodedCompletionCode, completionCode);
    EXPECT_EQ(decodedOperationStatus, operationStatus);
    EXPECT_EQ(decodedCompletionPercentage, completionPercentage);
    EXPECT_EQ(decodedCompletionTimeSeconds, completionTimeSeconds);
    EXPECT_EQ(decodedOperationExecutionFlags.byte,
              operationExecutionFlags.byte);
    EXPECT_EQ(decodedResultTransferHandle, resultTransferHandle);
    EXPECT_EQ(decodedPermissionFlags.byte, permissionFlags.byte);
    EXPECT_EQ(decodedResponsePayloadLength, responsePayloadLength);
    EXPECT_EQ(decodedEtag.string_length_bytes, strlen(etag) + 1);
    EXPECT_EQ(memcmp(etag, decodedEtag.string_data, strlen(etag)), 0);
    EXPECT_EQ(decodedResponsePayload, responsePayload);
}
