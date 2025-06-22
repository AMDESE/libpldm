/* SPDX-License-Identifier: Apache-2.0 OR GPL-2.0-or-later */
#ifndef RDE_H
#define RDE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <libpldm/base.h>
#include <libpldm/utils.h>
#include <stddef.h>
#include <stdint.h>

/* Response lengths are inclusive of completion code */
#define PLDM_RDE_NEGOTIATE_REDFISH_PARAMETERS_REQ_BYTES	    3
#define PLDM_RDE_NEGOTIATE_REDFISH_PARAMETERS_RESP_MIN_SIZE 12
#define PLDM_RDE_NEGOTIATE_MEDIUM_PARAMETERS_REQ_BYTES	    4
#define PLDM_RDE_NEGOTIATE_MEDIUM_PARAMETERS_RESP_BYTES	    5
#define PLDM_RDE_MIN_TRANSFER_SIZE_BYTES		    64
#define PLDM_RDE_SCHEMA_DICTIONARY_REQ_BYTES		    5
#define PLDM_RDE_SCHEMA_DICTIONARY_RESP_BYTES		    6
#define PLDM_RDE_SCHEMA_URI_REQ_BYTES			    6
#define PLDM_RDE_SCHEMA_URI_RESP_FIXED_BYTES		    2
#define PLDM_RDE_SCHEMA_URI_RESP_MAX_VAR_BYTES		    500
#define PLDM_RDE_GET_RESOURCE_ETAG_REQ_BYTES		    4
#define PLDM_RDE_GET_RESOURCE_ETAG_RESP_FIXED_BYTES	    4 // Include NULL

#define PLDM_RDE_MULTIPART_SEND_REQ_FIXED_BYTES		    15
#define PLDM_RDE_MULTIPART_SEND_RESP_BYTES		    2
#define PLDM_RDE_MULTIPART_RECEIVE_REQ_BYTES		    7
#define PLDM_RDE_MULTIPART_RECEIVE_RESP_FIXED_BYTES	    10

// Define rdeOpID as a 16-bit unsigned integer
typedef uint16_t rde_op_id;

enum pldm_rde_commands {
	PLDM_NEGOTIATE_REDFISH_PARAMETERS = 0x01,
	PLDM_NEGOTIATE_MEDIUM_PARAMETERS = 0x02,
	PLDM_GET_SCHEMA_DICTIONARY = 0x03,
	PLDM_GET_SCHEMA_URI = 0x04,
	PLDM_GET_RESOURCE_ETAG = 0x05,
	PLDM_RDE_MULTIPART_SEND = 0x30,
	PLDM_RDE_MULTIPART_RECEIVE = 0x31,
};

enum pldm_rde_varstring_format {
	PLDM_RDE_VARSTRING_UNKNOWN = 0,
	PLDM_RDE_VARSTRING_ASCII = 1,
	PLDM_RDE_VARSTRING_UTF_8 = 2,
	PLDM_RDE_VARSTRING_UTF_16 = 3,
	PLDM_RDE_VARSTRING_UTF_16LE = 4,
	PLDM_RDE_VARSTRING_UTF_16BE = 5,
};

enum pldm_rde_transfer_flag {
	PLDM_RDE_START = 0,
	PLDM_RDE_MIDDLE = 1,
	PLDM_RDE_END = 2,
	PLDM_RDE_START_AND_END = 3,
};

enum pldm_rde_transfer_operation {
	PLDM_RDE_XFER_FIRST_PART = 0,
	PLDM_RDE_XFER_NEXT_PART = 1,
	PLDM_RDE_XFER_ABORT = 2,
};

/**
 * @brief MC feature support.
 *
 * The flags can be OR'd together to build the feature support for a MC.
 */
