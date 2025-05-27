#include <libpldm/rde.h>

#include "base.h"
#include "msgbuf.h"

#include <endian.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

LIBPLDM_ABI_STABLE
int encode_negotiate_redfish_parameters_req(uint8_t instance_id,
					    uint8_t mc_concurrency_support,
					    bitfield16_t *mc_feature_support,
					    struct pldm_msg *msg)
{
	PLDM_MSGBUF_DEFINE_P(buf);
	int rc;

	if (msg == NULL || mc_concurrency_support == 0 ||
	    mc_feature_support == NULL) {
		return PLDM_ERROR_INVALID_DATA;
	}

	struct pldm_header_info header = { 0 };
	header.msg_type = PLDM_REQUEST;
	header.instance = instance_id;
	header.pldm_type = PLDM_RDE;
	header.command = PLDM_NEGOTIATE_REDFISH_PARAMETERS;
	rc = pack_pldm_header(&header, &(msg->hdr));
	if (rc) {
		return rc;
	}

	rc = pldm_msgbuf_init_errno(
		buf, PLDM_RDE_NEGOTIATE_REDFISH_PARAMETERS_REQ_BYTES,
		msg->payload, PLDM_RDE_NEGOTIATE_REDFISH_PARAMETERS_REQ_BYTES);
	if (rc) {
		return rc;
	}

	pldm_msgbuf_insert(buf, mc_concurrency_support);
	pldm_msgbuf_insert(buf, mc_feature_support->value);

	return pldm_msgbuf_complete(buf);
}

LIBPLDM_ABI_STABLE
int decode_negotiate_redfish_parameters_req(const struct pldm_msg *msg,
					    uint8_t *mc_concurrency_support,
					    bitfield16_t *mc_feature_support)
{
	PLDM_MSGBUF_DEFINE_P(buf);
	int rc;

	if (msg == NULL || mc_concurrency_support == NULL ||
	    mc_feature_support == NULL) {
		return PLDM_ERROR_INVALID_DATA;
	}

	rc = pldm_msgbuf_init_errno(
		buf, PLDM_RDE_NEGOTIATE_REDFISH_PARAMETERS_REQ_BYTES,
		msg->payload, PLDM_RDE_NEGOTIATE_REDFISH_PARAMETERS_REQ_BYTES);

	if (rc) {
		return rc;
	}

	pldm_msgbuf_extract_p(buf, mc_concurrency_support);
	if (*mc_concurrency_support == 0) {
		fprintf(stderr,
			"Concurrency support shall be greater than 0\n");
		return PLDM_ERROR_INVALID_DATA;
	}

	pldm_msgbuf_extract(buf, mc_feature_support->value);

	return pldm_msgbuf_complete(buf);
}
LIBPLDM_ABI_STABLE
int encode_negotiate_redfish_parameters_resp(
	uint8_t instance_id, uint8_t completion_code,
	uint8_t device_concurrency_support,
	bitfield8_t *device_capabilities_flags,
	bitfield16_t *device_feature_support,
	uint32_t device_configuration_signature,
	const char *device_provider_name,
	enum pldm_rde_varstring_format name_format, size_t payload_length,
	struct pldm_msg *msg)
{
	PLDM_MSGBUF_DEFINE_P(buf);
	int rc;

	if (msg == NULL || device_capabilities_flags == NULL ||
	    device_feature_support == NULL || device_provider_name == NULL) {
		return PLDM_ERROR_INVALID_DATA;
	}

	if (completion_code != PLDM_SUCCESS) {
		return encode_cc_only_resp(instance_id, PLDM_RDE,
					   PLDM_NEGOTIATE_REDFISH_PARAMETERS,
					   completion_code, msg);
	}

	// Length should include NULL terminator.
	size_t str_len = strlen(device_provider_name) + 1;
	if (str_len > 255) {
		return PLDM_ERROR_INVALID_DATA;
	}

	struct pldm_header_info header = { 0 };
	header.msg_type = PLDM_RESPONSE;
	header.instance = instance_id;
	header.pldm_type = PLDM_RDE;
	header.command = PLDM_NEGOTIATE_REDFISH_PARAMETERS;

