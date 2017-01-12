
/**
 * @file /magma/src/providers/prime/prime.h
 *
 * @brief DESCRIPTIONxxxGOESxxxHERE
 *
 * $Author$
 * $Date$
 * $Revision$
 *
 */

#ifndef PRIME_H
#define PRIME_H

#define SECP256K1_KEY_PUB_LEN 33
#define SECP256K1_KEY_PRIV_LEN 32
#define SECP256K1_SHARED_SECRET_LEN 32

#define ED25519_KEY_PUB_LEN 32
#define ED25519_KEY_PRIV_LEN 32
#define ED25519_SIGNATURE_LEN 64

#define AES_TAG_LEN 16
#define AES_KEY_LEN 32
#define AES_BLOCK_LEN 16
#define AES_VECTOR_LEN 16

typedef enum {
	ED25519_PUB,
	ED25519_PRIV
} ed25519_key_type_t;

typedef enum {
	BINARY,                              /**< Serialized object in binary form. >*/
	ARMORED                              /**< An object that has been aromored using the Privacy Enhanced Message format. >*/
} prime_encoding_t;

typedef enum {
	NONE,
	SECURITY                             /**< Store the object in secure memory. >*/
} prime_flags_t;

typedef enum {
    PRIME_ORG_SIGNET = 1776,               /**< Organizational signet. >*/
	PRIME_ORG_KEY = 1952,                  /**< Organizational key. >*/
	PRIME_ORG_KEY_ENCRYPTED = 1947,        /**< Encrypted organizational key. >*/

	PRIME_USER_SIGNING_REQUEST = 1215,     /**< User signing request. >*/
    PRIME_USER_SIGNET = 1789,              /**< User signet. >*/
	PRIME_USER_KEY = 2013,                 /**< User key. >*/
	PRIME_USER_KEY_ENCRYPTED = 1976,       /**< Encrypted user key. >*/

    PRIME_MESSAGE_ENCRYPTED = 1847,        /**< An encrypted message. >*/
    PRIME_MESSAGE_SENT = 1851,             /**< An encrypted, appended, sent message. >*/
    PRIME_MESSAGE_DRAFT = 1861,            /**< An encrypted, appended, message draft. >*/
    PRIME_MESSAGE_NAKED = 1908,            /**< An encrypted, imported, unstructured, naked message. >*/

    PRIME_MESSAGE_BOUNCE = 1931,           /**< An encapsulated, encrypted message, that has bounced. >*/
    PRIME_MESSAGE_FORWARD = 1948,          /**< An encapsulated, encrypted message, that has been forwarded. >*/
    PRIME_MESSAGE_ABUSE = 2001,            /**< An encapsulated, encrypted message, sent as an abuse complaint. >*/

    PRIME_BINARY_OBJECT = 1837,            /**< A binary object. >*/
    PRIME_PROTOCOL_TICKET = 1841           /**< An encrypted protocol ticket. >*/
} prime_type_t;

typedef enum {
    PRIME_CHUNK_ENVELOPE = 1,              /**< Envelope block. >*/
	PRIME_CHUNK_EPHEMERAL = 2,             /**< Ephemeral chunk. >*/
	PRIME_CHUNK_ORIGIN = 3,                /**< Origin chunk. >*/
	PRIME_CHUNK_DESTINATION = 4,           /**< Destination chunk. >*/

	PRIME_CHUNK_METADATA = 32,             /**< Metadata block. >*/
	PRIME_CHUNK_COMMON = 33,               /**< Common headers chunk. >*/
	PRIME_CHUNK_HEADERS = 34,              /**< Remaining headers chunk. >*/

	PRIME_CHUNK_BODY = 34,                 /**< Body block. >*/
	PRIME_CHUNK_GENERIC = 35               /**< Generic chunk type. >*/
} prime_message_chunks_t;

typedef enum {
	ORG_PRIMARY_KEY = 1,
	ORG_SECONDARY_KEY = 2,
	ORG_ENCRYPTION_KEY = 3
} prime_org_key_fields_t;

typedef enum {
	ORG_PRIMARY_KEY = 1,
	ORG_SECONDARY_KEY = 2,
	ORG_ENCRYPTION_KEY = 3,
	ORG_SELF_SIGNATURE = 4,
	ORG_FULL_SIGNATURE = 253,
	ORG_IDENTIFIER = 254,
	ORG_IDENTIFIABLE_SIGNATURE = 255
} prime_org_signet_fields_t;

typedef enum {
	USER_SIGNING_KEY = 1,
	USER_ENCRYPTION_KEY = 2,
	USER_ALTERNATE_ENCRYPTION_KEY = 3,
} prime_user_key_fields_t;

typedef enum {
	USER_SIGNING_KEY = 1,
	USER_ENCRYPTION_KEY = 2,
	USER_ALTERNATE_ENCRYPTION_KEY = 3,
	USER_CUSTODY_SIGNATURE = 4,
	USER_SELF_SIGNATURE = 5,
	USER_CRYPTO_SIGNATURE = 6,
	USER_FULL_SIGNATURE = 253,
	USER_IDENTIFIER = 254,
	USER_IDENTIFIABLE_SIGNATURE = 255
} prime_user_signet_fields_t;

typedef enum {
	USER_SIGNING_KEY = 1,
	USER_ENCRYPTION_KEY = 2,
	USER_ALTERNATE_ENCRYPTION_KEY = 3,
	USER_CUSTODY_SIGNATURE = 4,
	USER_SELF_SIGNATURE = 5
} prime_user_signing_request_fields_t;

