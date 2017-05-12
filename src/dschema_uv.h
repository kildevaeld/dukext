#ifndef DUKEXT_SCHEMA_H
#define DUKEXT_SCHEMA_H

#include "dschema.h"
#include "duktape.h"

duk_bool_t dukext_is_fd(duk_context *ctx, duk_idx_t index);
duk_bool_t dukext_is_handle(duk_context *ctx, duk_idx_t index);
duk_bool_t dukext_is_req(duk_context *ctx, duk_idx_t index);
duk_bool_t dukext_is_fs(duk_context *ctx, duk_idx_t index);
duk_bool_t dukext_is_stream(duk_context *ctx, duk_idx_t index);
duk_bool_t dukext_is_timer(duk_context *ctx, duk_idx_t index);
duk_bool_t dukext_is_tcp(duk_context *ctx, duk_idx_t index);
duk_bool_t dukext_is_pipe(duk_context *ctx, duk_idx_t index);
duk_bool_t dukext_is_tty(duk_context *ctx, duk_idx_t index);

#endif
