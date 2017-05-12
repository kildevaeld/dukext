#ifndef DUKEXT_STREAM_H
#define DUKEXT_STREAM_H

#include "dukext.h"

duk_ret_t dukext_shutdown(duk_context *ctx);
duk_ret_t dukext_listen(duk_context *ctx);
duk_ret_t dukext_accept(duk_context *ctx);
duk_ret_t dukext_read_start(duk_context *ctx);
duk_ret_t dukext_read_stop(duk_context *ctx);
duk_ret_t dukext_write(duk_context *ctx);
duk_ret_t dukext_write2(duk_context *ctx);
duk_ret_t dukext_try_write(duk_context *ctx);
duk_ret_t dukext_is_readable(duk_context *ctx);
duk_ret_t dukext_is_writable(duk_context *ctx);
duk_ret_t dukext_stream_set_blocking(duk_context *ctx);

#endif
