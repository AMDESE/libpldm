/* SPDX-License-Identifier: Apache-2.0 OR GPL-2.0-or-later */
#ifndef RDE_H
#define RDE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <libpldm/base.h>
#include <stddef.h>
#include <stdint.h>

/* Response lengths are inclusive of completion code */
#define PLDM_RDE_NEGOTIATE_REDFISH_PARAMETERS_REQ_BYTES	    3
#define PLDM_RDE_NEGOTIATE_REDFISH_PARAMETERS_RESP_MIN_SIZE 12

enum pldm_rde_commands {
	PLDM_NEGOTIATE_REDFISH_PARAMETERS = 0x01,
};

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

/**
 * @brief Encode NegotiateRedfishParameters request.
 *
 * @param[in] instance_id - Message's instance id.
 * @param[in] mc_concurrency_support - MC concurrency support.
 * @param[in] mc_feature_support - MC feature support flags.
 * @param[in] payload_length - Length of the encoded payload segment.
 * @param[out] msg - Request message.
 * @return pldm_completion_codes.
 */
int encode_rde_negotiate_redfish_parameters_req(
	uint8_t instance_id, uint8_t mc_concurrency_support,
	bitfield16_t *mc_feature_support, size_t payload_length,
	struct pldm_msg *msg);

/**
 * @brief Decode NegotiateRedfishParameters request.
 *
 * @param[in] msg - Request message.
 * @param[in] payload_length - Length of request message payload.
 * @param[out] mc_concurrency_support - Pointer to a uint8_t variable.
 * @param[out] mc_feature_support - Pointer to a bitfield16_t variable.
 * @return pldm_completion_codes.
 */
int decode_rde_negotiate_redfish_parameters_req(
	const struct pldm_msg *msg, size_t payload_length,
	uint8_t *mc_concurrency_support, bitfield16_t *mc_feature_support);

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
int encode_rde_negotiate_redfish_parameters_resp(
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
int decode_rde_negotiate_redfish_parameters_resp(
	const struct pldm_msg *msg, size_t payload_length,
	uint8_t *completion_code, uint8_t *device_concurrency_support,
	bitfield8_t *device_capabilities_flags,
	bitfield16_t *device_feature_support,
	uint32_t *device_configuration_signature,
	struct pldm_rde_varstring *provider_name);

#ifdef __cplusplus
}
#endif

#endif /* LIBPLDM_RDE_H */