	rc = pack_pldm_header(&header, &(msg->hdr));
	if (rc != PLDM_SUCCESS) {
		return rc;
	}

	rc = pldm_msgbuf_init_errno(
		buf, PLDM_RDE_NEGOTIATE_REDFISH_PARAMETERS_RESP_MIN_SIZE,
		msg->payload, payload_length);
	if (rc != PLDM_SUCCESS) {
		return rc;
	}

	pldm_msgbuf_insert(buf, completion_code);
	pldm_msgbuf_insert(buf, device_concurrency_support);
	pldm_msgbuf_insert(buf, device_capabilities_flags->byte);
	pldm_msgbuf_insert(buf, device_feature_support->value);
	pldm_msgbuf_insert(buf, device_configuration_signature);
	pldm_msgbuf_insert_uint8(buf, name_format);
	pldm_msgbuf_insert(buf, (uint8_t)str_len);
	rc = pldm_msgbuf_insert_array(
		buf, str_len, (const uint8_t *)device_provider_name, str_len);
	if (rc != PLDM_SUCCESS) {
		return rc;
	}

	return pldm_msgbuf_complete(buf);
}

LIBPLDM_ABI_STABLE
int decode_negotiate_redfish_parameters_resp(
	const struct pldm_msg *msg, size_t payload_length,
	uint8_t *completion_code, uint8_t *device_concurrency_support,
	bitfield8_t *device_capabilities_flags,
	bitfield16_t *device_feature_support,
	uint32_t *device_configuration_signature,
	struct pldm_rde_varstring *provider_name)
{
	PLDM_MSGBUF_DEFINE_P(buf);
	int rc;

	if (msg == NULL || completion_code == NULL ||
	    device_concurrency_support == NULL ||
	    device_capabilities_flags == NULL ||
	    device_feature_support == NULL ||
	    device_configuration_signature == NULL || provider_name == NULL) {
		return PLDM_ERROR_INVALID_DATA;
	}

	rc = pldm_msgbuf_init_errno(
		buf, PLDM_RDE_NEGOTIATE_REDFISH_PARAMETERS_RESP_MIN_SIZE,
		msg->payload, payload_length);
	if (rc != PLDM_SUCCESS) {
		fprintf(stderr, "init failed\n");
		return rc;
	}

	pldm_msgbuf_extract_p(buf, completion_code);
	if (*completion_code != PLDM_SUCCESS) {
		return PLDM_SUCCESS;
	}

	pldm_msgbuf_extract_p(buf, device_concurrency_support);
	pldm_msgbuf_extract_p(buf, &device_capabilities_flags->byte);
	pldm_msgbuf_extract_p(buf, &device_feature_support->value);
	pldm_msgbuf_extract_p(buf, device_configuration_signature);
	pldm_msgbuf_extract_p(buf, &provider_name->string_format);
	pldm_msgbuf_extract_p(buf, &provider_name->string_length_bytes);

	pldm_msgbuf_span_required(buf, provider_name->string_length_bytes,
				  (void **)&provider_name->string_data);

	return pldm_msgbuf_complete(buf);
}

LIBPLDM_ABI_STABLE
int encode_negotiate_medium_parameters_req(uint8_t instance_id,
					   uint32_t mc_max_transfer_size,
					   struct pldm_msg *msg)
{
	PLDM_MSGBUF_DEFINE_P(buf);
	int rc;

	if (msg == NULL) {
		return PLDM_ERROR_INVALID_DATA;
	}

	struct pldm_header_info header = { 0 };
	header.instance = instance_id;
	header.pldm_type = PLDM_RDE;
	header.msg_type = PLDM_REQUEST;
	header.command = PLDM_NEGOTIATE_MEDIUM_PARAMETERS;
	rc = pack_pldm_header(&header, &(msg->hdr));
	if (rc != PLDM_SUCCESS) {
		return rc;
	}

	rc = pldm_msgbuf_init_errno(
		buf, PLDM_RDE_NEGOTIATE_MEDIUM_PARAMETERS_REQ_BYTES,
		msg->payload, PLDM_RDE_NEGOTIATE_MEDIUM_PARAMETERS_REQ_BYTES);
	if (rc != PLDM_SUCCESS) {
		fprintf(stderr, "init failed\n");
		return rc;
	}

