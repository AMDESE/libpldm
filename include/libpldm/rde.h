/* SPDX-License-Identifier: Apache-2.0 OR GPL-2.0-or-later */
#ifndef LIBPLDM_RDE_H
#define LIBPLDM_RDE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <libpldm/base.h>
#include <stddef.h>
#include <stdint.h>

/** @brief PLDM for Redfish Device Completion Code */
enum pldm_rde_completion_codes {
	PLDM_RDE_CC_ERROR_BAD_CHECKSUM = 0x80,
	PLDM_RDE_CC_ERROR_CANNOT_CREATE_OPERATION = 0x81,
	PLDM_RDE_CC_ERROR_NOT_ALLOWED = 0x82,
	PLDM_RDE_CC_ERROR_WRONG_LOCATION_TYPE = 0x83,
	PLDM_RDE_CC_ERROR_OPERATION_ABANDONED = 0x84,
	PLDM_RDE_CC_ERROR_OPERATION_UNKILLABLE = 0x85,
	PLDM_RDE_CC_ERROR_OPERATION_EXISTS = 0x86,
	PLDM_RDE_CC_ERROR_OPERATION_FAILED = 0x87,
	PLDM_RDE_CC_ERROR_UNEXPECTED = 0x88,
	PLDM_RDE_CC_ERROR_UNSUPPORTED = 0x89,
	PLDM_RDE_CC_ERROR_UNRECOGNIZED_CUSTOM_HEADER = 0x90,
	PLDM_RDE_CC_ERROR_ETAG_MATCH = 0x91,
	PLDM_RDE_CC_ERROR_NO_SUCH_RESOURCE = 0x92,
	PLDM_RDE_CC_ETAG_CALCULATION_ONGOING = 0x93,
	PLDM_RDE_CC_ERROR_INSUFFICIENT_STORAGE = 0x94,
};

/** @brief PLDM for Redfish Device Enablement Command */
enum pldm_rde_commands {
	PLDM_RDE_CMD_NEGOTIATE_REDFISH_PARAMETERS = 0x01,
	PLDM_RDE_CMD_NEGOTIATE_MEDIUM_PARAMETERS = 0x02,
	PLDM_RDE_CMD_GET_SCHEMA_DICTIONARY = 0x03,
	PLDM_RDE_CMD_GET_SCHEMA_URI = 0x04,
	PLDM_RDE_CMD_GET_RESOURCE_ETAG = 0x05,
	PLDM_RDE_CMD_GET_OEM_COUNT = 0x06,
	PLDM_RDE_CMD_GET_OEM_NAME = 0x07,
	PLDM_RDE_CMD_GET_REGISTRY_COUNT = 0x08,
	PLDM_RDE_CMD_GET_REGISTRY_DETAILS = 0x09,
	PLDM_RDE_CMD_SELECT_REGISTRY_VERSION = 0x0a,
	PLDM_RDE_CMD_GET_MESSAGE_REGISTRY = 0x0b,
	PLDM_RDE_CMD_GET_SCHEMA_FILE = 0x0c,
	PLDM_RDE_CMD_RDE_OPERATION_INIT = 0x10,
	PLDM_RDE_CMD_SUPPLY_CUSTOM_REQUEST_PARAMETERS = 0x11,
	PLDM_RDE_CMD_RETRIEVE_CUSTOM_RESPONSE_PARAMETERS = 0x12,
	PLDM_RDE_CMD_RDE_OPERATION_COMPLETE = 0x13,
	PLDM_RDE_CMD_RDE_OPERATION_STATUS = 0x14,
	PLDM_RDE_CMD_RDE_OPERATION_KILL = 0x15,
	PLDM_RDE_CMD_RDE_OPERATION_ENUMERATE = 0x16,
	PLDM_RDE_CMD_RDE_MULTIPART_SEND = 0x30,
	PLDM_RDE_CMD_RDE_MULTIPART_RECEIVE = 0x31,
};

/* Compatibility aliases used by AMD encode/decode implementations */
#define PLDM_NEGOTIATE_REDFISH_PARAMETERS \
	PLDM_RDE_CMD_NEGOTIATE_REDFISH_PARAMETERS
#define PLDM_NEGOTIATE_MEDIUM_PARAMETERS \
	PLDM_RDE_CMD_NEGOTIATE_MEDIUM_PARAMETERS

/* Response lengths are inclusive of completion code */
#define PLDM_RDE_NEGOTIATE_REDFISH_PARAMETERS_REQ_BYTES	    3
#define PLDM_RDE_NEGOTIATE_REDFISH_PARAMETERS_RESP_MIN_SIZE 12
#define PLDM_RDE_NEGOTIATE_MEDIUM_PARAMETERS_REQ_BYTES	    4
#define PLDM_RDE_NEGOTIATE_MEDIUM_PARAMETERS_RESP_BYTES	    5
#define PLDM_RDE_MIN_TRANSFER_SIZE_BYTES		    64

enum pldm_rde_varstring_format {
	PLDM_RDE_VARSTRING_UNKNOWN = 0,
	PLDM_RDE_VARSTRING_ASCII = 1,
	PLDM_RDE_VARSTRING_UTF_8 = 2,
	PLDM_RDE_VARSTRING_UTF_16 = 3,
	PLDM_RDE_VARSTRING_UTF_16LE = 4,
	PLDM_RDE_VARSTRING_UTF_16BE = 5,
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

/* @brief varstring PLDM data type */
struct pldm_rde_varstring {
	uint8_t string_format;	     // Format of the string
	uint8_t string_length_bytes; // Length of the string including NULL terminator
	char *string_data; // Pointer to the string data, should be NULL terminated
};

int encode_negotiate_redfish_parameters_req(uint8_t instance_id,
					    uint8_t mc_concurrency_support,
					    bitfield16_t *mc_feature_support,
					    struct pldm_msg *msg);

int decode_negotiate_redfish_parameters_req(const struct pldm_msg *msg,
					    uint8_t *mc_concurrency_support,
					    bitfield16_t *mc_feature_support);

int encode_negotiate_redfish_parameters_resp(
	uint8_t instance_id, uint8_t completion_code,
	uint8_t device_concurrency_support,
	bitfield8_t *device_capabilities_flags,
	bitfield16_t *device_feature_support,
	uint32_t device_configuration_signature,
	const char *device_provider_name,
	enum pldm_rde_varstring_format name_format, size_t payload_length,
	struct pldm_msg *msg);

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

#ifdef __cplusplus
}
#endif

#endif /* LIBPLDM_RDE_H */