enum pldm_rde_mc_feature {
	PLDM_RDE_MC_FEATURE_HEAD_SUPPORTED = 1 << 0,
	PLDM_RDE_MC_FEATURE_READ_SUPPORTED = 1 << 1,
	PLDM_RDE_MC_FEATURE_CREATE_SUPPORTED = 1 << 2,
	PLDM_RDE_MC_FEATURE_DELETE_SUPPORTED = 1 << 3,
	PLDM_RDE_MC_FEATURE_UPDATE_SUPPORTED = 1 << 4,
	PLDM_RDE_MC_FEATURE_REPLACE_SUPPORTED = 1 << 5,
	PLDM_RDE_MC_FEATURE_ACTION_SUPPORTED = 1 << 6,
	PLDM_RDE_MC_FEATURE_EVENTS_SUPPORTED = 1 << 7,
	PLDM_RDE_MC_FEATURE_BEJ_1_1_SUPPORTED = 1 << 8,
	// Reserved bits [15:9]
};

/**
 * @brief Device feature support.
 *
 * The flags can be OR'd together to build the feature support for an RDE Device.
 */
enum pldm_rde_device_feature_support {
	PLDM_RDE_DEVICE_FEATURE_HEAD_SUPPORTED = 1 << 0,
	PLDM_RDE_DEVICE_FEATURE_READ_SUPPORTED = 1 << 1,
	PLDM_RDE_DEVICE_FEATURE_CREATE_SUPPORTED = 1 << 2,
	PLDM_RDE_DEVICE_FEATURE_DELETE_SUPPORTED = 1 << 3,
	PLDM_RDE_DEVICE_FEATURE_UPDATE_SUPPORTED = 1 << 4,
	PLDM_RDE_DEVICE_FEATURE_REPLACE_SUPPORTED = 1 << 5,
	PLDM_RDE_DEVICE_FEATURE_ACTION_SUPPORTED = 1 << 6,
	PLDM_RDE_DEVICE_FEATURE_EVENTS_SUPPORTED = 1 << 7,
	// Reserved bits [15:8]
};

/**
 * @brief Device capability flags.
 *
 * The flags can be OR'd together to build capabilities of a device.
 */
enum device_capabilities_flags {
	PLDM_RDE_DEVICE_CAP_ATOMIC_RESOURCE_READ = 1 << 0,
	PLDM_RDE_DEVICE_CAP_EXPAND_SUPPORT = 1 << 1,
	PLDM_RDE_DEVICE_CAP_BEJ_1_1_SUPPORT = 1 << 2,
	// Reserved bits [7:3]
};

// Fixed overhead per varstring: format (1 byte) + length (1 byte)
#define PLDM_RDE_VARSTRING_HEADER_SIZE 2

/* @brief varstring PLDM data type */
struct pldm_rde_varstring {
	uint8_t string_format;	     // Format of the string
	uint8_t string_length_bytes; // Length of the string including NULL terminator
	char *string_data; // Pointer to the string data, should be NULL terminated
};

/* @brief schemaClass PLDM data type(enum8) Ref section: 5.3.2 */
enum pldm_rde_schema_type {
	PLDM_RDE_SCHEMA_MAJOR = 0,
	PLDM_RDE_SCHEMA_EVENT = 1,
	PLDM_RDE_SCHEMA_ANNOTATION = 2,
	PLDM_RDE_SCHEMA_COLLECTION_MEMBER_TYPE = 3,
	PLDM_RDE_SCHEMA_ERROR = 4,
	PLDM_RDE_SCHEMA_REGISTRY = 5,
};

enum pldm_rde_completion_codes {
	PLDM_RDE_BAD_CHECKSUM = 0x80,
	PLDM_RDE_CANNOT_CREATE_OPERATION = 0x81,
	PLDM_RDE_NOT_ALLOWED = 0x82,
	PLDM_RDE_WRONG_LOCATION_TYPE = 0x83,
	PLDM_RDE_OPERATION_ABANDONED = 0x84,
	PLDM_RDE_OPERATION_UNKILLABLE = 0x85,
	PLDM_RDE_ERROR_OPERATION_EXISTS = 0x86,
	PLDM_RDE_ERROR_OPERATION_FAILED = 0x87,
	PLDM_RDE_ERROR_UNEXPECTED = 0x88,
	PLDM_RDE_ERROR_UNSUPPORTED = 0x89,
	PLDM_RDE_ERROR_UNRECOGNIZED_CUSTOM_HEADER = 0x90,
	PLDM_RDE_ERROR_ETAG_MATCH = 0x91,
	PLDM_RDE_ERROR_NO_SUCH_RESOURCE = 0x92,
	PLDM_RDE_ERROR_ETAG_CALCULATION_ONGOING = 0x93,
};