// Allows the inclusion of this PRIME header without having to include the OpenSSL headers.
#ifdef HEADER_EC_H
typedef EC_KEY secp256k1_key_t;
#else
typedef void secp256k1_key_t;
#endif

typedef struct __attribute__ ((packed)) {
	ed25519_key_type_t type;
	struct __attribute__ ((packed)) {
		uint8_t private[ED25519_KEY_PRIV_LEN];
		uint8_t public[ED25519_KEY_PUB_LEN];
	};
} ed25519_key_t;

typedef struct __attribute__ ((packed)) {
	ed25519_key_t *signing;
	secp256k1_key_t *encryption;
} prime_user_key_t;

typedef struct __attribute__ ((packed)) {
	ed25519_key_t *signing;
	secp256k1_key_t *encryption;
} prime_org_key_t;

typedef struct __attribute__ ((packed)) {
	ed25519_key_t *signing;                /**< User signing key, field 1. >*/
	secp256k1_key_t *encryption;           /**< User encryption key, field 2. >*/
	struct {
		stringer_t *custody;               /**< User chain of custody signature, field 4. >*/
		stringer_t *user;                  /**< User signature, field 5. >*/
		stringer_t *org;                   /**< Organizational signature, field 6. >*/
	} signatures;
} prime_user_signet_t;

typedef struct __attribute__ ((packed)) {
	ed25519_key_t *signing;                /**< Organizational signing key, field 1. >*/
	secp256k1_key_t *encryption;           /**< Organizational encryption key, field 3. >*/
	stringer_t *signature;                 /**< Organizational signature, field 4. >*/
} prime_org_signet_t;

typedef struct __attribute__ ((packed)) {
	uint8_t type;
	stringer_t *buffer;
} prime_ephemeral_chunk_t;

typedef struct __attribute__ ((packed)) {
	struct {
		uint8_t type;                      /**< Chunk type, 1 through 255. >*/
		uint32_t length;                   /**< Payload length, must be divisible by 16. >*/
		uint32_t serialized;               /**< Serialized form, a 1 byte type, and 3 byte big endian length. >*/
	} header;
	struct {
		struct {
			placer_t signature;
			uint32_t length;
			uint8_t flags;
			uint8_t pad;
			placer_t data;
			placer_t trailing;
			stringer_t *buffer;
		} plain;
		struct {
			stringer_t *buffer;
		} encrypted;
	} payload;
	struct {
		stringer_t *author;
		stringer_t *origin;
		stringer_t *destination;
		stringer_t *recipient;
	} slots;
} prime_encrypted_chunk_t;

typedef struct __attribute__ ((packed)) {
	stringer_t *signature;
	struct {
		stringer_t *author;
		stringer_t *origin;
		stringer_t *destination;
		stringer_t *recipient;
	} slots;
} prime_signature_chunk_t;

typedef struct __attribute__ ((packed)) {
	struct {
		stringer_t *kek;
		secp256k1_key_t *ephemeral;
		secp256k1_key_t *author;
		secp256k1_key_t *origin;
		secp256k1_key_t *destination;
		secp256k1_key_t *recipient;
	} keys;
	struct {
		prime_ephemeral_chunk_t *ephemeral;
		prime_encrypted_chunk_t *origin;
		prime_encrypted_chunk_t *destination;
	} envelope;
	struct {
		prime_encrypted_chunk_t *common;
		prime_encrypted_chunk_t *other;
	} metadata;
	struct {
		prime_encrypted_chunk_t *body;
	} content;
	struct {
		prime_signature_chunk_t *tree;
		prime_signature_chunk_t *author;
		prime_signature_chunk_t *org;
	} signatures;
} prime_message_t;

typedef struct __attribute__ ((packed)) {
	prime_type_t type;
	prime_flags_t flags;
	struct {
		union {
			prime_org_key_t *org;
			prime_user_key_t *user;
		};
	} key;
	struct {
		union {
			prime_org_signet_t *org;
			prime_user_signet_t *user;
		};
	} signet;
	struct {
		union {
			prime_message_t *naked;
			prime_message_t *native;
		};
	} message;
} prime_t;

/// prime.c
prime_t *     prime_alloc(prime_type_t type, prime_flags_t flags);
void          prime_cleanup(prime_t *object);
void          prime_free(prime_t *object);
stringer_t *  prime_get(prime_t *object, prime_encoding_t encoding, stringer_t *output);
prime_t *     prime_key_decrypt(stringer_t *key, stringer_t *object, prime_encoding_t encoding, prime_flags_t flags);
stringer_t *  prime_key_encrypt(stringer_t *key, prime_t *object, prime_encoding_t encoding, stringer_t *output);
prime_t *     prime_key_generate(prime_type_t type, prime_flags_t flags);
prime_t *     prime_request_generate(prime_t *object, prime_t *previous);
prime_t *     prime_request_sign(prime_t *request, prime_t *org);
prime_t *     prime_set(stringer_t *object, prime_encoding_t encoding, prime_flags_t flags);
stringer_t *  prime_signet_fingerprint(prime_t *object, stringer_t *output);
prime_t *     prime_signet_generate(prime_t *object);
bool_t        prime_signet_validate(prime_t *object, prime_t *validator);
bool_t        prime_start(void);
void          prime_stop(void);

#include "transposition/transposition.h"
#include "cryptography/cryptography.h"
#include "messages/messages.h"
#include "signets/signets.h"
#include "keys/keys.h"

#endif