	pldm_msgbuf_insert(buf, mc_max_transfer_size);

	return pldm_msgbuf_complete(buf);
}

LIBPLDM_ABI_STABLE
int decode_negotiate_medium_parameters_req(const struct pldm_msg *msg,
					   uint32_t *mc_max_transfer_size)
{
	PLDM_MSGBUF_DEFINE_P(buf);
	int rc;

	if (msg == NULL || mc_max_transfer_size == NULL) {
		return PLDM_ERROR_INVALID_DATA;
	}

	rc = pldm_msgbuf_init_errno(
		buf, PLDM_RDE_NEGOTIATE_MEDIUM_PARAMETERS_REQ_BYTES,
		msg->payload, PLDM_RDE_NEGOTIATE_MEDIUM_PARAMETERS_REQ_BYTES);
	if (rc != PLDM_SUCCESS) {
		fprintf(stderr, "init failed\n");
		return rc;
	}

	pldm_msgbuf_extract_p(buf, mc_max_transfer_size);
	if (*mc_max_transfer_size < PLDM_RDE_MIN_TRANSFER_SIZE_BYTES) {
		return PLDM_ERROR_INVALID_DATA;
	}

	return pldm_msgbuf_complete(buf);
}

LIBPLDM_ABI_STABLE

int encode_negotiate_medium_parameters_resp(uint8_t instance_id,
					    uint8_t completion_code,
					    uint32_t device_max_transfer_size,
					    size_t payload_length,
					    struct pldm_msg *msg)
{
	PLDM_MSGBUF_DEFINE_P(buf);
	int rc;

	if (NULL == msg) {
		return PLDM_ERROR_INVALID_DATA;
	}

	struct pldm_header_info header = { 0 };
	header.msg_type = PLDM_RESPONSE;
	header.instance = instance_id;
	header.pldm_type = PLDM_RDE;
	header.command = PLDM_NEGOTIATE_MEDIUM_PARAMETERS;

	rc = pack_pldm_header(&header, &(msg->hdr));
	if (rc != PLDM_SUCCESS) {
		return rc;
	}

	rc = pldm_msgbuf_init_errno(
		buf, PLDM_RDE_NEGOTIATE_MEDIUM_PARAMETERS_RESP_BYTES,
		msg->payload, payload_length);
	if (rc != PLDM_SUCCESS) {
		fprintf(stderr, "init failed\n");
		return rc;
	}

	pldm_msgbuf_insert(buf, completion_code);
	if (completion_code != PLDM_SUCCESS) {
		return PLDM_SUCCESS;
	}

	pldm_msgbuf_insert(buf, device_max_transfer_size);

	return pldm_msgbuf_complete(buf);
}

LIBPLDM_ABI_STABLE
int decode_negotiate_medium_parameters_resp(const struct pldm_msg *msg,
					    size_t payload_length,
					    uint8_t *completion_code,
					    uint32_t *device_max_transfer_size)
{
	PLDM_MSGBUF_DEFINE_P(buf);
	int rc;

	if (msg == NULL || completion_code == NULL ||
	    device_max_transfer_size == NULL) {
		return PLDM_ERROR_INVALID_DATA;
	}

	rc = pldm_msgbuf_init_errno(
		buf, PLDM_RDE_NEGOTIATE_MEDIUM_PARAMETERS_RESP_BYTES,
		msg->payload, payload_length);
	if (rc != PLDM_SUCCESS) {
		fprintf(stderr, "init failed\n");
		return rc;
	}

	pldm_msgbuf_extract_p(buf, completion_code);
	if (*completion_code != PLDM_SUCCESS) {
		return PLDM_SUCCESS;
	}

	pldm_msgbuf_extract_p(buf, device_max_transfer_size);

	return pldm_msgbuf_complete(buf);
}

LIBPLDM_ABI_STABLE
int encode_get_schema_dictionary_req(uint8_t instance_id, uint32_t resource_id,
				     uint8_t schema_class,
				     size_t payload_length,
				     struct pldm_msg *msg)
{
	PLDM_MSGBUF_DEFINE_P(buf);
	int rc;

