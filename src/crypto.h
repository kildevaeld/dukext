#ifndef DUKEXT_CRYPTO_H
#define DUKEXT_CRYPTO_H

#ifdef __cplusplus
extern "C" {
#endif

#include "dukext.h"
#include <openssl/base.h>

#define DUKEXT_CRYPTO_HASH_MD5 1
#define DUKEXT_CRYPTO_HASH_SHA 2
#define DUKEXT_CRYPTO_HASH_SHA256 3
#define DUKEXT_CRYPTO_HASH_SHA512 4

typedef struct dukext_crypto_hash_s {
  int type;
  void *hash;
  int hash_ref;
  int ref;
} dukext_crypto_hash_t;

duk_bool_t dukext_crypto_is_hash(duk_context *ctx, duk_idx_t index);

duk_ret_t dukext_crypto_hash_create(duk_context *ctx);
duk_ret_t dukext_crypto_hash_update(duk_context *ctx);
duk_ret_t dukext_crypto_hash_digest(duk_context *ctx);
duk_ret_t dukext_crypto_hash_cleanup(duk_context *ctx);

duk_ret_t dukext_crypto_init(duk_context *ctx);

#ifdef __cplusplus
}
#endif

#endif