/**
 * @brief Encode NegotiateRedfishParameters request.
 *
 * @param[in] instance_id - Message's instance id.
 * @param[in] mc_concurrency_support - MC concurrency support.
 * @param[in] mc_feature_support - MC feature support flags.
 * @param[out] msg - Request message.
 * @return pldm_completion_codes.
 */
int encode_negotiate_redfish_parameters_req(uint8_t instance_id,
					    uint8_t mc_concurrency_support,
					    bitfield16_t *mc_feature_support,
					    struct pldm_msg *msg);

/**
 * @brief Decode NegotiateRedfishParameters request.
 *
 * @param[in] msg - Request message.
 * @param[out] mc_concurrency_support - Pointer to a uint8_t variable.
 * @param[out] mc_feature_support - Pointer to a bitfield16_t variable.
 * @return pldm_completion_codes.
 */
int decode_negotiate_redfish_parameters_req(const struct pldm_msg *msg,
					    uint8_t *mc_concurrency_support,
					    bitfield16_t *mc_feature_support);

/**
 * @brief Encode NegotiateRedfishParameters response
 *
 * @param[in] instance_id - Message's instance id.
 * @param[in] completion_code - PLDM completion code.
 * @param[in] device_concurrency_support - Concurrency support.
 * @param[in] device_capabilities_flags - Capabilities flags.
 * @param[in] device_feature_support - Feature support flags.
 * @param[in] device_configuration_signature - RDE device signature.
 * @param[in] device_provider_name - Null terminated device provider name.
 * @param[in] name_format - String format of the device_provider_name.
* @param[in] payload_length - Length of the payload buffer.
 * @param[out] msg - Response message will be written to this.
 * @return pldm_completion_codes.
 */
int encode_negotiate_redfish_parameters_resp(
	uint8_t instance_id, uint8_t completion_code,
	uint8_t device_concurrency_support,
	bitfield8_t *device_capabilities_flags,
	bitfield16_t *device_feature_support,
	uint32_t device_configuration_signature,
	const char *device_provider_name,
	enum pldm_rde_varstring_format name_format, size_t payload_length,
	struct pldm_msg *msg);

/**
 * @brief Decode NegotiateRedfishParameters response.
 *
 * @param[in] msg - Response message.
 * @param[out] completion_code - PLDM completion code.
 * @param[in] payload_length - Length of the payload buffer.
 * @param[out] device_concurrency_support - Concurrency support.
 * @param[out] device_capabilities_flags - Capabilities flags.
 * @param[out] device_feature_support - Feature support flags.
 * @param[out] device_configuration_signature - RDE device signature.
 * @param[out] provider_name - Device provider name. provider_name.string_data
 * will point to the starting location of the provider name in the pldm_msg
 * buffer.
 * @return pldm_completion_codes
 */
int decode_negotiate_redfish_parameters_resp(
	const struct pldm_msg *msg, size_t payload_length,
	uint8_t *completion_code, uint8_t *device_concurrency_support,
	bitfield8_t *device_capabilities_flags,
	bitfield16_t *device_feature_support,
	uint32_t *device_configuration_signature,
	struct pldm_rde_varstring *provider_name);

/**
 * @brief Encode NegotiateMediumParameters request.
 *
 * @param[in] instance_id - Message's instance id.
 * @param[in] mc_max_transfer_size - Maximum amount of data the MC can
 * support for a single message transfer.
 * @param[out] msg - Request message.
 * @return pldm_completion_codes.
 */
int encode_negotiate_medium_parameters_req(uint8_t instance_id,
					   uint32_t mc_max_transfer_size,
					   struct pldm_msg *msg);

/**
 * @brief Decode NegotiateMediumParameters request.
 *
 * @param[in] msg - Request message.
 * @param[out] mc_max_transfer_size - Pointer to a uint32_t variable.
 * @return pldm_completion_codes.
 */
int decode_negotiate_medium_parameters_req(const struct pldm_msg *msg,
					   uint32_t *mc_max_transfer_size);