	if (msg == NULL) {
		return PLDM_ERROR_INVALID_DATA;
	}

	struct pldm_header_info header = { 0 };
	header.instance = instance_id;
	header.pldm_type = PLDM_RDE;
	header.msg_type = PLDM_REQUEST;
	header.command = PLDM_GET_SCHEMA_DICTIONARY;
	rc = pack_pldm_header(&header, &(msg->hdr));
	if (rc != PLDM_SUCCESS) {
		return rc;
	}

	rc = pldm_msgbuf_init_errno(buf, PLDM_RDE_SCHEMA_DICTIONARY_REQ_BYTES,
				    msg->payload, payload_length);
	if (rc != PLDM_SUCCESS) {
		fprintf(stderr, "init failed\n");
		return rc;
	}

	pldm_msgbuf_insert(buf, resource_id);
	pldm_msgbuf_insert(buf, schema_class);

	return pldm_msgbuf_complete(buf);
}

LIBPLDM_ABI_STABLE
int decode_get_schema_dictionary_req(const struct pldm_msg *msg,
				     size_t payload_length,
				     uint32_t *resource_id,
				     uint8_t *requested_schema_class)
{
	PLDM_MSGBUF_DEFINE_P(buf);
	int rc;

	if (msg == NULL || resource_id == NULL ||
	    requested_schema_class == NULL) {
		return PLDM_ERROR_INVALID_DATA;
	}

	if (payload_length != PLDM_RDE_SCHEMA_DICTIONARY_REQ_BYTES) {
		return PLDM_ERROR_INVALID_LENGTH;
	}

	rc = pldm_msgbuf_init_errno(buf, PLDM_RDE_SCHEMA_DICTIONARY_REQ_BYTES,
				    msg->payload, payload_length);
	if (rc != PLDM_SUCCESS) {
		fprintf(stderr, "init failed\n");
		return rc;
	}

	pldm_msgbuf_extract_p(buf, resource_id);
	pldm_msgbuf_extract_p(buf, requested_schema_class);
	if (*requested_schema_class > PLDM_RDE_SCHEMA_REGISTRY) {
		return PLDM_ERROR_INVALID_DATA;
	}

	return pldm_msgbuf_complete(buf);
}

LIBPLDM_ABI_STABLE
int encode_get_schema_dictionary_resp(
	uint8_t instance_id, uint8_t completion_code, uint8_t dictionary_format,
	uint32_t transfer_handle, size_t payload_length, struct pldm_msg *msg)
{
	PLDM_MSGBUF_DEFINE_P(buf);
	int rc;

	if (NULL == msg) {
		return PLDM_ERROR_INVALID_DATA;
	}

	struct pldm_header_info header = { 0 };
	header.msg_type = PLDM_RESPONSE;
	header.instance = instance_id;
	header.pldm_type = PLDM_RDE;
	header.command = PLDM_GET_SCHEMA_DICTIONARY;
	rc = pack_pldm_header(&header, &(msg->hdr));
	if (rc != PLDM_SUCCESS) {
		return rc;
	}

	rc = pldm_msgbuf_init_errno(buf, PLDM_RDE_SCHEMA_DICTIONARY_RESP_BYTES,
				    msg->payload, payload_length);
	if (rc != PLDM_SUCCESS) {
		return rc;
	}

	pldm_msgbuf_insert(buf, completion_code);
	if (completion_code != PLDM_SUCCESS) {
		return PLDM_SUCCESS;
	}

	pldm_msgbuf_insert(buf, dictionary_format);
	pldm_msgbuf_insert(buf, transfer_handle);

	return pldm_msgbuf_complete(buf);
}

