#ifndef DUKEXT_UTILS_H
#define DUKEXT_UTILS_H

#include "dukext.h"

uv_loop_t *dukext_loop(duk_context *ctx);

void dukext_error(duk_context *ctx, int status);

void dukext_check(duk_context *ctx, int status);

dukext_handle_t *dukext_setup_handle(duk_context *ctx);
dukext_handle_t *dukext_cleanup_handle(duk_context *ctx, dukext_handle_t *data);

void dukext_store_handler(duk_context *ctx, dukext_handle_t *data, int type,
                          int index);
void dukext_emit_event(duk_context *ctx, dukext_handle_t *data,
                       dukext_callback_id type, int nargs);
#endif