/**
 * @brief Decode Negotiate Medium Parameters response
 *
 * @param[in] instance_id - Message's instance id.
 * @param[in] completion_code - PLDM completion code.
 * @param[in] device_max_transfer_size - The maximum number of bytes that
 * the RDE Device can support in a chunk for a single message transfer
 * @param[in] payload_length - Length of the encoded payload segment.
 * @param[out] msg - Response message will be written to this.
 * @return pldm_completion_codes.
 */
int encode_negotiate_medium_parameters_resp(uint8_t instance_id,
					    uint8_t completion_code,
					    uint32_t device_max_transfer_size,
					    size_t payload_length,
					    struct pldm_msg *msg);

/**
 * @brief Decode Negotiate Medium Parameters response
 *
 * @param[in] msg: PLDM Msg byte array received from the responder
 * @param[in] payload_length: Length of the payload
 * @param[out] completion_code: Completion code as set by the responder
 * @param[out] device_max_transfer_size: Pointer to a uint32_t variable
 */
int decode_negotiate_medium_parameters_resp(const struct pldm_msg *msg,
					    size_t payload_length,
					    uint8_t *completion_code,
					    uint32_t *device_max_transfer_size);

/**
 * @brief Encode GetSchemaDictionary request.
 *
 * @param[in] instance_id - Message's instance id.
 * @param[in] resource_id - The ResourceID of any resource in the Redfish
 * Resource PDR.A ResourceID of 0xFFFF FFFF may be supplied to retrieve
 * dictionaries common to all RDE Device resources (such as the event or
 * annotation dictionary) without referring to an individual resource
 * @param[in] schema_class - The class of schema being requested.
 * @param[in] payload_length - Length of request message payload.
 * @param[out] msg - Request message.
 * @return pldm_completion_codes.
 */
int encode_get_schema_dictionary_req(uint8_t instance_id, uint32_t resource_id,
				     uint8_t schema_class,
				     size_t payload_length,
				     struct pldm_msg *msg);

/**
 * @brief Decode GetSchemaDictionary request.
 *
 * @param[in] msg - Request message.
 * @param[in] payload_length - Length of request message payload.
 * @param[out] resource_id - Pointer to a uint32_t variable.
 * @param[out] requested_schema_class - Pointer to a uint8_t variable.
 * @return pldm_completion_codes.
 */
int decode_get_schema_dictionary_req(const struct pldm_msg *msg,
				     size_t payload_length,
				     uint32_t *resource_id,
				     uint8_t *requested_schema_class);

/**
 * @brief Encode GetSchemaDictionary response.
 *
 * @param[in] instance_id - Message's instance id.
 * @param[in] completion_code - PLDM completion code.
 * @param[in] dictionary_format - The format of the dictionary.
 * @param[in] transfer_handle - A data transfer handle that the MC shall
 * use.
 * @param[in] payload_length - Length of request message payload.
 * @param[out] msg - Response message will be written to this.
 * @return pldm_completion_codes.
 */
int encode_get_schema_dictionary_resp(
	uint8_t instance_id, uint8_t completion_code, uint8_t dictionary_format,
	uint32_t transfer_handle, size_t payload_length, struct pldm_msg *msg);
/**
 * @brief Decode GetSchemaDictionary Response
 *
 * @param[in] msg - Response Message
 * @param[in] payload_length - Length of the payload
 * @param[out] completion_code - Completion Code
 * @param[out] dictionary_format - Dictionary Format for the particular
 * resource id
 * @param[out] transfer_handle - Transfer Handle to be used to get
 * dictionary
 */
int decode_get_schema_dictionary_resp(const struct pldm_msg *msg,
				      size_t payload_length,
				      uint8_t *completion_code,
				      uint8_t *dictionary_format,
				      uint32_t *transfer_handle);

/**
 * @brief Encode GetSchemaURI request.
 *
 * @param[in] instance_id - Message's instance id.
 * @param[in] resource_id - The ResourceID of a resource in the Redfish
 *            Resource PDR.A ResourceID of 0xFFFFFFFF may be supplied to
 *            retrieve URIs for schemas common to all RDE Device resources.
 * @param[in] req_schema_class - The class of schema being requested.
 * @param[in] oem_extension_number - Shall be zero for a standard
 *            DMTF-published schema,or the one-based OEM extension to a
 *            standard schema.
 * @param[out] msg - Request message.
 * @return pldm_completion_codes.
 */