LIBPLDM_ABI_STABLE
int decode_get_schema_dictionary_resp(const struct pldm_msg *msg,
				      size_t payload_length,
				      uint8_t *completion_code,
				      uint8_t *dictionary_format,
				      uint32_t *transfer_handle)
{
	PLDM_MSGBUF_DEFINE_P(buf);
	int rc;

	if ((msg == NULL) || (dictionary_format == NULL) ||
	    (completion_code == NULL) || (transfer_handle == NULL)) {
		return PLDM_ERROR_INVALID_DATA;
	}

	rc = pldm_msgbuf_init_errno(buf, PLDM_RDE_SCHEMA_DICTIONARY_RESP_BYTES,
				    msg->payload, payload_length);
	if (rc != PLDM_SUCCESS) {
		return rc;
	}

	pldm_msgbuf_extract_p(buf, completion_code);
	if (*completion_code != PLDM_SUCCESS) {
		return PLDM_SUCCESS;
	}

	pldm_msgbuf_extract_p(buf, dictionary_format);
	pldm_msgbuf_extract_p(buf, transfer_handle);

	if (*dictionary_format > PLDM_RDE_SCHEMA_REGISTRY) {
		return PLDM_ERROR_INVALID_DATA;
	}
	return pldm_msgbuf_complete(buf);
}

LIBPLDM_ABI_STABLE
int encode_get_schema_uri_req(uint8_t instance_id, uint32_t resource_id,
			      uint8_t req_schema_class,
			      uint8_t oem_extension_number,
			      struct pldm_msg *msg)
{
	PLDM_MSGBUF_DEFINE_P(buf);
	int rc;

	if (msg == NULL) {
		return PLDM_ERROR_INVALID_DATA;
	}

	struct pldm_header_info header = { 0 };
	header.instance = instance_id;
	header.pldm_type = PLDM_RDE;
	header.msg_type = PLDM_REQUEST;
	header.command = PLDM_GET_SCHEMA_URI;
	rc = pack_pldm_header(&header, &(msg->hdr));
	if (rc != PLDM_SUCCESS) {
		return rc;
	}

	rc = pldm_msgbuf_init_errno(buf, PLDM_RDE_SCHEMA_URI_REQ_BYTES,
				    msg->payload,
				    PLDM_RDE_SCHEMA_URI_REQ_BYTES);
	if (rc != PLDM_SUCCESS) {
		fprintf(stderr, "init failed\n");
		return rc;
	}

	pldm_msgbuf_insert(buf, resource_id);
	pldm_msgbuf_insert(buf, req_schema_class);
	pldm_msgbuf_insert(buf, oem_extension_number);

	return pldm_msgbuf_complete(buf);
}

LIBPLDM_ABI_STABLE
int decode_get_schema_uri_req(const struct pldm_msg *msg, uint32_t *resource_id,
			      uint8_t *req_schema_class,
			      uint8_t *oem_extension_number)
{
	PLDM_MSGBUF_DEFINE_P(buf);
	int rc;

	if (msg == NULL || resource_id == NULL || req_schema_class == NULL ||
	    oem_extension_number == NULL) {
		return PLDM_ERROR_INVALID_DATA;
	}

	rc = pldm_msgbuf_init_errno(buf, PLDM_RDE_SCHEMA_URI_REQ_BYTES,
				    msg->payload,
				    PLDM_RDE_SCHEMA_URI_REQ_BYTES);
	if (rc != PLDM_SUCCESS) {
		fprintf(stderr, "init failed\n");
		return rc;
	}

	pldm_msgbuf_extract_p(buf, resource_id);
	pldm_msgbuf_extract_p(buf, req_schema_class);
	pldm_msgbuf_extract_p(buf, oem_extension_number);

	return pldm_msgbuf_complete(buf);
}

LIBPLDM_ABI_STABLE
int encode_get_schema_uri_resp(uint8_t instance_id, uint8_t completion_code,
			       uint8_t string_fragment_count,
			       const struct pldm_rde_varstring *schema_uri,
			       struct pldm_msg *msg)
{
	PLDM_MSGBUF_DEFINE_P(buf);
	int rc;

	if (!msg || !schema_uri || string_fragment_count == 0) {
		return PLDM_ERROR_INVALID_DATA;
	}

	// Set up PLDM header
	struct pldm_header_info header = { 0 };
	header.msg_type = PLDM_RESPONSE;
	header.instance = instance_id;
	header.pldm_type = PLDM_RDE;
	header.command = PLDM_GET_SCHEMA_URI;

	rc = pack_pldm_header(&header, &(msg->hdr));
	if (rc != PLDM_SUCCESS) {
		return rc;
	}

