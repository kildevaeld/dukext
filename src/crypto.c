#include "crypto.h"

#include "dschema.h"
#include "duk-v1-compat/duk_v1_compat.h"
#include <openssl/md5.h>
#include <openssl/sha.h>

duk_ret_t dukext_crypto_is_hash(duk_context *ctx, duk_idx_t index) {
  dukext_crypto_hash_t *req;
  duk_size_t size;
  if (!duk_is_fixed_buffer(ctx, index))
    return 0;
  req = duk_get_buffer(ctx, index, &size);
  return 1;
}

static dukext_crypto_hash_t *setup_handle(duk_context *ctx, const char *algo) {

  dukext_crypto_hash_t *handle = duk_push_fixed_buffer(ctx, sizeof(handle));
  if (strcmp(algo, "sha") == 0) {
    handle->sha = duk_push_fixed_buffer(ctx, sizeof(handle->sha));
    handle->type = DUKEXT_CRYPTO_HASH_SHA;
    if (!SHA1_Init(handle->sha)) {
    }
  } else if (strcmp(algo, "sha256") == 0) {
    handle->sha256 = duk_push_fixed_buffer(ctx, sizeof(handle->sha256));
    handle->type = DUKEXT_CRYPTO_HASH_SHA256;
    if (!SHA256_Init(handle->sha256)) {
    }

  } else if (strcmp(algo, "sha512") == 0) {
    handle->sha256 = duk_push_fixed_buffer(ctx, sizeof(handle->sha512));
    handle->type = DUKEXT_CRYPTO_HASH_SHA512;
    if (!SHA512_Init(handle->sha512)) {
    }

  } else if (strcmp(algo, "md5") == 0) {
    handle->md5 = duk_push_fixed_buffer(ctx, sizeof(handle->md5));
    handle->type = DUKEXT_CRYPTO_HASH_MD5;
    if (!MD5_Init(handle->md5)) {
    }

  } else {
    duk_error(ctx, 200, "crypto: unknown algorithm: %s", algo);
  }

  duk_pop(ctx);

  return handle;
}

static int update_hash(dukext_crypto_hash_t *handle, void *data,
                       duk_size_t size) {

  switch (handle->type) {
  case DUKEXT_CRYPTO_HASH_MD5:
    return MD5_Update(handle->md5, data, size);
  case DUKEXT_CRYPTO_HASH_SHA:
    return SHA1_Update(handle->sha, data, size);
  case DUKEXT_CRYPTO_HASH_SHA256:
    return SHA256_Update(handle->sha256, data, size);
  case DUKEXT_CRYPTO_HASH_SHA512:
    return SHA512_Update(handle->sha512, data, size);
  }

  return 0;
}

static int digest_hash(duk_context *ctx, dukext_crypto_hash_t *handle) {
  uint8_t *p;
  switch (handle->type) {
  case DUKEXT_CRYPTO_HASH_MD5:
    p = duk_push_fixed_buffer(ctx, MD5_DIGEST_LENGTH);
    return MD5_Final(p, handle->md5);
  case DUKEXT_CRYPTO_HASH_SHA:
    p = duk_push_fixed_buffer(ctx, SHA_DIGEST_LENGTH);
    return SHA1_Final(p, handle->sha);
  case DUKEXT_CRYPTO_HASH_SHA256:
    p = duk_push_fixed_buffer(ctx, SHA256_DIGEST_LENGTH);
    return SHA256_Final(p, handle->sha256);
  case DUKEXT_CRYPTO_HASH_SHA512:
    p = duk_push_fixed_buffer(ctx, SHA512_DIGEST_LENGTH);
    return SHA512_Final(p, handle->sha512);
  }

  return 0;
}

duk_ret_t dukext_crypto_hash_create(duk_context *ctx) {

  dschema_check(
      ctx, (const dukext_schema_entry[]){{"algorithm", duk_is_string}, {NULL}});

  const char *algo = duk_require_string(ctx, 0);

  dukext_crypto_hash_t *handle = setup_handle(ctx, algo);

  return 1;
}
duk_ret_t dukext_crypto_hash_update(duk_context *ctx) {

  dschema_check(ctx,
                (const dukext_schema_entry[]){{"hash", dukext_crypto_is_hash},
                                              {"buffer", duk_is_buffer_data},
                                              {NULL}});
  dukext_crypto_hash_t *handle = duk_get_buffer(ctx, 0, NULL);

  duk_size_t size;
  void *str = duk_get_buffer_data(ctx, 1, &size);

  if (!update_hash(handle, str, size)) {
    duk_error(ctx, 1, "crypto: unknown error");
  }

  return 0;
}
duk_ret_t dukext_crypto_hash_digest(duk_context *ctx) {

  dschema_check(ctx, (const dukext_schema_entry[]){
                         {"hash", dukext_crypto_is_hash}, {NULL}});

  dukext_crypto_hash_t *handle = duk_get_buffer(ctx, 0, NULL);

  if (!digest_hash(ctx, handle)) {
    duk_error(ctx, -1, "crypto: unknown error");
  }

  return 1;
}

static const duk_function_list_entry dukext_funcs[] = {

    {"createHash", dukext_crypto_hash_create, 1},
    {"updateHash", dukext_crypto_hash_update, 2},
    {"digestHash", dukext_crypto_hash_digest, 1},

    {NULL, NULL, 0}};

duk_ret_t dukext_crypto_init(duk_context *ctx) {

  duk_idx_t index = duk_normalize_index(ctx, -2);

  duk_get_prop_string(ctx, index, "exports");
  duk_push_object(ctx);
  duk_put_function_list(ctx, -1, dukext_funcs);

  duk_put_prop_string(ctx, -2, "crypto");
  duk_pop(ctx);
  duk_dump_context_stderr(ctx);

  duk_push_undefined(ctx);
  return 1;
}