int encode_get_schema_uri_req(uint8_t instance_id, uint32_t resource_id,
			      uint8_t req_schema_class,
			      uint8_t oem_extension_number,
			      struct pldm_msg *msg);

/**
 * @brief Decode GetSchemaURI request.
 *
 * @param[in] msg - Request message.
 * @param[out] resource_id - The ResourceID of a resource in the Redfish
 *             Resource PDR.
 * @param[out] req_schema_class - The class of schema being requested.
 * @param[out] oem_extension_number - The OEM extension number.
 * @return pldm_completion_codes.
 */
int decode_get_schema_uri_req(const struct pldm_msg *msg, uint32_t *resource_id,
			      uint8_t *req_schema_class,
			      uint8_t *oem_extension_number);

/**
 * @brief Encode GetSchemaURI response.
 *
 * @param[in] instance_id - Message's instance id.
 * @param[in] completion_code - Completion code of the response.
 * @param[in] string_fragment_count - The number of fragments into which the
 *            URI string is broken.
 * @param[in] schema_uri - Array of pointers to URI string fragments.
 * @param[in] payload_length - Length of response message payload.
 * @param[out] msg - Response message.
 * @return pldm_completion_codes.
 */
int encode_get_schema_uri_resp(uint8_t instance_id, uint8_t completion_code,
			       uint8_t string_fragment_count,
			       const struct pldm_rde_varstring *schema_uri,
			       struct pldm_msg *msg);

/**
 * @brief Decode GetSchemaURI response.
 * This function parses the response payload of a GetSchemaURI PLDM command.
 * It extracts the completion code, the number of URI string fragments, and
 * reconstructs the full schema URI from the fragments into a caller-provided
 * buffer.
 *
 * TODO: Need to revisit this memory allocation strategy
 * The caller must allocate a buffer large enough to hold the reconstructed
 * URI, including the null terminator. The buffer should be cast to a
 * `struct pldm_rde_varstring *` and passed to this function.

 *
 * @param[in] msg - Response message.
 * @param[out] completion_code - Completion code of the response.
 * @param[out] string_fragment_count - The number of fragments into which the
 *             URI string is broken.
 * @param[out] schema_uri_array - Pointer to a pre-allocated buffer to store
 *                                the reconstructed URI.
 * @param[in] payload_length - Max Length of response message payload.
 * @param[out] actual_uri_len - Actual length of the reconstructed URI.
 * @return pldm_completion_codes.
 *
 */
int decode_get_schema_uri_resp(const struct pldm_msg *msg,
			       uint8_t *completion_code,
			       uint8_t *string_fragment_count,
			       struct pldm_rde_varstring *schema_uri_array,
			       size_t payload_length, size_t *actual_uri_len);

/**
 * @brief Encode GetResourceETag request.
 *
 * @param[in] instance_id - Message's instance id.
 * @param[in] resource_id - The ResourceID of a resource in the Redfish
 * Resource PDR for the instance from which to get an ETag digest;
 * or 0xFFFF FFFF to get a global digest of all resource-based data
 * within the RDE Device. 
 * @param[out] msg - Request message.
 * @return pldm_completion_codes.
 */
int encode_get_resource_etag_req(uint8_t instance_id, uint32_t resource_id,
				 struct pldm_msg *msg);
/**
 * @brief Decode GetResourceETag request.
 *
 * @param[in] msg - Request message.
 * @param[out] resource_id - Pointer to a uint32_t variable.
 * @return pldm_completion_codes.
 */
int decode_get_resource_etag_req(const struct pldm_msg *msg,
				 uint32_t *resource_id);
/**
 * @brief Encode GetResourceETag response.
 *
 * @param[in] instance_id - Message's instance id.
 * @param[in] completion_code - PLDM completion code.
 * @param[in] etag_string_data - The RFC7232-compliant ETag string data; the string
 * text format shall be UTF-8. Either a strong or a weak etag may be returned.
 * This field shall be omitted if the CompletionCode is not SUCCESS.
 * @param[out] msg - Response message will be written to this.
 * @return pldm_completion_codes.
 */