	size_t payload_length = PLDM_RDE_SCHEMA_URI_RESP_FIXED_BYTES;
	for (int i = 0; i < string_fragment_count; ++i) {
		payload_length += PLDM_RDE_VARSTRING_HEADER_SIZE +
				  schema_uri[i].string_length_bytes;
	}

	rc = pldm_msgbuf_init_errno(buf, payload_length, msg->payload,
				    payload_length);
	if (rc != PLDM_SUCCESS) {
		return rc;
	}

	// Insert fixed fields
	pldm_msgbuf_insert(buf, completion_code);
	pldm_msgbuf_insert(buf, string_fragment_count);

	// Insert each varstring
	for (uint8_t i = 0; i < string_fragment_count; ++i) {
		const struct pldm_rde_varstring *entry = &schema_uri[i];

		if (!entry->string_data || entry->string_length_bytes == 0) {
			fprintf(stderr,
				"Invalid entry at index %d: data=%p, length=%d\n",
				i, entry->string_data,
				entry->string_length_bytes);
			return PLDM_ERROR_INVALID_DATA;
		}

		size_t str_len = entry->string_length_bytes;

		pldm_msgbuf_insert_uint8(buf, entry->string_format);
		pldm_msgbuf_insert_uint8(buf, str_len);

		rc = pldm_msgbuf_insert_array_char(
			buf, str_len, (const char *)entry->string_data,
			str_len);
		if (rc != PLDM_SUCCESS) {
			fprintf(stderr,
				"Insert failed at index (%d) remaining:%jd\n",
				i, buf->remaining);
			return rc;
		}
	}

	return pldm_msgbuf_complete(buf);
}

LIBPLDM_ABI_STABLE
int decode_get_schema_uri_resp(const struct pldm_msg *msg,
			       uint8_t *completion_code,
			       uint8_t *string_fragment_count,
			       struct pldm_rde_varstring *schema_uri_array,
			       size_t payload_length, size_t *actual_uri_len)
{
	PLDM_MSGBUF_DEFINE_P(buf);
	int rc;

	if (!msg || !completion_code || !string_fragment_count) {
		return PLDM_ERROR_INVALID_DATA;
	}

	rc = pldm_msgbuf_init_errno(buf, PLDM_RDE_SCHEMA_URI_RESP_FIXED_BYTES,
				    msg->payload, payload_length);
	if (rc != PLDM_SUCCESS) {
		return rc;
	}

	pldm_msgbuf_extract_p(buf, completion_code);
	if (*completion_code != PLDM_SUCCESS) {
		return PLDM_SUCCESS;
	}

	rc = pldm_msgbuf_extract_p(buf, string_fragment_count);
	if (rc != PLDM_SUCCESS) {
		return rc;
	}

	if (*string_fragment_count == 0) {
		return PLDM_ERROR_INVALID_DATA;
	}

	*actual_uri_len = 0;

	for (uint8_t i = 0; i < *string_fragment_count; ++i) {
		uint8_t format = 0;
		uint8_t length = 0;

		rc = pldm_msgbuf_extract(buf, format);
		if (rc != PLDM_SUCCESS) {
			return rc;
		}

		rc = pldm_msgbuf_extract(buf, length);

		if (rc != PLDM_SUCCESS || length <= 1) {
			return PLDM_ERROR_INVALID_LENGTH;
		}

		*actual_uri_len += length;
		if (*actual_uri_len >
		    (payload_length - PLDM_RDE_SCHEMA_URI_RESP_FIXED_BYTES)) {
			return PLDM_ERROR_INVALID_LENGTH;
		}

		schema_uri_array[i].string_format = format;
		schema_uri_array[i].string_length_bytes = length;

		void *span_ptr = NULL;

		rc = pldm_msgbuf_span_required(buf, length - 1, &span_ptr);
		if (rc != PLDM_SUCCESS) {
			return rc;
		}
		schema_uri_array[i].string_data = (char *)span_ptr;

		// Extract null terminator
		uint8_t dummy;
		rc = pldm_msgbuf_extract_p(buf, &dummy);
		if (rc != PLDM_SUCCESS) {
			return rc;
		}
		// Ensure null-termination
		schema_uri_array[i].string_data[length - 1] = '\0';
	}

	return pldm_msgbuf_complete(buf);
}
