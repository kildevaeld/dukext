#include "crypto.h"

#include "dschema.h"
#include "duk-v1-compat/duk_v1_compat.h"
#include "refs.h"
#include <openssl/md5.h>
#include <openssl/sha.h>

#ifdef DUKEXT_NODE_COMPAT
extern unsigned char crypto_js[];
extern unsigned int crypto_js_len;
#endif

duk_ret_t dukext_crypto_is_hash(duk_context *ctx, duk_idx_t index) {
  dukext_crypto_hash_t *req;
  duk_size_t size;
  if (!duk_is_fixed_buffer(ctx, index))
    return 0;
  req = duk_get_buffer(ctx, index, &size);
  return 1;
}

static dukext_crypto_hash_t *setup_handle(duk_context *ctx, const char *algo) {
  dukext_crypto_hash_t *handle;
  handle = duk_push_fixed_buffer(ctx, sizeof(handle));

  if (strcmp(algo, "sha") == 0) {
    handle->hash = malloc(sizeof(SHA_CTX));
    handle->type = DUKEXT_CRYPTO_HASH_SHA;
    if (!SHA1_Init(handle->hash)) {
      goto fail;
    }
  } else if (strcmp(algo, "sha256") == 0) {
    handle->hash = malloc(sizeof(SHA256_CTX));
    handle->type = DUKEXT_CRYPTO_HASH_SHA256;
    if (!SHA256_Init(handle->hash)) {
      goto fail;
    }

  } else if (strcmp(algo, "sha512") == 0) {
    handle->hash = malloc(sizeof(SHA512_CTX));
    handle->type = DUKEXT_CRYPTO_HASH_SHA512;
    if (!SHA512_Init(handle->hash)) {
      goto fail;
    }

  } else if (strcmp(algo, "md5") == 0) {
    handle->hash = malloc(sizeof(MD5_CTX));
    handle->type = DUKEXT_CRYPTO_HASH_MD5;
    if (!MD5_Init(handle->hash)) {
      goto fail;
    }

  } else {
    duk_error(ctx, 200, "crypto: unknown algorithm: %s", algo);
  }

  return handle;

fail:

  return NULL;
}

static int update_hash(dukext_crypto_hash_t *handle, void *data,
                       duk_size_t size) {

  switch (handle->type) {
  case DUKEXT_CRYPTO_HASH_MD5:
    return MD5_Update(handle->hash, data, size);
  case DUKEXT_CRYPTO_HASH_SHA:
    return SHA1_Update(handle->hash, data, size);
  case DUKEXT_CRYPTO_HASH_SHA256:
    return SHA256_Update(handle->hash, data, size);
  case DUKEXT_CRYPTO_HASH_SHA512:
    return SHA512_Update(handle->hash, data, size);
  }

  return 0;
}

static int digest_hash(duk_context *ctx, dukext_crypto_hash_t *handle) {
  uint8_t *p;
  switch (handle->type) {
  case DUKEXT_CRYPTO_HASH_MD5:
    p = duk_push_fixed_buffer(ctx, MD5_DIGEST_LENGTH);
    return MD5_Final(p, handle->hash);
  case DUKEXT_CRYPTO_HASH_SHA:
    p = duk_push_fixed_buffer(ctx, SHA_DIGEST_LENGTH);
    return SHA1_Final(p, handle->hash);
  case DUKEXT_CRYPTO_HASH_SHA256:
    p = duk_push_fixed_buffer(ctx, SHA256_DIGEST_LENGTH);
    return SHA256_Final(p, handle->hash);
  case DUKEXT_CRYPTO_HASH_SHA512:
    p = duk_push_fixed_buffer(ctx, SHA512_DIGEST_LENGTH);
    return SHA512_Final(p, handle->hash);
  }

  return 0;
}

duk_ret_t dukext_crypto_hash_create(duk_context *ctx) {

  dschema_check(
      ctx, (const dukext_schema_entry[]){{"algorithm", duk_is_string}, {NULL}});

  const char *algo = duk_require_string(ctx, 0);

  dukext_crypto_hash_t *handle = setup_handle(ctx, algo);

  if (!handle) {
    duk_error(ctx, 1, "crypto: could not create hash");
  }

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

duk_ret_t dukext_crypto_hash_cleanup(duk_context *ctx) {
  dschema_check(ctx, (const dukext_schema_entry[]){
                         {"hash", dukext_crypto_is_hash}, {NULL}});

  dukext_crypto_hash_t *handle = duk_get_buffer(ctx, 0, NULL);
  free(handle->hash);

  return 0;
}

static const duk_function_list_entry dukext_funcs[] = {

    {"create", dukext_crypto_hash_create, 1},
    {"update", dukext_crypto_hash_update, 2},
    {"digest", dukext_crypto_hash_digest, 1},
    {"clean", dukext_crypto_hash_cleanup, 1},

    {NULL, NULL, 0}};

duk_ret_t dukext_crypto_init(duk_context *ctx) {

  duk_idx_t index = duk_normalize_index(ctx, -2);
  duk_dup(ctx, index);

#ifdef DUKEXT_NODE_COMPAT
  duk_push_object(ctx);
  duk_put_function_list(ctx, -1, dukext_funcs);
  duk_put_prop_string(ctx, -2, "crypto");
  duk_pop_3(ctx);
  duk_push_lstring(ctx, crypto_js, crypto_js_len);
#else
  duk_get_prop_string(ctx, index, "exports");
  duk_put_function_list(ctx, -1, dukext_funcs);
  duk_pop_3(ctx);
  duk_push_undefined(ctx);

#endif
  return 1;
}