int encode_get_resource_etag_resp(uint8_t instance_id, uint8_t completion_code,
				  const char *etag_string_data,
				  struct pldm_msg *msg);
/**
 * @brief Decode GetResourceETag Response
 *
 * @param[in] msg - Response Message
 * @param[in] payload_length - Length of the payload
 * @param[out] completion_code - Completion Code
 * @param[out] dictionary_format - Dictionary Format for the particular
 * resource id
 * @param[out] etag - The RFC7232-compliant ETag string data; the string
 * text format shall be UTF-8. Either a strong or a weak etag may be returned.
 * This field shall be omitted if the CompletionCode is not SUCCESS.
 * dictionary.
 * @return pldm_completion_codes.
 * Note: In the event that the RDE Device cannot provide a response to this
 * command within the PT1 time period (defined in DSP0240), the RDE Device
 * may provide completion code ETAG_CALCULATION_ONGOING and continue the
 * process of generating the ETag. The MC may then poll for the completed
 * ETag by repeating the same GetResourceETag command that it gave that
 * previously yielded this result. The RDE Device in turn shall signal
 * whether it has completed the calculation by responding with a completion
 * code of either SUCCESS (the calculation is done) or ETAG_CALCULATION_ONGOING
 * (otherwise). It is recommended that the MC delay for an integer multiple
 * of PT1 between retry attempts.
 */
int decode_get_resource_etag_resp(const struct pldm_msg *msg,
				  size_t payload_length,
				  uint8_t *completion_code,
				  struct pldm_rde_varstring *etag);

/**
 * @brief Encode RDEMultipartSend request.
 *
 * @param[in] instance_id - Message's instance id.
 * @param[in] data_transfer_handle - A handle to uniquely identify the chunk of data
 *  to be sent.
 * @param[in] operation_id - Identification number for this Operation;
 * @param[in] transfer_flag - An indication of current progress within the transfer.
 * @param[in] next_data_transfer_handle - The handle for the next chunk of data for this
 * transfer; zero (0x00000000) if no further data.
 * @param[in] data_length_bytes - The length in bytes N of data being sent in this chunk, 
 * including both the Data and DataIntegrityChecksum (if present) fields.
 * @param[in] data - The chunk of data bytes
 * @param[in] data_integrity_checksum - 32-bit CRC for the entirety of data.
 * @param[out] msg - Request message.
 * @return pldm_completion_codes.
 */
int encode_rde_multipart_send_req(uint8_t instance_id,
				  uint32_t data_transfer_handle,
				  rde_op_id operation_id, uint8_t transfer_flag,
				  uint32_t next_data_transfer_handle,
				  uint32_t data_length_bytes, uint8_t *data,
				  uint32_t data_integrity_checksum,
				  struct pldm_msg *msg);
/**
 * @brief Decode RDEMultipartSend request.
 *
 * @param[in] msg - Request message.
 * @param[in] payload_len - Request message lenght.
 * @param[out] data_transfer_handle - Pointer to a uint32_t variable.
 * @param[out] operation_id - Pointer to a uint16_t variable.
 * @param[out] transfer_flag - Pointer to a uint8_t variable.
 * @param[out] next_data_transfer_handle - Pointer to a uint32_t variable.
 * @param[out] data_length_bytes - Pointer to a uint32_t variable.
 * @param[out] data - Pointer to an of bytes.
 * @param[out] data_integrity_checksum - Pointer to a uint32_t variable.
 * @return pldm_completion_codes.
 */
int decode_rde_multipart_send_req(const struct pldm_msg *msg,
				  uint32_t payload_len,
				  uint32_t *data_transfer_handle,
				  rde_op_id *operation_id,
				  uint8_t *transfer_flag,
				  uint32_t *next_data_transfer_handle,
				  uint32_t *data_length_bytes, uint8_t *data,
				  uint32_t *data_integrity_checksum);
/**
 * @brief Encode RDEMultipartSend response.
 *
 * @param[in] instance_id - Message's instance id.
 * @param[in] completion_code - PLDM completion code.
 * @param[in] transfer_operation - The action that the RDE Device is requesting
 * to MC
 * @param[in] payload_length - Payload lenth bytes.
 * @param[out] msg - Response message will be written to this.
 * @return pldm_completion_codes.
 */
int encode_rde_multipart_send_resp(uint8_t instance_id, uint8_t completion_code,
				   uint8_t transfer_operation,
				   size_t payload_length, struct pldm_msg *msg);
/**
 * @brief Decode GetResourceETag Response
 *
 * @param[in] msg - Response Message
 * @param[in] payload_length - Length of the payload
 * @param[out] completion_code - Completion Code
 * @param[out] transfer_operation - Pointer that holds transfer_operation
 * @return pldm_completion_codes.
 */
int decode_rde_multipart_send_resp(const struct pldm_msg *msg,
				   size_t payload_length,
				   uint8_t *completion_code,
				   uint8_t *transfer_operation);
/**
 * @brief Encode RDEMultipartReceive request.
 *
 * @param[in] instance_id - Message's instance id.
 * @param[in] data_transfer_handle - A handle to uniquely identify the chunk
 * of data to be retrieved.
 * @param[in] operation_id - Identification number for this operation.
 * @param[in] transfer_operation - The portion of data requested for the
 * transfer.
 * @param[out] msg - Request will be written to this.
 * @return pldm_completion_codes.
 */
int encode_rde_multipart_receive_req(uint8_t instance_id,
				     uint32_t data_transfer_handle,
				     rde_op_id operation_id,
				     uint8_t transfer_operation,
				     struct pldm_msg *msg);

/**
 * @brief Decode RDEMultipartReceive request.
 *
 * @param[in] msg - Request message.
 * @param[in] payload_length - Length of request message payload.
 * @param[out] data_transfer_handle - A handle to uniquely identify the
 * chunk of data to be retrieved.
 * @param[out] operation_id - Identification number for this operation.
 * @param[out] transfer_operation - The portion of data requested for the
 * transfer.
 * @return pldm_completion_codes.
 */
int decode_rde_multipart_receive_req(const struct pldm_msg *msg,
				     size_t payload_length,
				     uint32_t *data_transfer_handle,
				     rde_op_id *operation_id,
				     uint8_t *transfer_operation);
/**
 * @brief Encode RDEMultipartReceive response.
 *
 * @param[in] instance_id - Message's instance id.
 * @param[in] completion_code - PLDM completion code.
 * @param[in] transfer_flag - The portion of data being sent to MC.
 * @param[in] next_data_transfer_handle - A handle to uniquely identify the
 * next chunk of data to be retrieved.
 * @param[in] data_length_bytes - Length of the payload.
* @param[in] data - Pointer to the data.
 * @param[in] data_integrity_checksum - Checksum.
 * @param[out] msg - Response message will be written to this.
 * @return pldm_completion_codes.
 */
int encode_rde_multipart_receive_resp(
	uint8_t instance_id, uint8_t completion_code, uint8_t transfer_flag,
	uint32_t next_data_transfer_handle, uint32_t data_length_bytes,
	const uint8_t *data, uint32_t data_integrity_checksum,
	struct pldm_msg *msg);
/**
 * @brief Decode RDE Multipart Receive Response
 *
 * @param[in] msg - Response message
 * @param[in] payload_length - Expected length of the response, since the
 * response could be equal to the negotiated transfer chunk size, the
 * requester should usually set it to the negotiated transfer size
 * @param[out] completion_code - Pointer to Completion code
 * @param[out] transfer_flag - Pointer to Transfer flag
 * @param[out] transfer_operation - Pointer to Transfer operation
 * @param[out] data_length_bytes - Pointer to the length of payload
 * @param[out] data - Pointer to the payload
 * @param[in] data_integrity_checksum - Pointer to checksum.
 */
int decode_rde_multipart_receive_resp(
	const struct pldm_msg *msg, size_t payload_length,
	uint8_t *completion_code, uint8_t *transfer_flag,
	uint32_t *data_transfer_handle, uint32_t *data_length_bytes,
	uint8_t *data, uint32_t *data_integrity_checksum);

#ifdef __cplusplus
}
#endif

#endif /* LIBPLDM_RDE_H */
